//
// Created by George on 6/26/2023.
//

#include "ChatModel.h"

ChatModel::ChatModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

ChatModel::ChatMessage ChatModel::createUserMessage(const QString& message)
{
    ChatMessage chatMessage;
    chatMessage.icon = QIcon(":/resources/user_small.png");
    chatMessage.message = message;
    return chatMessage;
}

ChatModel::ChatMessage ChatModel::createBotMessage(const QString& message)
{
    ChatMessage chatMessage;
    chatMessage.icon = QIcon(":/resources/ai_prof_small.png");
    chatMessage.message = message;
    return chatMessage;
}

void ChatModel::addMessage(const ChatMessage& message)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages << message;
    endInsertRows();
}

void ChatModel::updateMessage(int index, const ChatMessage& message)
{
    if (index < 0 || index >= m_messages.size())
        return;

    m_messages[index].message += message.message;
    emit dataChanged(this->index(index), this->index(index));
}

int ChatModel::rowCount(const QModelIndex& parent) const
{
  return m_messages.size();
}

QVariant ChatModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= m_messages.size())
        return QVariant();

    const ChatMessage& message = m_messages[index.row()];

    switch (role) {
        case IconRole:
            return message.icon;
        case MessageRole:
            return message.message;
    }

    return QVariant();
}

QHash<int, QByteArray> ChatModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IconRole] = "icon";
    roles[MessageRole] = "message";
    return roles;
}
