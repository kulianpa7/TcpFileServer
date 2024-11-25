﻿#include "tcpfileserver.h"
#define tr QStringLiteral
TcpFileServer::TcpFileServer(QWidget *parent)
    : QDialog(parent)
{
    totalBytes = 0;
    byteReceived = 0;
    fileNameSize = 0;
    serverProgressBar = new QProgressBar;
    serverStatusLabel = new QLabel(QStringLiteral("伺服器端就緒"));
    startButton = new QPushButton(QStringLiteral("接收"));
    quitButton = new QPushButton(QStringLiteral("退出"));
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton,QDialogButtonBox::RejectRole);

    ipLabel = new QLabel(tr("IP："));
    ipInput = new QLineEdit;
    ipInput->setPlaceholderText(tr("例如：127.0.0.1"));
    portLabel = new QLabel(tr("Port："));
    portInput = new QLineEdit;
    portInput->setPlaceholderText(tr("例如：16998"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(serverProgressBar);
    mainLayout->addWidget(serverStatusLabel);

    mainLayout->addWidget(ipLabel);
    mainLayout->addWidget(ipInput);
    mainLayout->addWidget(portLabel);
    mainLayout->addWidget(portInput);

    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(QStringLiteral("接收檔案"));
    connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(&tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    connect(&tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)),this,
            SLOT(displayError(QAbstractSocket::SocketError)));
 }

TcpFileServer::~TcpFileServer()
{

}
void TcpFileServer::start()
{
    // 取得使用者輸入的 IP 和 Port
    QString ip = ipInput->text();
    QString portStr = portInput->text();

    if (ip.isEmpty() || portStr.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("錯誤"), QStringLiteral("請輸入 IP 和 Port！"));
        startButton->setEnabled(true);
        return;
    }

    bool isPortValid;
    quint16 port = portStr.toUShort(&isPortValid);
    if (!isPortValid || port == 0) {
        QMessageBox::warning(this, QStringLiteral("錯誤"), QStringLiteral("Port 無效！請輸入有效的 Port（1-65535）。"));
        startButton->setEnabled(true);
        return;
    }

    // 重設狀態
    startButton->setEnabled(false);
    byteReceived = 0;
    fileNameSize = 0;

    // 監聽特定 IP 和 Port，並提供重試選項
    while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress(ip), port)) {
        QMessageBox::StandardButton ret = QMessageBox::critical(
            this,
            QStringLiteral("錯誤"),
            QStringLiteral("無法啟動伺服器：%1").arg(tcpServer.errorString()),
            QMessageBox::Retry | QMessageBox::Cancel);

        if (ret == QMessageBox::Cancel) {
            startButton->setEnabled(true);
            return;
        }
    }

    // 成功啟動伺服器
    serverStatusLabel->setText(QStringLiteral("伺服器正在監聽 %1:%2").arg(ip).arg(port));
}


void TcpFileServer::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();//取得已接受的客戶端連線
    connect(tcpServerConnection, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress()));//連接readyRead()訊號至updateServerProgress()槽函數
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));//連接error()訊號至displayError()槽函數
    serverStatusLabel->setText(QStringLiteral("接受連線"));
    tcpServer.close(); //暫停接受客戶端連線
}

void TcpFileServer::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_4_6);
    if(byteReceived <= sizeof(qint64)*2)
      {
         if((fileNameSize == 0) && (tcpServerConnection->bytesAvailable() >=
                  sizeof(qint64)*2))
          {
              in >> totalBytes >> fileNameSize;
              byteReceived += sizeof(qint64)*2;
          }
          if((fileNameSize != 0) && (tcpServerConnection->bytesAvailable() >=
                  fileNameSize)  )
          {
              in >> fileName;
              byteReceived += fileNameSize;
              localFile = new QFile(fileName);
              if(!localFile->open(QFile::WriteOnly))
              {
                  QMessageBox::warning(this, QStringLiteral("應用程式"),
                                    QStringLiteral("無法讀取檔案 %1：\n%2.").arg(fileName)
                                    .arg(localFile->errorString()));
                  return;
              }
          }else{
              return;
          }
       }

    if (byteReceived < totalBytes)
      {
          byteReceived += tcpServerConnection->bytesAvailable();
          inBlock = tcpServerConnection->readAll();
          localFile->write(inBlock);
          inBlock.resize(0);
      }

    serverProgressBar->setMaximum(totalBytes);
    serverProgressBar->setValue(byteReceived);
    qDebug() << byteReceived;
    serverStatusLabel->setText(QStringLiteral("已接收 %1 Bytes")
                               .arg(byteReceived));

    if (byteReceived == totalBytes)
      {
          tcpServerConnection->close();
          startButton->setEnabled(true);
          localFile->fileName();
          localFile->close();
          start();
      }



}

void TcpFileServer::displayError(QAbstractSocket::SocketError socketError)
{
    QObject * server = qobject_cast<QObject *>(sender());
    if (server == tcpServerConnection) qDebug() << "Hi I am QTcpSocket";
    if (server == &tcpServer) qDebug() << "Hi I am QTcpServer";
   // if (socketError == QTcpSocket::RemoteHostClosedError)
   //     return;

    QMessageBox::information(this, QStringLiteral("網絡錯誤"),
                             QStringLiteral("產生如下錯誤: %1.")
                             .arg(tcpServerConnection->errorString()));
    tcpServerConnection->close();
    serverProgressBar->reset();
    serverStatusLabel->setText(QStringLiteral("伺服器就緒"));
    startButton->setEnabled(true);

}






