#ifndef BUFFERS_H
#define BUFFERS_H

#include <QObject>
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMap>

class WidgetTabPages;
class WidgetTreeView;
class Buffers : public QObject
{
    Q_OBJECT
public:
    static Buffers *getInstance();

//    QMap<QString, TcpServer *> getMapServers() const;
//    void setMapServers(const QMap<QString, TcpServer *> &mapServers);
//    void insertServer(const QString& key, TcpServer* server);
//    TcpServer *getTcpServer(const QString& key) const; // ip:port 组成的 key

    QString getServerKey() const;
    void setServerKey(const QString &serverKey);

    int getSocketNumber() const;

    void appendTabPage(int socketptr);
    void removeTabPage(int index);

    void appentTabData(int socketptr, const QByteArray& data);

    void setTabPages(WidgetTabPages *tabPages);
    void setTreeView(WidgetTreeView *treeview);

    void setItemExpand(const QStandardItem* item);
    void setCurrentIndex(const QModelIndex &index);

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

    // 当前选中客户端或者服务端的 Key
    QString mServerKey;

    // 统计共有多少个套接字
    int mSocketNumber = 0;

    // tab 页管理
    WidgetTabPages *mTabPages = nullptr;

    // tree view 管理
    WidgetTreeView *mTreeView = nullptr;
};

#endif // BUFFERS_H
