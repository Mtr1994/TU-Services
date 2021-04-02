#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QMap>
#include <QTcpServer>
#include "Public/defines.h"

namespace mtr {

class TcpSocket;
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(const QString& serverkey, QObject *parent = nullptr);
    ~TcpServer();

    void pauseSocket(uint32_t socketdescriptor);
    void startSocket(uint32_t socketdescriptor);
    void delSocket(uint32_t socketdescriptor);
    void clearSocket();

    QString getServerKey() const {return mServerKey;}

signals:
    void sgl_socket_ready_read(int operation, const ClientInfo& info);
    void sgl_socket_connected(int operation, const ClientInfo& info);
    void sgl_socket_disconnected(int operation, const ClientInfo& info);
    void sgl_socket_stateChanged(int operation, const ClientInfo& info);

public slots:
    void slot_socket_ready_read(TcpSocket* socket, const QByteArray& data);
    void slot_socket_connected(TcpSocket* socket);
    void slot_socket_disconnected(TcpSocket* socket);
    void slot_socket_stateChanged(TcpSocket* socket);

    void slot_new_socket();

private slots:
    ClientInfo getClientInfo(TcpSocket* socket);

private:
    QString mServerKey;
    QMap<uint32_t, TcpSocket*> mMapSockets;
};

}

#endif // TCPSERVER_H
