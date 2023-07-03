//
// Created by George on 6/26/2023.
//

#ifndef CHATQT2_CHATMODEL_H
#define CHATQT2_CHATMODEL_H


#include <QAbstractListModel>
#include <QIcon>

class ChatModel: public QAbstractListModel {
    Q_OBJECT

public:
    struct ChatMessage
    {
        QIcon icon;
        QString message;

    };

    enum {
        IconRole = Qt::UserRole + 1,
        MessageRole,
    };

    ChatModel(QObject* parent = nullptr);

    ChatMessage createUserMessage(const QString& message);
    ChatMessage createBotMessage(const QString& message);

    void addMessage(const ChatMessage& message);
    void updateMessage(int index, const ChatMessage& message);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<ChatMessage> m_messages;


};


#endif //CHATQT2_CHATMODEL_H
