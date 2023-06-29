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

//    QRect iconRect = option.rect;
//    iconRect.setSize(QSize(50, 50));
//    iconRect.moveLeft(option.rect.left() + 10);
//    iconRect.moveTop(option.rect.top() + 10);
//
//    painter->fillRect(iconRect, Qt::red);

    icon.paint(painter, option.rect, Qt::AlignLeft);
    painter->drawText(option.rect.adjusted(60, 0, 0, 0), message); // adjust these parameters as needed

    painter->restore();
}

QSize ChatDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // adjust this to your needs
    return QSize(option.rect.width(), 50);
}
