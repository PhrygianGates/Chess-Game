#include "piece.h"

piece::piece(pieceType type, int side, int i, int j, QObject *parent) : QObject(parent)
{
    this->type = type;
    this->side = side;
    this->i = i;
    this->j = j;
}


int piece::compare(piece *other)    //0:unknown(should not appear, can not move), 1:can eat, 2:eaten, 3:both die,
{
    if (other->side == side) {
        return 0;
    }

    pieceType t1 = type, t2 = other->type;
    return typeCompare(t1, t2);
}

int typeCompare(pieceType t1, pieceType t2) {   //not symmetrical, t1 is positive while t2 is negative
    if (t1 == junqi || t1 == dilei) {
        return 0;
    }
    if (t1 == zhadan) {
        return 3;
    }
    if (t2 == junqi) {
        return 1;
    }
    if (t2 == zhadan) {
        return 3;
    }
    if (t2 == dilei) {
        if (t1 == gongbin) return 1;
        else return 0;
    }
    if (t1 == t2) return 3;
    if (t1 < t2) return 1;

    else return 0;
}
