//
// Created by George on 6/24/2023.
//

#ifndef UNTITLED2_SETTINGSWINDOW_H
#define UNTITLED2_SETTINGSWINDOW_H


#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCloseEvent>
#include <QGridLayout>
#include <QComboBox>

class SettingsWindow : public QWidget {
Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow() override = default;


protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void saveSettings();
private:

    QLabel *apiKeyLabel;
    QLineEdit *apiKeyLineEdit;
    QPushButton *saveButton;
    QGridLayout *layout;
    QComboBox *modelLevelComboBox;
    QLineEdit *systemPromptLineEdit;


};


#endif //UNTITLED2_SETTINGSWINDOW_H
