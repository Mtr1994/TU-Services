#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>

TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),mSocketDescriptor(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::slot_read_data);
    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            emit sgl_close_client(mSocketDescriptor, this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
}

TcpSocket::~TcpSocket()
{

}

void TcpSocket::slot_sent_data(const QByteArray &data, const int id)
{
    if(id != mSocketDescriptor) return;
    write(data);
}

void TcpSocket::disConTcp(int i)
{
    if (i == mSocketDescriptor)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::slot_read_data()
{
    emit sgl_read_data(mSocketDescriptor, readAll());
}

QByteArray TcpSocket::handleData(QByteArray data, const QString &ip, qint16 port)
{
    QTime time;
    time.start();

    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}
    data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
    return data;
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
    }
}
