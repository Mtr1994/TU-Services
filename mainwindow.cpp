#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>

#include "Dialog/dialognote.h"

#include "Public/defines.h"
#include "Log/logger.h"
#include "Manager/servermanager.h"
#include "Manager/clientmanager.h"
#include "Public/threadpool.h"
#include "Dialog/dialogtcpserverargs.h"
#include "Dialog/dialogtcpclientargs.h"
#include "Dialog/dialognote.h"
#include "Dialog/dialogabout.h"

#include <QDebug>
#include <QThread>
#include <iostream>

using namespace mtr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    setWindowTitle("木头人网络测试工具");
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
    connect(ClientManager::Instance(), &ClientManager::sgl_client_operation, ui->tabPages, &WidgetTabPages::slot_client_operation);
    connect(ClientManager::Instance(), &ClientManager::sgl_client_operation, this, &MainWindow::slot_client_operation_);

    connect(ui->widgetSocketView, &WidgetTreeView::sgl_server_operation, ServerManager::Instance(), &ServerManager::slot_operation_server);
    connect(ui->widgetSocketView, &WidgetTreeView::sgl_client_operation, ServerManager::Instance(), &ServerManager::slot_operation_client);
    connect(ui->widgetSocketView, &WidgetTreeView::sgl_client_operation, ui->tabPages, &WidgetTabPages::slot_client_operation);
    connect(ui->widgetSocketView, &WidgetTreeView::sgl_client_operation, ClientManager::Instance(), &ClientManager::slot_operation_client);

    connect(ui->tabPages, &WidgetTabPages::sgl_client_operation, ServerManager::Instance(), &ServerManager::slot_operation_client);
    connect(ui->tabPages, &WidgetTabPages::sgl_client_operation, ClientManager::Instance(), &ClientManager::slot_operation_client);

    connect(ServerManager::Instance(), &ServerManager::sgl_socket_write, ui->tabPages, &WidgetTabPages::slot_socket_write);

    connect(ServerManager::Instance(), &ServerManager::sgl_add_server_status, this, &MainWindow::slot_add_server_status, Qt::QueuedConnection);

    ///
    /// \brief connect 有新的客户端连接了，通知树状列表更新
    ///
    connect(ServerManager::Instance(), &ServerManager::sgl_client_operation, ui->widgetSocketView, &WidgetTreeView::slot_client_operation);

    ///
    /// \brief connect 有新的客户端连接了，通知 Tab 页新增数据
    ///
    connect(ServerManager::Instance(), &ServerManager::sgl_client_operation, ui->tabPages, &WidgetTabPages::slot_client_operation);

    connect(ui->actionAddTcpServer, &QAction::triggered, this, &MainWindow::slot_add_server);
    connect(ui->actionAddTcpClient, &QAction::triggered, this, &MainWindow::slot_add_client);

    connect(ui->actionVersion, &QAction::triggered, [this]{
        DialogAbout about(this);
        about.exec();
    });
}

void MainWindow::slot_add_server_status(bool success)
{
    if (success) return;

    DialogNote note(this);
    note.hideButtonCancel();
    note.showMessage("新建服务失败");
}

// 主界面处理客户端连接异常的情况（如果数据发送异常等错误，在对应页面提示）
void MainWindow::slot_client_operation_(int operation, const ClientInfo &info)
{
    if (operation != Client_Error) return;

    ui->statusbar->showMessage("连接服务 " + info.serverkey + " 发生错误：" + info.error);
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
    DialogTcpClientArgs args(this);
    args.exec();

    if (args.result() == 0) return;

    ui->statusbar->showMessage("开始连接服务 " + QString("%1:%2").arg(args.getAddress()).arg(args.getPort()));
    ClientManager::Instance()->connectServer(args.getAddress(), args.getPort().toUInt());
}
