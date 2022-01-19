#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QCursor>
#include <QMouseEvent>
#include "piece.h"
#include "square.h"
#include "board.h"
#include <iostream>
#include <createconnectdialog.h>
#include <connectdialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //test typeCompare
    /*pieceType t1 = zhadan, t2 = gongbin;
    int res = typeCompare(t1, t2);
    if (res == 0) qDebug() << "unknown";
    if (res == 1) qDebug() << "eat";
    if (res == 2) qDebug() << "eaten";
    if (res == 3) qDebug() << "both die";*/

    //testBoardCreate
    /*chessboard = new board(this);
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 5; j++) {
            std::cout << chessboard->squares[i][j]->type << " ";
        }
        std::cout << std::endl;
    }*/

    //testBoardConnect
    /*chessboard = new board(this);
    qDebug() << chessboard->isConnect(3,1,4,1);
    qDebug() << chessboard->isConnect(10,2,10,3);
    qDebug() << chessboard->isConnect(5,0,4,1);
    qDebug() << chessboard->isConnect(3,2,5,2);*/

    //testBoardConnectThroughClick
    /*chessboard = new board(this);
    tempy = tempx = -1;*/

    recvStart = false;
    sendStart = false;
    steps = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::whoWin()
{
    qDebug() << "blue movable:" << chessboard->isAnyMovable(0);
    qDebug() << "red movable:" << chessboard->isAnyMovable(1);
    if (chessboard->haveBlueJunqi == false || !chessboard->isAnyMovable(0)) {
        if (player[0] == 0) {
            if (host) {
                sendMessage(-1, -1, 0, 0);
                youLose();
            }
            else {
                sendMessage(-4, -4, 0, 0);
                youWin();
            }
        } else {
            if (host) {
                sendMessage(-4, -4, 0, 0);
                youWin();
            }
            else {
                sendMessage(-1, -1, 0, 0);
                youLose();
            }
        }
    }
    if (chessboard->haveRedJunqi == false || !chessboard->isAnyMovable(1)) {
        if (player[0] == 0) {
            if (host) {
                sendMessage(-4, -4, 0, 0);
                youWin();
            }
            else {
                sendMessage(-1, -1, 0, 0);
                youLose();
            }
        } else {
            if (host) {
                sendMessage(-1, -1, 0, 0);
                youLose();
            }
            else {
                sendMessage(-4, -4, 0, 0);
                youWin();
            }
        }
    }
    if (overTimeNum >= 3) {
        sendMessage(-1, -1, 0, 0);
        youLose();
    }
}

void MainWindow::action(int x, int y, bool random, int chosenSide, pieceType chosenType)
{
    if (x >= 0 && x < 12 && y >= 0 && y < 5) {
        //qDebug() << currentPlayer << player[0] << player[1];
        square* s = chessboard->squares[x][y];
        if (!isSelected) {
            if (!s->isAssigned) {
                int side = s->assignPiece(random, chosenSide, chosenType);
                if (random) {
                    steps++;
                    qDebug() << steps;
                }
                if (player[0] == -1) {  //have not assign side
                    if (currentPlayer == 0) {
                        record0[0] = record0[1];
                        record0[1] = side;
                        if (record0[0] == record0[1]) {
                            player[0] = record0[0];
                            player[1] = 1 - player[0];
                        }
                    } else {
                        record1[0] = record1[1];
                        record1[1] = side;
                        if (record1[0] == record1[1]) {
                            player[1] = record1[0];
                            player[0] = 1 - player[1];
                        }
                    }
                    if (player[0] == 0) {
                        /*player0->setText("Player 0: blue");
                        player1->setText("Player 1: red");*/
                        if (host) {
                            playerSide->setText("BLUE");
                            playerSide->setStyleSheet("QLabel{background-color:rgb(0,0,255);}");
                        } else {
                            playerSide->setText("RED");
                            playerSide->setStyleSheet("QLabel{background-color:rgb(255,0,0);}");
                        }
                    } else if (player[0] == 1) {
                        /*player0->setText("Player 0: red");
                        player1->setText("Player 1: blue");*/
                        if (!host) {
                            playerSide->setText("BLUE");
                            playerSide->setStyleSheet("QLabel{background-color:rgb(0,0,255);}");
                        } else {
                            playerSide->setText("RED");
                            playerSide->setStyleSheet("QLabel{background-color:rgb(255,0,0);}");
                        }
                    }
                }
                time->setHMS(0, 0, 20);
                currentPlayer = 1 - currentPlayer;
                //playerLabel->setText("Player" + QString::number(currentPlayer));
                if (host) {
                    if (currentPlayer == 0) {
                        suggestLabel->setText("Your Turn!");
                    } else {
                        suggestLabel->setText("Wait...");
                    }
                } else {
                    if (currentPlayer == 1) {
                        suggestLabel->setText("Your Turn!");
                    } else {
                        suggestLabel->setText("Wait...");
                    }
                }
            }
            else if (s->p != nullptr){
                if (s->p->side == player[currentPlayer]) {
                    selectedSquare = s;
                    isSelected = true;
                    selectedSquare->label->setStyleSheet("border-width: 2px; border-style: solid; border-color: rgb(0, 255, 0);");
                    //qDebug() << "select!";
                } else {
                    //qDebug() << "wrong side!";
                }
            }
        }
        else {
            //qDebug() << chessboard->isConnect(selectedSquare, s);
            if (selectedSquare->p != nullptr && s->p != nullptr) {
                //qDebug() << selectedSquare->p->compare(s->p);
            }
            int flag = chessboard->isMovable(selectedSquare, s);
            //qDebug() << "flag:" << flag;
            chessboard->move(selectedSquare, s);
            //qDebug() << "execute move";
            whoWin();
            //qDebug() << "execute whoWin";
            if (flag != 0) {
                if (random) {
                    steps++;
                    qDebug() << steps;
                }
                time->setHMS(0, 0, 20);
                currentPlayer = 1 - currentPlayer;
                //playerLabel->setText("Player" + QString::number(currentPlayer));
                if (host) {
                    if (currentPlayer == 0) {
                        suggestLabel->setText("Your Turn!");
                    } else {
                        suggestLabel->setText("Wait...");
                    }
                } else {
                    if (currentPlayer == 1) {
                        suggestLabel->setText("Your Turn!");
                    } else {
                        suggestLabel->setText("Wait...");
                    }
                }
            } else {
                if ((host && currentPlayer == 0) || (!host && currentPlayer == 1)) {
                    suggestLabel->setText("Wrong Move!");
                }
            }
            //qDebug() << "execute if else";
            if (selectedSquare != nullptr) selectedSquare->label->setStyleSheet("");
            selectedSquare = nullptr;
            isSelected = false;
            //qDebug() << "execute all";
        }
    }

}

