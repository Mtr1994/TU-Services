#include "tcpserver.h"
#include "tcpsocket.h"
#include "Log/logger.h"

using namespace mtr;

TcpServer::TcpServer(const QString &serverkey, QObject *parent)
    : QTcpServer(parent), mServerKey(serverkey)
{

}

TcpServer::~TcpServer()
{
    LOG_DEBUG("delete tcpserver");
}

void TcpServer::pauseSocket(uint32_t socketdescriptor)
{
   // mMapSockets[socketdescriptor].pause();
}

void TcpServer::startSocket(uint32_t socketdescriptor)
{
   // mMapSockets[socketdescriptor].startup();
}

void TcpServer::delSocket(uint32_t socketdescriptor)
{
   // mMapSockets[socketdescriptor].close();
   // mMapSockets.take(socketdescriptor).deleteLater();
}

void TcpServer::clearSocket()
{
//    for (auto& socket : mMapSockets)
//    {
//       // if (socket.isOpen()) socket.close();
    //    }
}

void TcpServer::slot_socket_ready_read(TcpSocket* socket, const QByteArray &data)
{
    ClientInfo info = getClientInfo(socket);
    info.data = data;
    emit sgl_socket_ready_read(Client_Data, info);
}

void TcpServer::slot_socket_connected(TcpSocket* socket)
{
    ClientInfo info = getClientInfo(socket);
    emit sgl_socket_connected(Client_Startup, info);
}

void TcpServer::slot_socket_disconnected(TcpSocket* socket)
{
    ClientInfo info = getClientInfo(socket);
    emit sgl_socket_disconnected(Client_Pause, info);
}

void TcpServer::slot_socket_stateChanged(TcpSocket* socket)
{
    ClientInfo info = getClientInfo(socket);
    emit sgl_socket_stateChanged(Client_StatusChange, info);
}

void TcpServer::slot_new_socket()
{
    QTcpSocket* socket = nextPendingConnection();
    if (nullptr == socket) return;

    TcpSocket* tcpsocket = new TcpSocket;
    tcpsocket->setSocketDescriptor(socket->socketDescriptor());
    mMapSockets.insert(socket->socketDescriptor(), tcpsocket);

    connect(tcpsocket, &TcpSocket::sgl_ready_read, this, &TcpServer::slot_socket_ready_read);
    connect(tcpsocket, &TcpSocket::sgl_connected, this, &TcpServer::slot_socket_connected);
    connect(tcpsocket, &TcpSocket::sgl_disconnected, this, &TcpServer::slot_socket_disconnected);
    connect(tcpsocket, &TcpSocket::sgl_stateChanged, this, &TcpServer::slot_socket_stateChanged);

    ClientInfo info = getClientInfo(tcpsocket);
    emit sgl_socket_connected(Client_Add, info);
}

ClientInfo TcpServer::getClientInfo(TcpSocket *socket)
{
    ClientInfo info
    {
        mServerKey,
        socket->peerAddress().toString() + ":" + QString::number(socket->peerPort()),
        socket->peerAddress().toString(),
        socket->peerPort(),
        socket->localAddress().toString(),
        socket->localPort(),
        socket->socketDescriptor(),
        nullptr
    };

    return info;
}
