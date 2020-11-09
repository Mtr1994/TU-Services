#ifndef WIDGETTREEVIEW_H
#define WIDGETTREEVIEW_H

#include <QWidget>
#include <QString>
#include <QModelIndex>
#include <QStandardItemModel>

#include "widgettabpages.h"

namespace Ui {
class WidgetTreeView;
}

class WidgetTreeView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTreeView(QWidget *parent = nullptr);
    ~WidgetTreeView();

    void init(WidgetTabPages* widget);

    void deleteItem(int index);

    bool contains(int index);

signals:
    void sgl_current_index_change(int index);

private slots:
    void on_treeView_customContextMenuRequested(const QPoint &pos);

private:
    void slot_current_change(const QModelIndex &current, const QModelIndex &previous);
    void slot_add_socket_item(int index, int type, const QString& address, const QString& port);

private:
    Ui::WidgetTreeView *ui;
    QStandardItemModel *mModelSockets = nullptr;

    WidgetTabPages *mTabPages = nullptr;
};

#endif // WIDGETTREEVIEW_H
