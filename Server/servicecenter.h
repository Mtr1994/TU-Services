﻿#ifndef SERVICECENTER_H
#define SERVICECENTER_H

#include <QObject>
#include <QMap>

#include "tcpserver.h"
#include "tcpsocket.h"

class ServiceCenter : public QObject
{
    Q_OBJECT
public:
    explicit ServiceCenter(QObject *parent = nullptr);

    bool addServer(const QString &address, int port);
    void removeServer(const QString& key);

    bool addSocket(const QString &address, int port);
    void removeSocket(int socketptr);

    int sentData(QString& key, int socketptr, const QByteArray& data);

public slots:
    void slot_new_client(const QString& key, const int socketptr, const QString &address,const quint16 port);
    void slot_close_client(const QString& key, const int socketptr, const QString &address,const quint16 port);
    void slot_read_data(int socketptr, const QByteArray &data);

private:
    QMap<QString, TcpServer*> mMapServer;
    QMap<QString, TcpSocket*> mMapSocket;
};

#endif // SERVICECENTER_H
