//
// Created by George on 6/24/2023.
//

#include <QTextEdit>
#include <QSettings>
#include <QJsonObject>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), settingsWindow(std::make_unique<SettingsWindow>() ),
manager(new QNetworkAccessManager(this)) {
    searchLineEdit = new QLineEdit(this);
    searchButton = new QPushButton("Search", this);
    settingsButton = new QPushButton("Settings", this);
    resultsTextEdit = new QTextEdit(this);
    resultsTextEdit->setReadOnly(true);

    // layout

    auto* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(searchLineEdit);
    horizontalLayout->addWidget(searchButton);

    auto *verticalLayout = new QVBoxLayout();
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addWidget(resultsTextEdit);
    verticalLayout->addWidget(settingsButton);

    auto *centralWidget = new QWidget(this);
    centralWidget->setLayout(verticalLayout);
    setCentralWidget(centralWidget);



    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::openSettingsWindow);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::sendRequest);
    //connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::handleReply);
}

MainWindow::~MainWindow() = default;

void MainWindow::openSettingsWindow() {
    settingsWindow->show();
}

void MainWindow::sendRequest() {
    QSettings settings("Test Company", "Test App");
    QString apiKey = settings.value("apiKey").toString();
    QString modelLevel = settings.value("modelLevel").toString();
    QString systemPrompt = settings.value("systemPrompt").toString();

    if (modelLevel.isEmpty()) {
        modelLevel = "gpt-3.5-turbo";
    }
    qDebug() << "API Key:" << apiKey;
    // Setup request
    QNetworkRequest request;
    request.setUrl(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());

    // The JSON data
    QJsonObject messageObject;
    QJsonObject systemMessageObject;
    if (!systemPrompt.isEmpty()) {
        qDebug() << "System prompt:" << systemPrompt;
        systemMessageObject.insert("role", "system");
        systemMessageObject.insert("content", systemPrompt);
    }
    messageObject.insert("role", "user");
    messageObject.insert("content", searchLineEdit->text());

    QJsonArray messagesArray;
    if (!systemPrompt.isEmpty()) {
        messagesArray.append(systemMessageObject);
    }
    messagesArray.append(messageObject);
    qDebug() << "Messages array:" << messagesArray;
    qDebug() << "Model level:" << modelLevel;
    QJsonObject mainObject;
    mainObject.insert("model", modelLevel);
    mainObject.insert("messages", messagesArray);
    mainObject.insert("temperature", 0.7);
    mainObject.insert("stream", true);

    QJsonDocument doc(mainObject);
    qDebug() << "Sending request to:" << request.url();
    // Send request
    QNetworkReply* reply = manager->post(request, QJsonDocument(doc).toJson());
     // Connect the readyRead signal to a slot that will handle the data
    connect(reply, &QNetworkReply::readyRead, this, &MainWindow::handleData);

    // Connect the finished signal to a slot that will handle the end of data
    connect(reply, &QNetworkReply::finished, this, &MainWindow::handleEndOfData);


}

void MainWindow::handleData() {

      // The sender is the QNetworkReply object
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    // Read the available data and process it
    QByteArray data = reply->readAll();

    processData(data);

}
void MainWindow::handleEndOfData() {
    // The sender is the QNetworkReply object
     QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
     if (!reply) {
         qDebug() << "Reply is null";
         return;
     }

    if (reply->error()) {
        resultsTextEdit->setText(reply->errorString());
    } else {
        QByteArray remainingData = reply->readAll();
        qDebug() << "End of stream. Remaining data:" << remainingData;
        // process the remaining data, append it to your resultsTextEdit
        processData(remainingData);
    }

    reply->deleteLater();
}

void MainWindow::handleReply(QNetworkReply *reply) {
    qDebug() << "HTTP status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
qDebug() << "Error:" << reply->error();
   if (reply->error()) {
        resultsTextEdit->setText(reply->errorString());
        return;
    }
    else {
        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = json.object();
        QString responseText = obj["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();
        resultsTextEdit->setText(responseText);
    }
    reply->deleteLater();
}

void MainWindow::processData(QByteArray &data) {
    QString dataString = QString::fromUtf8(data);
    if (dataString.startsWith("data: ")) {
        dataString.remove(0, 6);  // Remove the "data: " prefix
    }

    // Ignore if it's not a JSON object
    if (dataString.isEmpty() || !dataString.startsWith("{")) {
        return;
    }

    // Parse the JSON data
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(dataString.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << parseError.errorString();
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();
    QJsonArray choicesArray = jsonObject["choices"].toArray();
    if (choicesArray.isEmpty()) {
        qDebug() << "Invalid JSON data";
        return;
    }

    QJsonObject choicesObject = choicesArray[0].toObject();
    if (!choicesObject.contains("delta")) {
        qDebug() << "Invalid JSON data";
        return;
    }

    QJsonObject deltaObject = choicesObject["delta"].toObject();
    QString content = deltaObject["content"].toString();

    // Update the QTextEdit with the content
    if (resultsTextEdit) {
        resultsTextEdit->insertPlainText(content);
    } else {
        qDebug() << "resultsTextEdit is null";
    }
}
