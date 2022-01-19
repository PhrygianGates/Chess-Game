#ifndef PIECE_H
#define PIECE_H

#include <QObject>


enum pieceType{junqi = 0, siling, junzhang, shizhang, lvzhang, tuanzhang, yingzhang, lianzhang, paizhang, gongbin, dilei, zhadan};
int typeCompare(pieceType t1, pieceType t2);

class piece : public QObject
{
    Q_OBJECT
public:
    explicit piece(pieceType type, int side, int i, int j, QObject *parent = nullptr);
    pieceType type;
    int side;
    int i;
    int j;
    int compare(piece* other);  //0:cannot eat, 1:can eat, 2:eaten, 3:both die

signals:

};

#endif // PIECE_H
