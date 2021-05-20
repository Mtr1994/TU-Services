#include "widgettabpages.h"
#include "ui_widgettabpages.h"

#include <QTabBar>
#include <QTextCodec>

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

void WidgetTabPages::addTab(const QString& key, int socketptr)
{
    qDebug() << "key " <<key;
    int current = 0;
    if (ui->tabWidget->count() > 0)
    {
        current = ui->tabWidget->currentIndex();
    }

    WidgetTabContent *content = new WidgetTabContent(key, socketptr, this);
    ui->tabWidget->addTab(content, "-----");
    ui->tabWidget->setCurrentIndex(current);

    connect(content, &WidgetTabContent::sgl_client_operation, this, &WidgetTabPages::sgl_client_operation);

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

    QTextCodec *codec = QTextCodec::codecForName(content->getCurrentCodeTypeName().data());
    QString string = codec->toUnicode(data);

    content->appendData(string);
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

void WidgetTabPages::slot_client_operation(int operation, const ClientInfo &info)
{
    // 客户端的各种动作，不同的消息给出不同的操作
    switch (operation) {
    case Client_Add:
        this->addTab(info.serverkey, info.socketDescriptor);
        break;
    case Client_Data:
        this->appentData(info.socketDescriptor, info.data);
        break;
    case Client_Close:
        this->removeTab(info.socketDescriptor);
        break;
    }
}

void WidgetTabPages::slot_socket_write(uint32_t socketdescriptor, int length)
{
    if (!mMapContent.contains(socketdescriptor)) return;
    auto widget = mMapContent.value(socketdescriptor);
    widget->applySendResult(length);
}
