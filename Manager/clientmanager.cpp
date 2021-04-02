#include "clientmanager.h"

using namespace mtr;

ClientManager::ClientManager(QObject *parent) : QObject(parent)
{

}

ClientManager *ClientManager::Instance()
{
    static ClientManager manager;
    return &manager;
}
