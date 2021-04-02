#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>

#include "Dialog/dialognote.h"

#include "Public/defines.h"
#include "Log/logger.h"
#include "Buffer/buffers.h"
#include "Manager/servermanager.h"
#include "Manager/clientmanager.h"
#include "Public/threadpool.h"
#include "Dialog/dialogtcpserverargs.h"
#include "Dialog/dialognote.h"

#include <QDebug>
#include <QThread>

using namespace mtr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    setWindowTitle("木头人网络测试工具");

    qDebug() << "main " << QThread::currentThreadId();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();
    resize(rect.width() * 0.52, rect.height() * 0.6);

    ui->splitter->setStretchFactor(0, 2);
    ui->splitter->setStretchFactor(1, 9);

    // 关联所有信号与槽
    connect(ServerManager::Instance(), &ServerManager::sgl_server_operation, ui->widgetSocketView, &WidgetTreeView::slot_server_operation);
    connect(ClientManager::Instance(), &ClientManager::sgl_client_operation, ui->widgetSocketView, &WidgetTreeView::slot_client_operation);
    connect(ui->widgetSocketView, &WidgetTreeView::sgl_server_operation, ServerManager::Instance(), &ServerManager::slot_operation_server);

    connect(ServerManager::Instance(), &ServerManager::sgl_add_server_status, this, &MainWindow::slot_add_server_status, Qt::QueuedConnection);

    connect(ServerManager::Instance(), &ServerManager::sgl_client_operation, ui->widgetSocketView, &WidgetTreeView::slot_client_operation);

    connect(ui->actionAddTcpServer, &QAction::triggered, this, &MainWindow::slot_add_server);
    connect(ui->actionAddTcpClient, &QAction::triggered, this, &MainWindow::slot_add_client);
}

void MainWindow::slot_add_server_status(bool success)
{
    if (success) return;

    DialogNote note(this);
    note.hideButtonCancel();
    note.showMessage("新建服务失败");
}

void MainWindow::slot_add_server()
{
    DialogTcpServerArgs args(this);
    args.exec();
    if (args.result() == 0) return;
    ServerManager::Instance()->addServer(args.getAddress(), args.getPort().toUInt());
}

void MainWindow::slot_add_client()
{
//    DialogTcpClientArgs args(this);
//    args.exec();
//    if (args.result() == 0) return;
//    QString address = args.getAddress();
//    int port = args.getPort().toUInt();
//    bool status = false;//ServiceCenter::getService().addSocket(address, port);
//    QString key = QString("%1:%2").arg(address).arg(port);
//    if (!status)
//    {
//        DialogNote note(this);
//        note.hideButtonCancel();
//        note.showMessage("无法连接到服务器");
//    }
}
