#include "buffers.h"
#include "Log/logger.h"
#include "Public/defines.h"

static Buffers* buffer = nullptr;

Buffers::Buffers(QObject *parent) : QObject(parent)
{

}

void Buffers::setTabPages(WidgetTabPages *tabPages)
{
    mTabPages = tabPages;
}

int Buffers::getSocketNumber() const
{
    return mSocketNumber;
}

void Buffers::appendTabPage(int socketptr)
{
    mTabPages->addTab(socketptr);
}

void Buffers::removeTabPage(int index)
{
    mTabPages->removeTab(index);
}

void Buffers::appentTabData(int socketptr, const QByteArray &data)
{
    mTabPages->appentData(socketptr, data);
}

QString Buffers::getServerKey() const
{
    return mServerKey;
}

void Buffers::setServerKey(const QString &serverKey)
{
    mServerKey = serverKey;
}

TcpServer *Buffers::getTcpServer(const QString &key) const
{
    if (!mMapServers.contains(key)) return nullptr;
    return mMapServers.value(key);
}

QMap<QString, TcpServer *> Buffers::getMapServers() const
{
    return mMapServers;
}

void Buffers::setMapServers(const QMap<QString, TcpServer *> &mapServers)
{
    mMapServers = mapServers;
}

void Buffers::insertServer(const QString &key, TcpServer *server)
{
    mMapServers.insert(key, server);
}

Buffers *Buffers::getInstance()
{
    if (nullptr == buffer)
    {
        buffer = new Buffers;
    }
    return buffer;
}

QStandardItemModel *Buffers::getModelItems() const
{
    return mModelSocketItems;
}

void Buffers::setModelItems(QStandardItemModel *modelItems)
{
    mModelSocketItems = modelItems;
}

void Buffers::removeItem(int ptr, const QString &ip, int port)
{
    QString address = ip + ":" + QString::number(port);
    QModelIndexList list = mModelSocketItems->match(mModelSocketItems->index(0, 0), Qt::DisplayRole, address, -1, Qt::MatchExactly | Qt::MatchRecursive);
    for (auto& index : list)
    {
        QStandardItem *item = mModelSocketItems->itemFromIndex(index);
        if (nullptr == item) continue;
        int socketptr = item->data(Qt::UserRole + 4).toUInt();
        if (ptr == socketptr)
        {
            mModelSocketItems->removeRow(item->row(), index.parent());
            break;
        }
    }
}

void Buffers::addClientItem(const QString &key, const int socketptr, const QString &address, const quint16 port)
{
    LOG_DEBUG("add client");
    QModelIndexList list = mModelSocketItems->match(mModelSocketItems->index(0, 0), Qt::DisplayRole, key, 1, Qt::MatchExactly | Qt::MatchRecursive);
    if (list.length() == 0) return;

    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/client.png"), QString("%1:%2").arg(address).arg(port));
    item->setData(Type_Tcp_Client, Qt::UserRole + 1);
    item->setData(address, Qt::UserRole + 2);
    item->setData(port, Qt::UserRole + 3);
    item->setData(socketptr, Qt::UserRole + 4);
    item->setData(mSocketNumber, Qt::UserRole + 5); // tab index
    item->setData(key, Qt::UserRole + 6); // server key

    QStandardItem *parentItem = mModelSocketItems->itemFromIndex(list.at(0));
    if (nullptr == parentItem) delete item;
    else
    {
        parentItem->appendRow(item);
        mSocketNumber++;
    }
}

void Buffers::addClientItem(const int socketptr, const QString &address, const quint16 port)
{
    LOG_DEBUG("add client");
    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/client.png"), QString("%1:%2").arg(address).arg(port));
    item->setData(Type_Tcp_Client, Qt::UserRole + 1);
    item->setData(address, Qt::UserRole + 2);
    item->setData(port, Qt::UserRole + 3);
    item->setData(socketptr, Qt::UserRole + 4);
    item->setData(mSocketNumber, Qt::UserRole + 5); // tab index
    item->setData("", Qt::UserRole + 6); // server key

    mSocketNumber++;
    mModelSocketItems->appendRow(item);
}

void Buffers::removeClientItem(const QString &key, const int socketptr, const QString &address, const quint16 port)
{
    Q_UNUSED(address);
    Q_UNUSED(port);
    LOG_DEBUG("remove client");
    QModelIndexList listServer = mModelSocketItems->match(mModelSocketItems->index(0, 0), Qt::DisplayRole, key, -1, Qt::MatchExactly | Qt::MatchRecursive);
    for (auto& server : listServer)
    {
        QString clientStr = QString("%1:%2").arg(address).arg(port);
        QModelIndexList listClient = mModelSocketItems->match(server, Qt::DisplayRole, clientStr, -1, Qt::MatchExactly | Qt::MatchRecursive);
        for (auto& client : listClient)
        {
            QStandardItem *item = mModelSocketItems->itemFromIndex(client);
            if (nullptr == item) continue;
            int ptr = item->data(Qt::UserRole + 4).toUInt();
            if (ptr == socketptr)
            {
                mModelSocketItems->removeRow(item->row(), client.parent());
                mSocketNumber--;
                break;
            }
        }
    }
}

void Buffers::addServerItem(const QString &address, const quint16 port)
{
    LOG_DEBUG("add server");
    QString key = QString("%1:%2").arg(address).arg(port);
    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/server.png"), key);
    item->setData(Type_Tcp_Server, Qt::UserRole + 1);
    item->setData(address, Qt::UserRole + 2);
    item->setData(port, Qt::UserRole + 3);
    item->setData(-1, Qt::UserRole + 4);
    item->setData(-1, Qt::UserRole + 5);
    item->setData("", Qt::UserRole + 6);

    mModelSocketItems->appendRow(item);
}

void Buffers::removeServerItem(const QString &address, const quint16 port)
{

}
