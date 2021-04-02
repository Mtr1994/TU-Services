#ifndef WIDGETTABCONTENT_H
#define WIDGETTABCONTENT_H

#include <QWidget>
#include <QStandardItem>
#include <QByteArray>
#include <QList>

namespace Ui {
class WidgetTabContent;
}

class WidgetTabContent : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetTabContent(int socketptr, QWidget *parent = nullptr);
    ~WidgetTabContent();

    void init();

    void appendError(const QString& data);
    void appendData(const QString& data);

signals:
    void sgl_new_client_coming(const int ptr, const QString & address,const quint16 port, QStandardItem* item);
    void sgl_client_disconnect(const int ptr, const QString & address,const quint16 port, QStandardItem* item);

private slots:
    void on_btnSend_clicked();

private:
    Ui::WidgetTabContent *ui;

    // tab 所对应的套接字
    int mSocketptr = 0;
};

#endif // WIDGETTABCONTENT_H
