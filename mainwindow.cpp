//
// Created by George on 6/24/2023.
//

#include <QTextEdit>
#include <QSettings>
#include <QJsonObject>
#include <QListView>
#include "mainwindow.h"
#include "ChatDelegate.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), settingsWindow(std::make_unique<SettingsWindow>() ),
manager(new QNetworkAccessManager(this)), currentMessageIndex(-1) {
    searchLineEdit = new QLineEdit(this);
    searchButton = new QPushButton("Search", this);
    settingsButton = new QPushButton("Settings", this);


    chatListView = new QListView(this);
    chatModel = new ChatModel(this);

    chatListView->setModel(chatModel);
    chatListView->setItemDelegate(new ChatDelegate(this));


    // layout

    auto* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(searchLineEdit);
    horizontalLayout->addWidget(searchButton);

    auto *verticalLayout = new QVBoxLayout();
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addWidget(chatListView);
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

    // User message to chat
    ChatModel::ChatMessage userMessage = chatModel->createUserMessage(searchLineEdit->text());

    chatModel->addMessage(userMessage);
    currentMessageIndex = -1;

    // AI Message, initially empty
    ChatModel::ChatMessage aiMessage = chatModel->createBotMessage("");
    chatModel->addMessage(aiMessage);

    // Get Current index
    currentMessageIndex = chatModel->rowCount() - 1;


    // Send request
    QNetworkReply* reply = manager->post(request, QJsonDocument(doc).toJson());
     // Connect the readyRead signal to a slot that will handle the data
    connect(reply, &QNetworkReply::readyRead, this, &MainWindow::handleData);

    // Connect the finished signal to a slot that will handle the end of data
    connect(reply, &QNetworkReply::finished, this, &MainWindow::handleEndOfData);


}

void MainWindow::handleData() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    buffer.append(reply->readAll());

    // Find first occurrence of a double newline
    int endOfEvent = buffer.indexOf("\n\n");

    // Keep processing while there are complete events in the buffer
    while (endOfEvent >= 0) {
        QByteArray eventBytes = buffer.left(endOfEvent).trimmed();  // Get complete event

        // Remove processed event from buffer
        buffer.remove(0, endOfEvent + 2);

        // Only process data: events
        if (eventBytes.startsWith("data: ")) {
            eventBytes.remove(0, 6);  // Remove the "data: " prefix
            processData(eventBytes);
        }

        // Check if there is another complete event in the buffer
        endOfEvent = buffer.indexOf("\n\n");
    }
}

void MainWindow::handleEndOfData() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if (!reply) {
        qDebug() << "Reply is null";
        return;
    }

    if (reply->error()) {
        ChatModel::ChatMessage aiMessage = chatModel->createBotMessage(reply->errorString());
        chatModel->updateMessage(currentMessageIndex, aiMessage);
    } else {
        QByteArray remainingData = reply->readAll();
        qDebug() << "End of stream. Remaining data:" << remainingData;

        // append remaining data to buffer, then attempt to process the buffer
        buffer.append(remainingData);
        processData(buffer);
        buffer.clear();
    }

    reply->deleteLater();
    currentMessageIndex = -1;
}

void MainWindow::processData(QByteArray &data) {
    // Parse the JSON data
    QJsonParseError parseError;
    qDebug() << "Attempting to parse data:" << data;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << parseError.errorString();
        return;
    }

    // Extract content
    QJsonObject choicesObject = jsonDoc.object()["choices"].toArray()[0].toObject();
    QString content = choicesObject["delta"].toObject()["content"].toString();
    ChatModel::ChatMessage aiMessage = chatModel->createBotMessage(content);

    // Append content to AI message
    if (!content.isEmpty()) {
        if (currentMessageIndex >= 0) {
            chatModel->updateMessage(currentMessageIndex, aiMessage);
        }
    }
}