void MainWindow::youWin()
{
    //QApplication::setQuitOnLastWindowClosed(true);
    timer->stop();
    QMessageBox::information(this, "Game over", "You win!");
    //qDebug() << "herewin";
    this->close();
}

void MainWindow::youLose()
{
    //QApplication::setQuitOnLastWindowClosed(true);
    timer->stop();
    QMessageBox::information(this, "Game over", "You lose!");
    //qDebug() << "herelose";
    this->close();

}

void MainWindow::start()
{
    //add labels
    {
        labels.push_back(ui->label_2);
        labels.push_back(ui->label_3);
        labels.push_back(ui->label_4);
        labels.push_back(ui->label_5);
        labels.push_back(ui->label_6);
        labels.push_back(ui->label_7);
        labels.push_back(ui->label_8);
        labels.push_back(ui->label_9);
        labels.push_back(ui->label_10);
        labels.push_back(ui->label_11);
        labels.push_back(ui->label_12);
        labels.push_back(ui->label_13);
        labels.push_back(ui->label_14);
        labels.push_back(ui->label_15);
        labels.push_back(ui->label_16);
        labels.push_back(ui->label_17);
        labels.push_back(ui->label_18);
        labels.push_back(ui->label_19);
        labels.push_back(ui->label_20);
        labels.push_back(ui->label_21);
        labels.push_back(ui->label_22);
        labels.push_back(ui->label_23);
        labels.push_back(ui->label_24);
        labels.push_back(ui->label_25);
        labels.push_back(ui->label_26);
        labels.push_back(ui->label_27);
        labels.push_back(ui->label_28);
        labels.push_back(ui->label_29);
        labels.push_back(ui->label_30);
        labels.push_back(ui->label_31);
        labels.push_back(ui->label_32);
        labels.push_back(ui->label_33);
        labels.push_back(ui->label_34);
        labels.push_back(ui->label_35);
        labels.push_back(ui->label_36);
        labels.push_back(ui->label_37);
        labels.push_back(ui->label_38);
        labels.push_back(ui->label_39);
        labels.push_back(ui->label_40);
        labels.push_back(ui->label_41);
        labels.push_back(ui->label_42);
        labels.push_back(ui->label_43);
        labels.push_back(ui->label_44);
        labels.push_back(ui->label_45);
        labels.push_back(ui->label_46);
        labels.push_back(ui->label_47);
        labels.push_back(ui->label_48);
        labels.push_back(ui->label_49);
        labels.push_back(ui->label_50);
        labels.push_back(ui->label_51);
        labels.push_back(ui->label_52);
        labels.push_back(ui->label_53);
        labels.push_back(ui->label_54);
        labels.push_back(ui->label_55);
        labels.push_back(ui->label_56);
        labels.push_back(ui->label_57);
        labels.push_back(ui->label_58);
        labels.push_back(ui->label_59);
        labels.push_back(ui->label_60);
        labels.push_back(ui->label_61);
    }
    for (int i = 0; i < 60; i++) {
        //labels[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        labels[i]->installEventFilter(this);
        labels[i]->setFixedSize(101, 40);
        labels[i]->setScaledContents(true);
    }
    chessboard = new board(labels, this);
    //qDebug() << chessboard->squares[5][4]->label->text();

    /*QSignalMapper *mapper = new QSignalMapper(this);
    for (int i = 0; i < 60; i++) {
        connect(labels[i], SIGNAL(clicked(bool)), mapper, SLOT(map()));
        mapper->setMapping(labels[i], i);
    }
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(mouseClick(int)));*/

    isSelected = false;
    selectedSquare = nullptr;

    player[0] = player[1] = -1;

    record0[0] = record0[1] = record1[0] = record1[1] = -1;
    suggestLabel = ui->label_62;
    suggestLabel->setText("");
    playerSide = ui->label_63;
    playerSide->setText("N/A");

    overTimeNum = 0;
    ui->label_64->setText("OverTime:" + QString::number(overTimeNum));

    timer = new QTimer(this);
    time = new QTime(0, 0, 20);
    timeLCD = ui->lcdNumber;
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timeLCD->setDigitCount(9);
    timeLCD->display(time->toString("hh:mm:ss"));


    timer->start(1000);

    currentPlayer = 0;
    if (host) {
        suggestLabel->setText("Your turn!");
    }
    else {
        suggestLabel->setText("Wait...");
    }

    steps = 0;
}

