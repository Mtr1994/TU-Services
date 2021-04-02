#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>

#include "Public/defines.h"

namespace mtr {

class TcpServer;
class ServerManager : public QObject
{
    Q_OBJECT
private:
    explicit ServerManager(QObject *parent = nullptr);

public:
    static ServerManager* Instance();

    bool addServer(const QString& address, uint32_t port);

public slots:
    void slot_operation_server(int operation, const ServerInfo& info);

signals:
    void sgl_server_operation(int operation, const ServerInfo& info);
    void sgl_client_operation(int operation, const ClientInfo& info);
    void sgl_add_server_status(bool success);

private:
    QMap<QString, TcpServer*> mMapServers;
};

}

#endif // SERVERMANAGER_H
