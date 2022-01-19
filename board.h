#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include "square.h"
#include "piece.h"

class board : public QObject
{
    Q_OBJECT
public:
    explicit board(QVector<QLabel*>& labels, QObject *parent = nullptr);
    square*** squares;
    int isMovable(square* s1, square* s2);     //check if able to move from s1 to s2,
                                               //0:can't move, 1:move and eat, 2:move and eaten, 3:both die, 4:just move
    bool isConnect(int x1, int y1, int x2, int y2);
    bool isConnect(square* s1, square* s2);
    //will there be memory leak?

    void move(square* s1, square* s2);

    bool haveBlueJunqi;
    bool haveRedJunqi;
    int haveBlueDilei;
    int haveRedDilei;

    bool isMovable(square* s1, int side);
    bool isAnyMovable(int side);

private:
    void connect(square* s1, square* s2);
    bool dfs(square* s1, square* s2, bool flag, vector<square*>& visit);

signals:

};

#endif // BOARD_H
