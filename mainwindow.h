#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "piece.h"
#include "square.h"
#include "board.h"
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    board *chessboard;
    /*int tempx;
    int tempy;*/
    QVector<QLabel*> labels;

    bool isSelected;
    square* selectedSquare;

    int player[2];  //eg: player[0] = 1 means player0 plays blue.
    int currentPlayer;
    int record0[2];
    int record1[2]; //to assign side to each player
    QLabel* suggestLabel;
    QLabel* playerSide;
    bool host;

    void whoWin();   //-1:nobody, 0:player0, 1:player1

    QTimer *timer;
    QTime *time;
    QLCDNumber* timeLCD;

    QTcpSocket * readWriteSocket = nullptr;

    void action(int x, int y, bool random, int chosenSide, pieceType chosenType);

    int overTimeNum;

    void youWin();
    void youLose();

    void start();

    bool recvStart;
    bool sendStart;

    void init();

    int steps;


private slots:
    void mousePressEvent(QMouseEvent *e);

    int getRowIndex(QMouseEvent *e);    //return the row index, if nothing, return -1
    int getColumnIndex(QMouseEvent *e); //similar to above, but almost never return -1.

    void timerUpdate();

    void recvMessage();
    void sendMessage(int x, int y, int side, int type);

    void on_actioncreate_connection_triggered();

    void on_actionconnect_triggered();

    void on_actionStart_triggered();

    void on_actionAdmit_defeat_triggered();


    void on_actionDisconnect_triggered();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

};
#endif // MAINWINDOW_H
