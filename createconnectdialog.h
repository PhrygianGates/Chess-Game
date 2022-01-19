#ifndef CREATECONNECTDIALOG_H
#define CREATECONNECTDIALOG_H

#include <QDialog>
#include <QtNetwork>

namespace Ui {
class createConnectDialog;
}

class createConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit createConnectDialog(QWidget *parent = nullptr);
    ~createConnectDialog();

    QHostAddress getAddress();
    quint16 getPort();
    QTcpSocket * getReadWriteSocket();

public slots:
    void acceptConnection();

private:
    Ui::createConnectDialog *ui;
    QTcpServer * listenSocket = nullptr;
    QTcpSocket * readWriteSocket = nullptr;

    void accept();

    void ignore();
};

#endif // CREATECONNECTDIALOG_H
