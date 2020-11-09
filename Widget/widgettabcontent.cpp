#include "widgettabcontent.h"
#include "ui_widgettabcontent.h"

#include <QDateTime>

#include "Public/defines.h"
#include "Public/encoding.h"
#include "Buffer/buffers.h"
#include "Log/logger.h"
#include "Server/servicecenter.h"

// test
#include <QDebug>

extern ServiceCenter* glService;

WidgetTabContent::WidgetTabContent(int socketptr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTabContent), mSocketptr(socketptr)
{
    ui->setupUi(this);

    init();
}

WidgetTabContent::~WidgetTabContent()
{
    delete ui;
}

void WidgetTabContent::init()
{
    ui->splitter->setStretchFactor(0, 8);
    ui->splitter->setStretchFactor(1, 2);
}

void WidgetTabContent::appendError(const QString &data)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->tbDatas->append("<p style=\"font-family:Perpetua; color:#999999; font-size:50%\">" + time + ":</p>" + "<p color='#fd5401'> &nbsp;&nbsp;&nbsp;&nbsp;" + data+ "</p>");
}

void WidgetTabContent::appendData(const QString &data)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->tbDatas->append("<p style=\"font-family:Perpetua; color:#999999; font-size:50%\">" + time + ":</p>" + "<p color='#666666'> &nbsp;&nbsp;&nbsp;&nbsp;" + data+ "</p>");
}

/**
 * @brief WidgetTabContent::on_btnSend_clicked
 * 发送数据接口
 */

void WidgetTabContent::on_btnSend_clicked()
{
    QString key = Buffers::getInstance()->getServerKey();
    if (key.isEmpty()) return;
    QByteArray array = QByteArray::fromStdString(ui->tbSendDatas->toPlainText().toStdString());
    if (ui->menuHex->isChecked())
    {
        array = array.toHex();
        array = array.toUpper();
    }

    int size = glService->sentData(key, mSocketptr, array);
    if (size == 0) return appendError("发送失败");

    LOG_DEBUG("write " + QString::number(size).toStdString() + " bytes of data");
    appendData(QString::fromStdString(array.toStdString()));
    ui->tbSendDatas->clear();
    ui->tbSendDatas->clearFocus();
}
