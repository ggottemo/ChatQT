//
// Created by George on 6/24/2023.
//
#include <QApplication>
#include <QDebug>
#include <QSslSocket>
#include "mainwindow.h"
#include <QIcon>

int main(int argc, char *argv[]) {
     qDebug() << "OpenSSL supported:" << QSslSocket::supportsSsl();
     qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    QCoreApplication::addLibraryPath("./plugins");
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("./resources/icon.ico"));
    MainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}