void MainWindow::init()
{
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 5; j++) {
            chessboard->squares[i][j]->label->clear();
            chessboard->squares[i][j] = nullptr;
        }
    }
    chessboard = nullptr;
    /*for (int i = 0; i < 60; i++) {
        labels.pop_back();
    }*/
    square::blue = vector<pieceType>{junqi, siling, junzhang, shizhang, shizhang, lvzhang, lvzhang, tuanzhang, tuanzhang,
                                   zhadan, zhadan, lianzhang, lianzhang, lianzhang, paizhang, paizhang, paizhang, gongbin,
                                   gongbin, gongbin, dilei, dilei, dilei, yingzhang, yingzhang};
    square::red = vector<pieceType>{junqi, siling, junzhang, shizhang, shizhang, lvzhang, lvzhang, tuanzhang, tuanzhang,
                                   zhadan, zhadan, lianzhang, lianzhang, lianzhang, paizhang, paizhang, paizhang, gongbin,
                                   gongbin, gongbin, dilei, dilei, dilei, yingzhang, yingzhang};

    isSelected = false;
    selectedSquare = nullptr;

    player[0] = player[1] = -1;

    record0[0] = record0[1] = record1[0] = record1[1] = -1;

    suggestLabel->setText("");

    playerSide->setText("N/A");
    playerSide->setStyleSheet("");

    overTimeNum = 0;
    ui->label_64->setText("OverTime:" + QString::number(overTimeNum));

    timer->stop();
    time = new QTime(0, 0, 20);
    timeLCD->setDigitCount(9);
    timeLCD->display(time->toString("hh:mm:ss"));

    recvStart = false;
    sendStart = false;

    currentPlayer = 0;

    steps = 0;
}



void MainWindow::timerUpdate()
{
    //qDebug() << "here";
    if (time->second() == 0) {
        if ((host && currentPlayer == 0) || (!host && currentPlayer == 1)) {
            overTimeNum++;
            ui->label_64->setText("OverTime:" + QString::number(overTimeNum));
            whoWin();
        }
        *time = time->addSecs(20);
        currentPlayer = 1 - currentPlayer;
        //playerLabel->setText("Player" + QString::number(currentPlayer));
        if (host) {
            if (currentPlayer == 0) {
                suggestLabel->setText("Your Turn!");
            } else {
                suggestLabel->setText("Wait...");
            }
        } else {
            if (currentPlayer == 1) {
                suggestLabel->setText("Your Turn!");
            } else {
                suggestLabel->setText("Wait...");
            }
        }
    }
    *time = time->addSecs(-1);
    QString strTime = time->toString("hh:mm:ss");
    timeLCD->display(strTime);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    //qDebug() << e->type();
    int x = getRowIndex(e), y = getColumnIndex(e);
    if (x < 0 || x >= 12 || y < 0 || y >= 12) return;
    //qDebug() << e->x() << e->y();
    //qDebug() << x << y;
    if ((host && currentPlayer == 0) || (!host && currentPlayer == 1)) {
        action(x, y, true, 0, gongbin);
        if (chessboard->squares[x][y]->p != nullptr) sendMessage(x, y, chessboard->squares[x][y]->p->side, (int)chessboard->squares[x][y]->p->type);
        else sendMessage(x, y, 0, 0);
    }
}

