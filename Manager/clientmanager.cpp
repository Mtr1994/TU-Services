#include "clientmanager.h"
#include "Net/tcpsocket.h"
#include "Public/softconstants.h"

#include <QHostAddress>

//test
#include <QDebug>
#include <chrono>

using namespace mtr;

ClientManager::ClientManager(QObject *parent) : QObject(parent)
{

}

ClientManager *ClientManager::Instance()
{
    static ClientManager manager;
    return &manager;
}

void ClientManager::connectServer(const QString &address, uint port)
{
    TcpSocket* socket = new TcpSocket;
    connect(socket, &TcpSocket::sgl_connected, this, &ClientManager::slot_socket_connect);
    connect(socket, &TcpSocket::sgl_disconnected, this, &ClientManager::slot_socket_disconnect);

    connect(socket, &TcpSocket::sgl_ready_read, this, &ClientManager::slot_ready_read);
    connect(socket, &TcpSocket::sgl_stateChanged, this, &ClientManager::slot_stateChanged);
    connect(socket, &TcpSocket::sgl_error_occurred, this, &ClientManager::slot_error_occurred);

    socket->connectServer(address, port);
}

void ClientManager::slot_socket_connect(TcpSocket *socket)
{
    mMapSocket.insert(socket->socketDescriptor(), socket);

    auto info = getSocketInfo(socket);
    emit sgl_client_operation(Client_Add, info);
}

void ClientManager::slot_socket_disconnect(TcpSocket *socket)
{
    Q_UNUSED(socket);

    auto info = getSocketInfo(socket);
    emit sgl_client_operation(Client_Pause, info);
}

void ClientManager::slot_ready_read(TcpSocket *socket, const QByteArray &data)
{
    auto info = getSocketInfo(socket);
    info.data = data;

    emit sgl_client_operation(Client_Data, info);
}

void ClientManager::slot_stateChanged(TcpSocket *socket)
{
    Q_UNUSED(socket);
}

void ClientManager::slot_error_occurred(const QString& server, QAbstractSocket::SocketError error)
{
    QString errorStr = SoftConstants::getSocketError(error);

    ClientInfo info {Tcp_Client, server, "", 0, errorStr, ""};

    emit sgl_client_operation(Client_Error, info);
}

void ClientManager::slot_operation_client(int operation, const ClientInfo &info)
{
    if (operation == Client_Data)
    {
        TcpSocket* socket = mMapSocket.value(info.socketDescriptor);
        if (nullptr == socket) return;

        socket->write(info.data);
    }
    else if (operation == Client_Close)
    {
        TcpSocket* socket = mMapSocket.value(info.socketDescriptor);
        if (nullptr == socket) return;

        socket->close();
    }
}

const ClientInfo ClientManager::getSocketInfo(const TcpSocket *socket)
{
    QString serverkey = QString("%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort());
    QString socketkey = QString("%1:%2").arg(socket->localAddress().toString()).arg(socket->localPort());
    qintptr socketDescriptor = socket->socketDescriptor();

    ClientInfo info {Tcp_Client, serverkey, socketkey, socketDescriptor, "", ""};

    return info;
}
