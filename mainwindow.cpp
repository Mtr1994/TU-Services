#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>

#include "Server/tcpserver.h"
#include "Dialog/dialogtcpserverargs.h"
#include "Dialog/dialogtcpclientargs.h"
#include "Dialog/dialognote.h"

#include "Public/encoding.h"
#include "Public/defines.h"
#include "Log/logger.h"
#include "Server/servicecenter.h"
#include "Buffer/buffers.h"

extern ServiceCenter* glService;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();

    resize(rect.width() * 0.52, rect.height() * 0.6);
    setWindowTitle("土豆网络测试工具");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->treeView->init(ui->tabPages);
    connect(ui->treeView, &WidgetTreeView::sgl_current_index_change, ui->tabPages, &WidgetTabPages::slot_current_index_change);
    connect(ui->actionAddTcpServer, &QAction::triggered, [this](){
        DialogTcpServerArgs args(this);
        args.exec();
        if (args.result() == 0) return;

        bool status = glService->addServer(args.getAddress(), args.getPort().toUInt());
        if (status) Buffers::getInstance()->addServerItem(args.getAddress(), args.getPort().toUInt());
        else
        {
            DialogNote note(this);
            note.hideButtonCancel();
            note.showMessage("监听失败");
        }
    });

    connect(ui->actionAddTcpClient, &QAction::triggered, [this](){
        DialogTcpClientArgs args(this);
        args.exec();
        if (args.result() == 0) return;
        int count = ui->tabPages->getTabCount();
       // ui->treeView->addItem(count, Type_Tcp_Client, args.getAddress(), args.getPort());
    });

    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 7);


    Buffers::getInstance()->setTabPages(ui->tabPages);
}
