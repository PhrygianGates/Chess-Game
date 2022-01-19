#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <regex>
#include <QMessageBox>

connectDialog::connectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectDialog)
{
    ui->setupUi(this);
}

connectDialog::~connectDialog()
{
    delete ui;
    if(!hasConnected){
        delete readWriteSocket;
    }
}

QHostAddress connectDialog::getAddress()
{
    return QHostAddress(ui->lineEdit->text());
}

quint16 connectDialog::getPort()
{
    return ui->lineEdit_2->text().toUShort();
}

QTcpSocket *connectDialog::getReadWriteSocket()
{
    return readWriteSocket;
}

bool connectDialog::ipOk()
{
    QString ip = ui->lineEdit->text();
    std::regex r("[0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+");
    std::string s = ip.toStdString();
    if(std::regex_match(s, r)){
        return true;
    }
    else{
        return false;
    }
}


void connectDialog::acceptConnection(){
    hasConnected = true;
    return QDialog::accept();
}

void connectDialog::accept()
{
    //int a = 0;
    if(readWriteSocket == nullptr){
        if(!ipOk()){
            QMessageBox::warning(this, "error", "Your input is wrong!");
            return;
        }
        ui->lineEdit->setVisible(false);
        ui->lineEdit_2->setVisible(false);
        ui->label->setVisible(false);
        ui->label_2->setVisible(false);
        this->readWriteSocket = new QTcpSocket;
        QObject::connect(readWriteSocket,SIGNAL(connected()),this,SLOT(acceptConnection()));
        this->readWriteSocket->connectToHost(getAddress(),getPort());
    }
}
