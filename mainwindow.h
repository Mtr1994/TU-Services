#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Public/defines.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

public slots:
    void slot_add_server_status(bool success);
    void slot_client_operation_(int operation, const ClientInfo& info);

private slots:
    void slot_add_server();
    void slot_add_client();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
