#include "widgettabpages.h"
#include "ui_widgettabpages.h"

#include <QTabBar>

#include "Public/defines.h"
#include "Widget/widgettabcontent.h"

// test
#include <QDebug>

WidgetTabPages::WidgetTabPages(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTabPages)
{
    ui->setupUi(this);

    init();
}

WidgetTabPages::~WidgetTabPages()
{
    delete ui;
}

void WidgetTabPages::init()
{
    ui->tabWidget->tabBar()->hide();
}

int WidgetTabPages::getTabCount()
{
    return ui->tabWidget->count();
}

void WidgetTabPages::addTab(int socketptr)
{
    int current = 0;
    if (ui->tabWidget->count() > 0)
        current = ui->tabWidget->currentIndex();
    WidgetTabContent *content = new WidgetTabContent(socketptr, this);
    ui->tabWidget->addTab(content, "-----");
    ui->tabWidget->setCurrentIndex(current);
    mMapContent.insert(socketptr, content);
}

void WidgetTabPages::removeTab(int socketptr)
{
    WidgetTabContent *content = mMapContent.take(socketptr);
    if (nullptr == content) return;
    int index = ui->tabWidget->indexOf(content);
    if (index < 0) return;
    ui->tabWidget->removeTab(index);
}

void WidgetTabPages::appentData(int socketptr, const QByteArray &data)
{
    WidgetTabContent *content = mMapContent.value(socketptr);
    if (nullptr == content) return;
    content->appendData(data);
}

/// <summary>
/// 如果是新添加 Content，就新建，否则改变当前 Index
/// </summary>
/// <param name="item"></param>
void WidgetTabPages::slot_current_index_change(int socketptr)
{
    if (!mMapContent.contains(socketptr)) return;
    ui->tabWidget->setCurrentWidget(mMapContent.value(socketptr));
}

void WidgetTabPages::slot_new_client_coming(const int ptr, const QString &address, const quint16 port, QStandardItem *parentitem)
{
    int index = ui->tabWidget->count();
    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/client.png"), QString("%1:%2").arg(address).arg(port));
    item->setData(index, Qt::UserRole + 1);
    item->setData(Tcp_Client, Qt::UserRole + 2); // tcp_server tcp_client 等
    item->setData(address, Qt::UserRole + 3);
    item->setData(port, Qt::UserRole + 4);
    item->setData(ptr, Qt::UserRole + 5); // 套接字

    parentitem->appendRow(item);
}
