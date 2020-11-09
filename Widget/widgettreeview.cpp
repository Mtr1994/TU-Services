#include "widgettreeview.h"
#include "ui_widgettreeview.h"

#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QMenu>
#include <QAction>

#include "Public/treeitemdelegate.h"
#include "Public/defines.h"
#include "Dialog/dialognote.h"
#include "Public/encoding.h"
#include "Buffer/buffers.h"
#include "Log/logger.h"

WidgetTreeView::WidgetTreeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTreeView)
{
    ui->setupUi(this);
}

WidgetTreeView::~WidgetTreeView()
{
    delete ui;
    if (nullptr != mModelSockets) delete mModelSockets;
}

void WidgetTreeView::init(WidgetTabPages *widget)
{
    mTabPages = widget;
    mModelSockets = new QStandardItemModel(this);
    ui->treeView->setItemDelegate(new TreeItemDelegate);
    ui->treeView->setModel(mModelSockets);

    Buffers::getInstance()->setModelItems(mModelSockets);
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

void WidgetTreeView::slot_current_change(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (!current.isValid()) return;
    QStandardItem *item = mModelSockets->itemFromIndex(current);
    if (item->data(Qt::UserRole + 1).toUInt() == Type_Tcp_Client)
        item = mModelSockets->itemFromIndex(current.parent());

    if (nullptr == item) return;

    QString key = item->data(Qt::UserRole + 2).toString() + ":" + item->data(Qt::UserRole + 3).toString();
    Buffers::getInstance()->setServerKey(key);
    emit sgl_current_index_change(mTabPages->getTabCount());
}

void WidgetTreeView::slot_add_socket_item(int index, int type, const QString &address, const QString &port)
{

}

void WidgetTreeView::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    LOG_DEBUG("123");
    // 渲染管道右键菜单
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid()) return;

    QStandardItem *item =mModelSockets->itemFromIndex(index);
    QString id = item->data(Qt::UserRole + 1).toString();
    bool visible = item->checkState() == Qt::Checked;

    QMenu menu(this);
    QAction actionShow(tr("开始"));
    actionShow.setEnabled(!visible);
    connect(&actionShow, &QAction::triggered, [&item]() { item->setCheckState(Qt::Checked); });
    menu.addAction(&actionShow);
    QAction actionHide(tr("暂停"));
    actionHide.setEnabled(visible);
    connect(&actionHide, &QAction::triggered, [&item]() { item->setCheckState(Qt::Unchecked); });
    menu.addAction(&actionHide);
    QAction actionDel(tr("删除"));
    connect(&actionDel, &QAction::triggered, [this, index](){  });
    menu.addAction(&actionDel);
    QAction actionInfo(tr("详细信息"));
    connect(&actionInfo, &QAction::triggered, [this]() {  });
    menu.addAction(&actionInfo);
    menu.exec(QCursor::pos());
}
