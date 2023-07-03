// chatdelegate.cpp
#include <QPainter>
#include "chatdelegate.h"
#include "chatmodel.h"

ChatDelegate::ChatDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

void ChatDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    QIcon icon = qvariant_cast<QIcon>(index.data(ChatModel::IconRole));
    QString message = qvariant_cast<QString>(index.data(ChatModel::MessageRole));

    // QRect with a fixed size for the icon
    QRect iconRect = option.rect;
    iconRect.setSize(QSize(20, 20));
    iconRect.moveLeft(option.rect.left() + 10);
    iconRect.moveTop(option.rect.top() + 10);

    // Paint the icon and the text
    icon.paint(painter, iconRect, Qt::AlignLeft);
    painter->drawText(option.rect.adjusted(60, 0, 0, 0), message);

    painter->restore();
}

QSize ChatDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // Fetch the message from the model
    QString message = qvariant_cast<QString>(index.data(ChatModel::MessageRole));

    // Calculate the bounding rectangle for the text when it's word-wrapped to the width of the list view
    // You may need to adjust the width value to leave space for the icon
    QFontMetrics metrics(option.font);
    QRect textRect = metrics.boundingRect(0, 0, option.rect.width() - 60, INT_MAX, Qt::AlignLeft | Qt::TextWordWrap, message);

    // The size hint is the size of the text plus some extra space
    return textRect.size() + QSize(10, 10);  // adding 10 for some extra space
}