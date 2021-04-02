#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

namespace mtr {

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);
    TcpSocket(const TcpSocket& socket);

    ~TcpSocket();

    int sentData(const QByteArray& data);
    void pause();
    void startup();

signals:
    void sgl_ready_read(TcpSocket* socket, const QByteArray& data);
    void sgl_connected(TcpSocket* socket);
    void sgl_disconnected(TcpSocket* socket);
    void sgl_stateChanged(TcpSocket* socket);

private slots:
    void slot_ready_read();
    void slot_connected();
    void slot_disconnected();
    void slot_stateChanged(QAbstractSocket::SocketState state);
};

}

#endif // TCPSOCKET_H
