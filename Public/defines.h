#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <QString>
#include <QByteArray>

enum {Tcp_Server, Tcp_Client, Udp_Server, Udp_Client};

enum ClientOperation{Client_Add = 1, Client_Startup, Client_Pause, Client_Close, Client_Data, Client_StatusChange};

enum ServerOperation{Server_Add = 1, Server_Pause, Server_Startup, Server_Close};

typedef struct {
    QString serverkey;
    QString socketkey;
    QString peerAddress;
    uint32_t peerPort;
    QString localAddress;
    uint32_t localPort;
    qintptr socketDescriptor;
    QByteArray data;
}ClientInfo;

typedef struct {
    QString key;
    QString name;
    QString ip;
    uint32_t port;
}ServerInfo;

#endif // DEFINES_H