int MainWindow::getRowIndex(QMouseEvent *e)
{
    int y = e->y();
    if (y > 25 && y < 63) return 0;
    if (y < 107) return 1;
    if (y < 156) return 2;
    if (y < 199) return 3;
    if (y < 246) return 4;
    if (y < 291) return 5;
    if (y < 380) return -1;
    if (y < 422) return 6;
    if (y < 468) return 7;
    if (y < 515) return 8;
    if (y < 560) return 9;
    if (y < 608) return 10;
    if (y < 651) return 11;
    else return -1;
}

int MainWindow::getColumnIndex(QMouseEvent *e)
{
    int x = e->x();
    if (x > 0 && x < 102) return 0;
    if (x < 211) return 1;
    if (x < 317) return 2;
    if (x < 419) return 3;
    if (x < 529) return 4;
    else return -1;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    for (int i = 0; i < 60; i++) {
        if (obj == labels[i]) {
            if (ev->type() == QEvent::Enter) {
                labels[i]->setStyleSheet("border-width: 2px; border-style: solid; border-color: rgb(255, 0, 0);");
                return true;
            }
            if (ev->type() == QEvent::Leave && (selectedSquare == nullptr || labels[i] != selectedSquare->label)) {
                //qDebug() << "here";
                labels[i]->setStyleSheet("");
                return true;
            }
            return false;
        }
    }
    return eventFilter(obj, ev);
}


void MainWindow::recvMessage() {
    QByteArray info = readWriteSocket->readAll();
    //qDebug() << "receive:" << int(info[0]) - 4 << int(info[1]) - 4 << int(info[2]) - 4 << int(info[3]) - 4;
    int x = int(info[0]) - 8, y = int(info[1]) - 8;
    if (x == -1) youWin();
    else if (x == -2) {
        recvStart = true;
        if (sendStart) start();
    }
    else if (x == -3) {
        readWriteSocket->close();
        readWriteSocket = nullptr;
        QMessageBox::information(this, "Notice", "Disconnect!");
       if (sendStart && recvStart) init();
    }
    else if (x == -4) {
        youLose();
    }
    else {
        int side = int(info[2]) - 8;
        int t = int(info[3]) - 8;
        /*pieceType type;
        if (t == 0) type = junqi;
        else if (t == 1) type = siling;
        else if (t == 2) type = junzhang;
        else if (t == 3) type = shizhang;
        else if (t == 4) type = lvzhang;
        else if (t == 5) type = tuanzhang;
        else if (t == 6) type = yingzhang;
        else if (t == 7) type = lianzhang;
        else if (t == 8) type = paizhang;
        else if (t == 9) type = gongbin;
        else if (t == 10) type = dilei;
        else type = zhadan;*/
        pieceType type = static_cast<pieceType>(t);
        action(x, y, false, side, type);
    }
}

void MainWindow::sendMessage(int x, int y, int side, int type)
{
    //qDebug() << "send:" << x << y << side << type;
    QByteArray* message = new QByteArray;
    message->append(x + 8).append(y + 8).append(side + 8).append(type + 8);
    readWriteSocket->write(message->data());
    readWriteSocket->waitForBytesWritten();
}

void MainWindow::on_actioncreate_connection_triggered()
{
    createConnectDialog d(this);
    //qDebug() << "0";
    if (d.exec() == createConnectDialog::Accepted) {
        //qDebug() << "1";
        readWriteSocket = d.getReadWriteSocket();
        //qDebug() << "2";
        QObject::connect(readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
        host = true;
    }

}

void MainWindow::on_actionconnect_triggered()
{
    connectDialog d(this);
    if (d.exec() == connectDialog::Accepted) {
        readWriteSocket = d.getReadWriteSocket();
        QObject::connect(readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
        host = false;
    }
    //test send and receive
    /*QByteArray *array = new QByteArray;
    array->append(3).append(4);
    readWriteSocket->write(array->data());
    readWriteSocket->waitForBytesWritten();*/
}

void MainWindow::on_actionStart_triggered()
{
    if (sendStart && recvStart) return;
    sendStart = true;
    sendMessage(-2, -2, 0, 0);
    if (recvStart) start();
}

void MainWindow::on_actionAdmit_defeat_triggered()
{
    if (steps < 20) {
        suggestLabel->setText("20 steps!");
    } else {
        sendMessage(-1, -1, 0, 0);
        youLose();
    }
}



void MainWindow::on_actionDisconnect_triggered()
{
    sendMessage(-3, -3, 0, 0);
    readWriteSocket->close();
    readWriteSocket = nullptr;
    QMessageBox::information(this, "Notice", "Disconnect!");
    if (sendStart && recvStart) init();
}
