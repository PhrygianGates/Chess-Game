#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include "piece.h"
#include <vector>
using std::vector;

class square : public QObject
{
    Q_OBJECT

public:
    explicit square(int i, int j, int type, QObject *parent = nullptr);
    int type; //0:others, 1:camps, 2:railways
    int i;
    int j;
    int index;
    piece* p;
    QVector<square*> adjs;
    QLabel* label;
    int assignPiece(bool random, int chosenSide, pieceType chosenType);  //return blue0 or red1, -1 means false
    static vector<pieceType> blue;
    static vector<pieceType> red;
    bool isAssigned;

signals:

};

#endif // SQUARE_H
