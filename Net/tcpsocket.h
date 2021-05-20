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

    void connectServer(const QString& address, uint port);

signals:
    void sgl_ready_read(TcpSocket* socket, const QByteArray& data);
    void sgl_connected(TcpSocket* socket);
    void sgl_disconnected(TcpSocket* socket);
    void sgl_stateChanged(TcpSocket* socket);
    void sgl_error_occurred(const QString& server, QAbstractSocket::SocketError error);

private slots:
    void slot_ready_read();
    void slot_connected();
    void slot_disconnected();
    void slot_stateChanged(QAbstractSocket::SocketState state);
    void slot_error_occurr(QAbstractSocket::SocketError error);

private:
    QString mAddress;
    uint mPort;
};

}

#endif // TCPSOCKET_H
