//
// Created by George on 6/24/2023.
//

#include <QSettings>
#include <QGridLayout>
#include "settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :QWidget(parent) {

    apiKeyLabel = new QLabel("API Key", this);
    apiKeyLineEdit = new QLineEdit(this);
    saveButton = new QPushButton("Save", this);

    // Load settings
    QSettings settings ("Test Company", "Test App");
    apiKeyLineEdit->setText(settings.value("apiKey").toString());

    // Layout
     layout = new QGridLayout(this);
     layout->addWidget(apiKeyLabel, 0,0);
     layout->addWidget(apiKeyLineEdit, 0,1);
     layout->addWidget(saveButton, 1,1,1,1, Qt::AlignRight);
     setLayout(layout);

    connect(saveButton, &QPushButton::clicked, this, &SettingsWindow::saveSettings);
}

void SettingsWindow::saveSettings() {
    QSettings settings ("Test Company", "Test App");
    settings.setValue("apiKey", apiKeyLineEdit->text());
}

void SettingsWindow::closeEvent(QCloseEvent *event) {
     saveSettings();
     event->accept();
}
