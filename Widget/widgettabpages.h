#ifndef WIDGETTABITEMS_H
#define WIDGETTABITEMS_H

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QMap>

#include "Widget/widgettabcontent.h"
#include "Public/defines.h"

namespace Ui {
class WidgetTabPages;
}

class WidgetTabPages : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTabPages(QWidget *parent = nullptr);
    ~WidgetTabPages();

    void init();

    int getTabCount();

public:
    void addTab(const QString& key, int socketptr);
    void removeTab(int socketptr);
    void setCurrentIndex(int index);

    void appentData(int socketptr, const QByteArray& data);

signals:
    void sgl_client_operation(int operation, const ClientInfo& info);

public slots:
    void slot_current_index_change(int socketptr);
    void slot_client_operation(int operation, const ClientInfo &info);
    void slot_socket_write(uint32_t socketdescriptor, int length);


private:
    Ui::WidgetTabPages *ui;

    QMap<int ,WidgetTabContent*> mMapContent;
};

#endif // WIDGETTABITEMS_H
