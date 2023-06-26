//
// Created by George on 6/24/2023.
//

#ifndef UNTITLED2_MAINWINDOW_H
#define UNTITLED2_MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include <QTextEdit>


#include "settingswindow.h"
#include <memory>

class MainWindow: public QMainWindow {
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openSettingsWindow();
    void sendRequest();
    void handleReply(QNetworkReply *reply);
    void handleData();
    void handleEndOfData();
    void processData(QByteArray &data);

private:
    QPushButton *settingsButton;
    QPushButton *searchButton;
    QLineEdit *searchLineEdit;
    QTextEdit *resultsTextEdit;
    std::unique_ptr<SettingsWindow> settingsWindow;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
};


#endif //UNTITLED2_MAINWINDOW_H
