#include "servicecenter.h"

#include "Buffer/buffers.h"
#include "Log/logger.h"
#include "Public/encoding.h"

ServiceCenter* glService = new ServiceCenter;

ServiceCenter::ServiceCenter(QObject *parent) : QObject(parent)
{

}

/**
 * @brief ServiceCenter::addServer
 * @param address
 * @param port
 * @return
 * 添加服务到缓存
 */

bool ServiceCenter::addServer(const QString &address, int port)
{
    TcpServer* server = new TcpServer;
    connect(server, &TcpServer::sgl_new_client, this, &ServiceCenter::slot_new_client);
    connect(server, &TcpServer::sgl_close_client, this, &ServiceCenter::slot_close_client);
    connect(server, &TcpServer::sgl_read_data, this, &ServiceCenter::slot_read_data);

    bool status = server->listen(QHostAddress(address), port);
    if (status) mMapServer.insert(address + ":" + QString::number(port), server);
    else delete server;

    return status;
}

void ServiceCenter::removeServer(const QString &key)
{
    mMapServer.remove(key);
}

int ServiceCenter::sentData(QString &key, int socketptr, const QByteArray &data)
{
    TcpServer *server = mMapServer.value(key);
    if (nullptr == server) return 0;
    emit server->sgl_sent_data(data, socketptr);
    return 1;
}

void ServiceCenter::slot_new_client(const QString &key, const int socketptr, const QString &address, const quint16 port)
{
    Buffers::getInstance()->addClientItem(key, socketptr, address, port);
    Buffers::getInstance()->appendTabPage(socketptr);

    Buffers::getInstance()->appentTabData(socketptr, QByteArray::fromStdString(QString("新链接 {%1:%2}").arg(address).arg(port).toStdString()));
}

void ServiceCenter::slot_close_client(const QString& key, const int socketptr, const QString &address, const quint16 port)
{
    Buffers::getInstance()->removeClientItem(key, socketptr, address, port);
    Buffers::getInstance()->removeTabPage(socketptr);
}

void ServiceCenter::slot_read_data(int socketptr, const QByteArray &data)
{
    LOG_DEBUG("recv data from " + std::to_string(socketptr));
    Buffers::getInstance()->appentTabData(socketptr, data);
}
