#ifndef WIDGETTREEVIEW_H
#define WIDGETTREEVIEW_H

#include <QWidget>
#include <QString>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStandardItem>

#include "Manager/servermanager.h"
#include "Manager/clientmanager.h"

namespace Ui {
class WidgetTreeView;
}

class WidgetTreeView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTreeView(QWidget *parent = nullptr);
    ~WidgetTreeView();

    void init();

    void deleteItem(int index);

    bool contains(int index);

    void setItemExpand(const QStandardItem* item);

    void setCurrentIndex(const QModelIndex& index);

signals:
    void sgl_current_socket_change(int index);
    void sgl_server_operation(int operation, const ServerInfo& info);

public slots:
    void slot_server_operation(int operation, const ServerInfo& info);
    void slot_client_operation(int operation, const ClientInfo& info);

private slots:
    void on_treeView_customContextMenuRequested(const QPoint &pos);

private:
    void slot_current_change(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::WidgetTreeView *ui;
    QStandardItemModel *mModelSockets = nullptr;
};

#endif // WIDGETTREEVIEW_H
