#include <QApplication>
#include <QTabWidget>
#include "tcpfileserver.h"
#include "tcpfilesender.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTabWidget *tab1;
    tab1=new QTabWidget;
    TcpFileServer *w1;
    w1 = new TcpFileServer;
    TcpFileSender *w2;
    w2 = new TcpFileSender;
    tab1->addTab(w1,"server");
    tab1->addTab(w2,"client");
    tab1->show();
    return a.exec();
}
