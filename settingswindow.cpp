//
// Created by George on 6/24/2023.
//

#include <QSettings>
#include <QGridLayout>
#include "settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :QWidget(parent) {

    apiKeyLabel = new QLabel("API Key", this);
    apiKeyLineEdit = new QLineEdit(this);
    systemPromptLineEdit = new QLineEdit(this);
    modelLevelComboBox = new QComboBox(this);
    saveButton = new QPushButton("Save", this);

    modelLevelComboBox->addItem("gpt-3.5-turbo");
    modelLevelComboBox->addItem("gpt-4");

    // Load settings
    QSettings settings ("Test Company", "Test App");
    apiKeyLineEdit->setText(settings.value("apiKey").toString());
    modelLevelComboBox->setCurrentText(settings.value("modelLevel").toString());
    systemPromptLineEdit->setText(settings.value("systemPrompt").toString());

    // Layout
     auto* vLayout = new QVBoxLayout();
    vLayout->addWidget(new QLabel("API Key:"));
    vLayout->addWidget(apiKeyLineEdit);
    vLayout->addWidget(new QLabel("Model:"));
    vLayout->addWidget(modelLevelComboBox);
    vLayout->addWidget(new QLabel("Initial System Message:"));
    vLayout->addWidget(systemPromptLineEdit);
    vLayout->addWidget(saveButton);

    setLayout(vLayout);

    connect(saveButton, &QPushButton::clicked, this, &SettingsWindow::saveSettings);
}

void SettingsWindow::saveSettings() {
    QSettings settings ("Test Company", "Test App");
    settings.setValue("apiKey", apiKeyLineEdit->text());
    settings.setValue("modelLevel", modelLevelComboBox->currentText());
    settings.setValue("systemPrompt", systemPromptLineEdit->text());

    settings.sync();
}

void SettingsWindow::closeEvent(QCloseEvent *event) {
     saveSettings();
     event->accept();
}
