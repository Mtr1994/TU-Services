#include "tcpsocket.h"

using namespace mtr;

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
    connect(this, &QTcpSocket::readyRead, this, &TcpSocket::slot_ready_read);
    connect(this, &TcpSocket::connected, this, &TcpSocket::slot_connected);
    connect(this, &TcpSocket::disconnected, this, &TcpSocket::slot_disconnected);
    connect(this, &TcpSocket::stateChanged, this, &TcpSocket::slot_stateChanged);
}

TcpSocket::TcpSocket(const TcpSocket& socket)
{
    this->setSocketDescriptor(socket.socketDescriptor());
}

TcpSocket::~TcpSocket()
{

}

int TcpSocket::sentData(const QByteArray &data)
{
    return write(data, data.length());
}

void TcpSocket::pause()
{
    setSocketState(UnconnectedState);
}

void TcpSocket::startup()
{
    setSocketState(ConnectedState);
}

void TcpSocket::slot_ready_read()
{
    emit sgl_ready_read(this, readAll());
}

void TcpSocket::slot_connected()
{
    emit sgl_connected(this);
}

void TcpSocket::slot_disconnected()
{
    emit sgl_disconnected(this);
}

void TcpSocket::slot_stateChanged(QAbstractSocket::SocketState state)
{
    Q_UNUSED(state);
    emit sgl_stateChanged(this);
}

