#include "servermanager.h"
#include "Net/tcpserver.h"
#include "Public/defines.h"

using namespace mtr;

ServerManager::ServerManager(QObject *parent) : QObject(parent)
{

}

ServerManager* ServerManager::Instance()
{
    static ServerManager server;
    return &server;
}

bool ServerManager::addServer(const QString &address, uint32_t port)
{
    TcpServer* server = new TcpServer(address + ":" + QString::number(port));
    bool status = server->listen(QHostAddress(address), port);
    if (status)
    {
        connect(server, &TcpServer::sgl_socket_ready_read, this, &ServerManager::sgl_client_operation);
        connect(server, &TcpServer::sgl_socket_connected, this, &ServerManager::sgl_client_operation);
        connect(server, &TcpServer::sgl_socket_disconnected, this, &ServerManager::sgl_client_operation);
        connect(server, &TcpServer::sgl_socket_stateChanged, this, &ServerManager::sgl_client_operation);

        connect(server, &TcpServer::newConnection, server, &TcpServer::slot_new_socket);

        ServerInfo info{server->getServerKey(), "服务器", address, port};
        emit sgl_server_operation(Server_Add, info);
        mMapServers.insert(info.key, server);
    }
    else
    {
        delete server;
    }

    emit sgl_add_server_status(status);
    return status;
}

void ServerManager::slot_operation_server(int operation, const ServerInfo &info)
{
    if (operation == Server_Close)
    {
        TcpServer* server = mMapServers.take(info.key);
        if (nullptr == server) return;
        server->clearSocket();
        server->close();
        delete server;
    }
}
