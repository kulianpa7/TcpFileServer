#ifndef TCPFILESENDER_H
#define TCPFILESENDER_H

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>

class TcpFileSender : public QDialog
{
    Q_OBJECT
private:
    QProgressBar *clientProgressBar;
    QLabel *clientStatusLabel;
    QPushButton *startBtn;
    QPushButton *quitbtn;
    QPushButton *openbtn;
    QTcpSocket tcpClient;
    QDialogButtonBox *btnbox;
    QLabel *destAddrLabel;
    QLineEdit *editAddr;
    QLabel *destportLabel;
    QLineEdit *editport;

    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesToWrite;
    qint64 loadSize;
    QString fileName;
    QFile *localFile;
    QByteArray outBlock;
    QString hostadd;
    quint16 hostport;

public:
    TcpFileSender(QWidget *parent = nullptr);
    ~TcpFileSender();

public slots:
    void start();
    void startTransfer();
    void updateClientProgress(qint64 numBytes);
    void openFile();
    void updateDest();
};
#endif // TCPFILESENDER_H
