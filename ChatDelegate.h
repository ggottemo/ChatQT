//
// Created by George on 6/26/2023.
//

#ifndef CHATQT2_CHATDELEGATE_H
#define CHATQT2_CHATDELEGATE_H

#include <QStyledItemDelegate>

class ChatDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ChatDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};


#endif //CHATQT2_CHATDELEGATE_H
