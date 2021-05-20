#include "widgettabcontent.h"
#include "ui_widgettabcontent.h"

#include <QDateTime>
#include <QTextCodec>

#include "Public/defines.h"
#include "Log/logger.h"
#include "Public/softconstants.h"

// test
#include <QDebug>

WidgetTabContent::WidgetTabContent(const QString& key, int socketptr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTabContent), mServerKey(key), mSocketptr(socketptr)
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

    SoftConstants::fillComboBox(SoftConstants::Item_CodeType, ui->cbbCodeType);
}

void WidgetTabContent::appendError(const QString &data)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->tbDatas->append("<p style=\"font-family:Perpetua; color:#999999; font-size:50%\">" + time + ":</p>" + "<p color='#fd5401'> &nbsp;&nbsp;&nbsp;&nbsp;" + data + "</p>");
}

void WidgetTabContent::appendData(const QString &data)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->tbDatas->append("<p style=\"font-family:Perpetua; color:#999999; font-size:50%\">" + time + ":</p>" + "<p color='#666666'> &nbsp;&nbsp;&nbsp;&nbsp;" + data + "</p>");
}

void WidgetTabContent::applySendResult(int length)
{
    if (length == 0) return appendError("发送失败");

     QByteArray array = QByteArray::fromStdString(ui->tbSendDatas->toPlainText().toStdString());

    LOG_DEBUG("write " + QString::number(length).toStdString() + " bytes of data");
    appendData(QString::fromStdString(array.toStdString()));
    ui->tbSendDatas->clear();
    ui->tbSendDatas->clearFocus();
}

std::string WidgetTabContent::getCurrentCodeTypeName()
{
    return ui->cbbCodeType->currentText().toStdString().data();
}

/**
 * @brief WidgetTabContent::on_btnSend_clicked
 * 发送数据接口
 */

void WidgetTabContent::on_btnSend_clicked()
{
    if (ui->tbSendDatas->toPlainText().isEmpty()) return;

    QByteArray array = QByteArray::fromStdString(ui->tbSendDatas->toPlainText().toStdString());

    QTextCodec *codec = QTextCodec::codecForName(ui->cbbCodeType->currentText().toStdString().data());
    array = codec->fromUnicode(array);

    if (ui->menuHex->isChecked())
    {
        array = array.toHex();
        array = array.toUpper();
    }

    ClientInfo info {0, mServerKey, "", mSocketptr, "", array};

    emit sgl_client_operation(Client_Data, info);
}
