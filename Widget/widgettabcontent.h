#ifndef WIDGETTABCONTENT_H
#define WIDGETTABCONTENT_H

#include <QWidget>
#include <QStandardItem>
#include <QByteArray>
#include <QList>

#include "Public/defines.h"

namespace Ui {
class WidgetTabContent;
}

class WidgetTabContent : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetTabContent(const QString& key, int socketptr, QWidget *parent = nullptr);
    ~WidgetTabContent();

    void init();

    void appendError(const QString& data);
    void appendData(const QString& data);

    void applySendResult(int length);

    std::string getCurrentCodeTypeName();

signals:
    void sgl_client_operation(int operation, const ClientInfo& info);

private slots:
    void on_btnSend_clicked();

private:
    Ui::WidgetTabContent *ui;

    // 服务端地址
    QString mServerKey;

    // tab 所对应的套接字
    int mSocketptr = 0;
};

#endif // WIDGETTABCONTENT_H
