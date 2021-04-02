#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include "Public/defines.h"

namespace mtr {

class ClientManager : public QObject
{
    Q_OBJECT
private:
    explicit ClientManager(QObject *parent = nullptr);

public:
    static ClientManager* Instance();

signals:
    void sgl_client_operation(int operation, const ClientInfo& info);

};

}

#endif // CLIENTMANAGER_H
