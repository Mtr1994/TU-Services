#ifndef WIDGETTABITEMS_H
#define WIDGETTABITEMS_H

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QMap>

#include "Widget/widgettabcontent.h"

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
    void addTab(int socketptr);
    void removeTab(int socketptr);
    void setCurrentIndex(int index);

    void appentData(int socketptr, const QByteArray& data);

public slots:
    void slot_current_index_change(int socketptr);
    void slot_new_client_coming(const int ptr, const QString & address, const quint16 port, QStandardItem *parentitem);

private:
    Ui::WidgetTabPages *ui;

    QMap<int ,WidgetTabContent*> mMapContent;
};

#endif // WIDGETTABITEMS_H
