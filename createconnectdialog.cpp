#include "createconnectdialog.h"
#include "ui_createconnectdialog.h"
#include <QNetworkInterface>

createConnectDialog::createConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createConnectDialog)
{
    ui->setupUi(this);
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }

     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();

    ui->lineEdit->setText(strIpAddress);
}

createConnectDialog::~createConnectDialog()
{
    delete ui;
    if(listenSocket){
        listenSocket->close();
    }
}

QHostAddress createConnectDialog::getAddress()
{
    return QHostAddress(ui->lineEdit->text());
}
quint16 createConnectDialog::getPort()
{
    return ui->lineEdit_2->text().toUShort();
}

QTcpSocket *createConnectDialog::getReadWriteSocket()
{
    return readWriteSocket;
}
void createConnectDialog::acceptConnection()
{
    qDebug() << "accept";
    readWriteSocket = listenSocket->nextPendingConnection();
    listenSocket->close();
    return QDialog::accept();
}
void createConnectDialog::accept()
{
    //qDebug() << "accept";
    if(listenSocket == nullptr){
        listenSocket =new QTcpServer;
    }
    listenSocket->listen(QHostAddress::Any,getPort());
    ui->lineEdit->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    QObject::connect(listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}
