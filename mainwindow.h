//
// Created by George on 6/24/2023.
//

#ifndef UNTITLED2_MAINWINDOW_H
#define UNTITLED2_MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QByteArray>
#include <QTextEdit>


#include "settingswindow.h"
#include "ChatModel.h"
#include <memory>

class MainWindow: public QMainWindow {
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openSettingsWindow();
    void sendRequest();

    void handleData();
    void handleEndOfData();
    void processData(QByteArray &data);

private:
    QPushButton *settingsButton;
    QPushButton *searchButton;
    QLineEdit *searchLineEdit;

    std::unique_ptr<SettingsWindow> settingsWindow;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    ChatModel* chatModel;
    QListView* chatListView;

    int currentMessageIndex;
};


#endif //UNTITLED2_MAINWINDOW_H
