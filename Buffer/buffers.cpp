#include "buffers.h"
#include "Log/logger.h"
#include "Public/defines.h"

#include "Widget/widgettabpages.h"
#include "Widget/widgettreeview.h"

static Buffers* buffer = nullptr;

Buffers::Buffers(QObject *parent) : QObject(parent)
{

}

void Buffers::setTabPages(WidgetTabPages *tabPages)
{
    mTabPages = tabPages;
}

void Buffers::setTreeView(WidgetTreeView *treeview)
{
    mTreeView = treeview;
}

void Buffers::setItemExpand(const QStandardItem *item)
{
    if (nullptr == item) return;
    if (nullptr == mTreeView) return;
    mTreeView->setItemExpand(item);
}

void Buffers::setCurrentIndex(const QModelIndex& index)
{
    if (nullptr == mTreeView) return;
    mTreeView->setCurrentIndex(index);
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

Buffers *Buffers::getInstance()
{
    if (nullptr == buffer)
    {
        buffer = new Buffers;
    }
    return buffer;
}
