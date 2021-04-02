#include "widgettreeview.h"
#include "ui_widgettreeview.h"

#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QMenu>
#include <QAction>

#include "Public/treeitemdelegate.h"
#include "Public/defines.h"
#include "Dialog/dialognote.h"
#include "Buffer/buffers.h"
#include "Log/logger.h"

using namespace mtr;

WidgetTreeView::WidgetTreeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTreeView)
{
    ui->setupUi(this);

    //
    init();
}

WidgetTreeView::~WidgetTreeView()
{
    delete ui;
    if (nullptr != mModelSockets) delete mModelSockets;
}

void WidgetTreeView::init()
{
    mModelSockets = new QStandardItemModel(this);
    ui->treeView->setItemDelegate(new TreeItemDelegate);
    ui->treeView->setModel(mModelSockets);
    // 信号管理
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &WidgetTreeView::slot_current_change);
}

void WidgetTreeView::deleteItem(int index)
{
    if (!contains(index)) return;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, index, 1, Qt::MatchExactly | Qt::MatchRecursive);
    int row = list.at(0).row();
    mModelSockets->removeRow(row);
}

bool WidgetTreeView::contains(int index)
{
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, index, 1, Qt::MatchExactly | Qt::MatchRecursive);
    return list.length() > 0;
}

void WidgetTreeView::setItemExpand(const QStandardItem *item)
{
    if (nullptr == item) return;
    ui->treeView->expand(item->index());
}

void WidgetTreeView::setCurrentIndex(const QModelIndex &index)
{
    if (!index.isValid()) return;
    ui->treeView->setCurrentIndex(index);
}

void WidgetTreeView::slot_server_operation(int operation, const ServerInfo &info)
{
    if (operation == Server_Add)
    {
        LOG_DEBUG("add server");
        QString key = QString("%1:%2").arg(info.ip).arg(info.port);
        QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/server.png"), info.key);
        item->setData(Tcp_Server, Qt::UserRole + 1);
        item->setData(info.name, Qt::UserRole + 2);
        item->setData(info.ip, Qt::UserRole + 3);
        item->setData(info.port, Qt::UserRole + 4);
        mModelSockets->appendRow(item);
    }
    else if (operation == Server_Close)
    {
         QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::DisplayRole, info.key, 1, Qt::MatchExactly | Qt::MatchRecursive);
         if (list.isEmpty()) return;
         mModelSockets->removeRow(list.at(0).row());
    }
}

void WidgetTreeView::slot_client_operation(int operation, const ClientInfo &info)
{
    if (operation == Client_Add)
    {
        QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::DisplayRole, info.socketkey, 1, Qt::MatchExactly | Qt::MatchRecursive);
        if (list.isEmpty())
        {
            LOG_DEBUG("add client");
            QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/client.png"), info.socketkey);
            item->setData(Tcp_Client, Qt::UserRole + 1);
            item->setData(info.peerAddress, Qt::UserRole + 2);
            item->setData(info.peerPort, Qt::UserRole + 3);
            item->setData(info.socketDescriptor, Qt::UserRole + 4);

            list = mModelSockets->match(mModelSockets->index(0, 0), Qt::DisplayRole, info.serverkey, 1, Qt::MatchExactly | Qt::MatchRecursive);
            if (list.isEmpty())
            {
                delete item;
                return;
            }
            QStandardItem *parentItem = mModelSockets->itemFromIndex(list.at(0));
            if (nullptr == parentItem)
            {
                delete item;
                return;
            }
            else
            {
                parentItem->appendRow(item);
            }
        }
    }
}

void WidgetTreeView::slot_current_change(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (!current.isValid()) return;
    QStandardItem *item = mModelSockets->itemFromIndex(current);
    if (nullptr == item) return;
    if (item->data(Qt::UserRole + 1).toUInt() == Tcp_Server) return;
    QString key = item->data(Qt::UserRole + 6).toString();
    Buffers::getInstance()->setServerKey(key);
    // 使用 socketptr 定位 tab index
   // emit sgl_current_index_change(item->data(Qt::UserRole + 4).toUInt());
}

void WidgetTreeView::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    // 渲染管道右键菜单
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid()) return;

    QStandardItem *item =mModelSockets->itemFromIndex(index);
    int type = item->data(Qt::UserRole + 1).toUInt();
    QString start = (type == Tcp_Server) ? "启动监听" : "启动连接";
    QString stop = (type == Tcp_Server) ? "停止监听" : "断开连接";
    QString del = (type == Tcp_Server) ? "删除服务" : "删除连接";
    QString key = item->text();

    QMenu menu(this);
    QAction actionStart(start);
    //actionStart.setEnabled(!working);
    connect(&actionStart, &QAction::triggered, [&key, type]() {
       // if (type == Type_Tcp_Server)
            //ServiceCenter::getService().startServer(key);
    });
    menu.addAction(&actionStart);
    QAction actionStop(stop);
    //actionStop.setEnabled(working);
    connect(&actionStop, &QAction::triggered, [&key, type]() {
       // if (type == Type_Tcp_Server)
            //ServiceCenter::getService().stopServer(key);
    });
    menu.addAction(&actionStop);
    QAction actionDel(del);
    connect(&actionDel, &QAction::triggered, [this, &key](){
        ServerInfo info{key, "", "", 0};
        emit sgl_server_operation(Server_Close, info);
        slot_server_operation(Server_Close, info);
    });
    menu.addAction(&actionDel);

    menu.exec(QCursor::pos());
}
