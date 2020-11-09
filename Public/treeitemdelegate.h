#ifndef TREEITEMDELEGATE_H
#define TREEITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QPainter>

#include <QRect>

class TreeItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TreeItemDelegate(){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QRect rect(option.rect);
        rect.setLeft(0);
        if (option.state & QStyle::State_Selected )
        {
            painter->fillRect(rect, QBrush("#aa7cb3f1"));
        }
        else if (option.state & QStyle::State_MouseOver )
        {
            painter->fillRect(rect, QBrush("#557cb3f1"));
        }

        QStyledItemDelegate::paint(painter, option, index);
    }
};

#endif // TREEITEMDELEGATE_H
