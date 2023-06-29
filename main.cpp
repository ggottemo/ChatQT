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
    QIcon winIcon (":/resources/icon.ico");
    if (winIcon.isNull()) {
        qDebug() << "Icon is null";
    }
    app.setWindowIcon(winIcon);
    QPixmap pixmap(":/resources/user_small.png");
if (pixmap.isNull()) {
    qDebug() << "Failed to load pixmap from image file";
}

    MainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}