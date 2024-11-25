#ifndef TCPFILESENDER_H
#define TCPFILESENDER_H

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>

class TcpFileSender : public QDialog
{
    Q_OBJECT

public:
    TcpFileSender(QWidget *parent = 0);
    ~TcpFileSender();

public slots:
    void start();                    // 開始傳送檔案
    void startTransfer();            // 開始傳輸資料
    void updateClientProgress(qint64 numBytes); // 更新進度條
    void openFile();                 // 開啟檔案

private:
    QProgressBar     *clientProgressBar; // 用於顯示傳輸進度
    QLabel           *clientStatusLabel; // 用於顯示狀態
    QLabel           *ipAddressLabel;    // 顯示 "IP 位址" 的標籤
    QLineEdit        *ipAddressInput;    // 用於輸入目標 IP 的文字框

    QLabel           *ipPortLabel;    // 顯示 "IP 位址" 的標籤
    QLineEdit        *ipPortInput;    // 用於輸入目標 IP 的文字框

    QPushButton      *startButton;       // "開始" 按鈕
    QPushButton      *quitButton;        // "退出" 按鈕
    QPushButton      *openButton;        // "開檔" 按鈕
    QDialogButtonBox *buttonBox;         // 按鈕框
    QTcpSocket       tcpClient;          // 客戶端 TCP 套接字

    qint64           totalBytes;         // 檔案總大小
    qint64           bytesWritten;       // 已寫入的大小
    qint64           bytesToWrite;       // 剩餘要寫入的大小
    qint64           loadSize;           // 每次傳輸的塊大小
    QString          fileName;           // 檔案名稱
    QFile            *localFile;         // 本地檔案指標
    QByteArray       outBlock;           // 傳輸資料的緩衝區
};

#endif // TCPFILESENDER_H
