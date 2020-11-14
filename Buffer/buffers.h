#ifndef BUFFERS_H
#define BUFFERS_H

#include <QObject>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMap>

#include "Server/tcpserver.h"
#include "Widget/widgettabpages.h"

class Buffers : public QObject
{
    Q_OBJECT
public:
    static Buffers *getInstance();

    QStandardItemModel *getModelItems() const;
    void setModelItems(QStandardItemModel *modelItems);
    void removeItem(int ptr, const QString& ip, int port);
    void addClientItem(const QString& key, const int socketptr, const QString &address,const quint16 port);
    void removeClientItem(const QString& key, const int socketptr, const QString &address,const quint16 port);
    void addServerItem(const QString &address,const quint16 port);
    void removeServerItem(const QString &address,const quint16 port);

    QMap<QString, TcpServer *> getMapServers() const;
    void setMapServers(const QMap<QString, TcpServer *> &mapServers);
    void insertServer(const QString& key, TcpServer* server);
    TcpServer *getTcpServer(const QString& key) const; // ip:port 组成的 key

    QString getServerKey() const;
    void setServerKey(const QString &serverKey);

    int getSocketNumber() const;

    void appendTabPage(int socketptr);
    void removeTabPage(int index);

    void appentTabData(int socketptr, const QByteArray& data);

    void setTabPages(WidgetTabPages *tabPages);

protected:
    // 禁止外部构造和虚构, 子类的"内部"的其他函数可以调用
    explicit Buffers(QObject *parent = nullptr);
    ~Buffers() {}

private:
    // 禁止外部拷贝和赋值操作
    Buffers(const Buffers&) {};
    const Buffers& operator=(const Buffers& bf) {
        return bf;
    };

    QStandardItemModel *mModelSocketItems = nullptr;

    QMap<QString, TcpServer *> mMapServers;

    // 当前选中客户端或者服务端的 Key
    QString mServerKey;

    // 统计共有多少个套接字
    int mSocketNumber = 0;

    // tab 页管理
    WidgetTabPages *mTabPages;
};

#endif // BUFFERS_H
