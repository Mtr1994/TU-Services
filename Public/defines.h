#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <QString>
#include <QByteArray>

enum {Tcp_Server, Tcp_Client_Slave, Tcp_Client, Udp_Server, Udp_Client};

enum ClientOperation{Client_Add = 1, Client_Startup, Client_Pause, Client_Close, Client_Data, Client_StatusChange, Client_Error};

enum ServerOperation{Server_Add = 1, Server_Pause, Server_Startup, Server_Close};

typedef struct {
    int type; // 区分被动接受的套接字和主动连接的套接字
    QString serverkey;
    QString socketkey;
    qintptr socketDescriptor;
    QString error;
    QByteArray data;
}ClientInfo;

typedef struct {
    QString key;
    QString name;
    QString ip;
    uint32_t port;
}ServerInfo;

#endif // DEFINES_H
