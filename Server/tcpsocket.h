#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>

#include <QTime>


class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~TcpSocket();
    QByteArray handleData(QByteArray data,const QString & ip, qint16 port);//用来处理数据的函数

    qintptr getSocketDescriptor() const;

signals:
    void sgl_read_data(int socketptr, const QByteArray &);
    void sgl_close_client(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的

public slots:
    void slot_sent_data(const QByteArray & ,const int);//发送信号的槽
    void disConTcp(int i);

protected slots:
    void slot_read_data();//接收数据
    void startNext();//处理下一个
protected:
    QFutureWatcher<QByteArray> watcher;
    QQueue<QByteArray> datas;
private:
    qintptr mSocketDescriptor;
    QMetaObject::Connection dis;
};

#endif // TCPSOCKET_H
