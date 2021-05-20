#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QMap>
#include <QAbstractSocket>

#include "Public/defines.h"

namespace mtr {

class TcpSocket;
class ClientManager : public QObject
{
    Q_OBJECT
private:
    explicit ClientManager(QObject *parent = nullptr);

public:
    static ClientManager* Instance();

    void connectServer(const QString& address, uint port);

public slots:
    void slot_socket_connect(TcpSocket* socket);
    void slot_socket_disconnect(TcpSocket* socket);
    void slot_ready_read(TcpSocket* socket, const QByteArray& data);
    void slot_stateChanged(TcpSocket* socket);
    void slot_error_occurred(const QString& server, QAbstractSocket::SocketError error);

    void slot_operation_client(int operation, const ClientInfo& info);

signals:
    void sgl_client_operation(int operation, const ClientInfo& info);

private:
    const ClientInfo getSocketInfo(const TcpSocket* socket);

private:
    QMap<uint64_t, TcpSocket*> mMapSocket;
};

}

#endif // CLIENTMANAGER_H
