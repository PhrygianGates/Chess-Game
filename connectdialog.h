#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QtNetwork>

namespace Ui {
class connectDialog;
}

class connectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit connectDialog(QWidget *parent = nullptr);
    ~connectDialog();

    QHostAddress getAddress();
    quint16 getPort();
    QTcpSocket * getReadWriteSocket();

public slots:

    void acceptConnection();

private:
    Ui::connectDialog *ui;
    bool hasConnected = false;

    QTcpSocket * readWriteSocket = nullptr;

    bool ipOk();

    void accept();
};

#endif // CONNECTDIALOG_H
