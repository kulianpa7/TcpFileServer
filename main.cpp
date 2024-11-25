#include "tcpfileserver.h"
#include "tcpfilesender.h"
#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 建立主窗口
    QMainWindow mainWindow;
    QTabWidget *tabWidget = new QTabWidget;

    // 建立 Client 分頁並嵌入 TcpFileSender
    TcpFileSender *clientPageWidget = new TcpFileSender;
    QWidget *clientPage = new QWidget;
    QVBoxLayout *clientLayout = new QVBoxLayout(clientPage);
    clientLayout->addWidget(clientPageWidget);
    clientPage->setLayout(clientLayout);

    tabWidget->addTab(clientPage, "Client");

    // 建立 Server 分頁並嵌入 TcpFileServer
    TcpFileServer *serverPageWidget = new TcpFileServer;
    QWidget *serverPage = new QWidget;
    QVBoxLayout *serverLayout = new QVBoxLayout(serverPage);
    serverLayout->addWidget(serverPageWidget);
    serverPage->setLayout(serverLayout);

    // 將 Server 分頁加入 QTabWidget
    tabWidget->addTab(serverPage, "Server");

    // 設定 QTabWidget 為主窗口中心的內容
    mainWindow.setCentralWidget(tabWidget);
    mainWindow.resize(800, 600);  // 設定窗口大小
    mainWindow.show();

    return a.exec();
}
