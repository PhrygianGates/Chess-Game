#include "square.h"
#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include "piece.h"
#include <algorithm>
#include <random>
#include <time.h>

vector<pieceType> square::blue{junqi, siling, junzhang, shizhang, shizhang, lvzhang, lvzhang, tuanzhang, tuanzhang,
                               zhadan, zhadan, lianzhang, lianzhang, lianzhang, paizhang, paizhang, paizhang, gongbin,
                               gongbin, gongbin, dilei, dilei, dilei, yingzhang, yingzhang};
vector<pieceType> square::red{junqi, siling, junzhang, shizhang, shizhang, lvzhang, lvzhang, tuanzhang, tuanzhang,
                               zhadan, zhadan, lianzhang, lianzhang, lianzhang, paizhang, paizhang, paizhang, gongbin,
                               gongbin, gongbin, dilei, dilei, dilei, yingzhang, yingzhang};


square::square(int i, int j, int type, QObject *parent) : QObject(parent)
{
    this->i = i;
    this->j = j;
    this->type= type;
    this->p = nullptr;
    adjs = QVector<square*>();
    index = i * 5 + j;
    label = nullptr;
    isAssigned = false;
}

int square::assignPiece(bool random, int chosenSide, pieceType chosenType)
{
    /*p = new piece(gongbin, 0, i, j, this);
    label->setScaledContents(true);
    label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/gongbin_blue.png"));*/
    //qDebug() << "blueSize:" << blue.size() << "redSize:" << red.size();
    int side;
    pieceType type;
    if (random) {
        srand(time(NULL));
        side = rand() % 2;  //0:blue, 1:red
        //qDebug() << "side: " << side;
        if (side == 0) {
            if (!blue.empty()) {
                std::random_shuffle(blue.begin(), blue.end());
                type = blue.back();
                blue.pop_back();
            }
            else {
                side = 1;
                std::random_shuffle(red.begin(), red.end());
                type = red.back();
                red.pop_back();
            }
        } else {
            if (!red.empty()) {
                std::random_shuffle(red.begin(), red.end());
                type = red.back();
                red.pop_back();
            }
            else {
                side = 0;
                std::random_shuffle(blue.begin(), blue.end());
                type = blue.back();
                blue.pop_back();
            }
        }
    } else {
        side = chosenSide;
        type = chosenType;
        if (side == 0) {
            for (auto itr = blue.begin(); itr != blue.end(); itr++) {
                if (*itr == type) {
                    blue.erase(itr);
                    break;
                }
            }
        } else {
            for (auto itr = red.begin(); itr != red.end(); itr++) {
                if (*itr == type) {
                    red.erase(itr);
                    break;
                }
            }
        }
    }

    p = new piece(type, side, i, j, this);
    //label->setScaledContents(true);
    if (side == 0) {
        switch (type) {
        case junqi:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/junqi_blue.png"));
            break;
        case siling:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/siling_blue.png"));
            break;
        case junzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/junzhang_blue.png"));
            break;
        case shizhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/shizhang_blue.png"));
            break;
        case lvzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/lvzhang_blue.png"));
            break;
        case tuanzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/tuanzhang_blue.png"));
            break;
        case zhadan:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/zhadan_blue.png"));
            break;
        case lianzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/lianzhang_blue.png"));
            break;
        case paizhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/paizhang_blue.png"));
            break;
        case gongbin:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/gongbin_blue.png"));
            break;
        case dilei:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/dilei_blue.png"));
            break;
        case yingzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/yingzhang_blue.png"));
            break;
        }
    }
    else {
        switch (type) {
        case junqi:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/junqi_red.png"));
            break;
        case siling:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/siling_red.png"));
            break;
        case junzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/junzhang_red.png"));
            break;
        case shizhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/shizhang_red.png"));
            break;
        case lvzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/lvzhang_red.png"));
            break;
        case tuanzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/tuanzhang_red.png"));
            break;
        case zhadan:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/zhadan_red.png"));
            break;
        case lianzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/lianzhang_red.png"));
            break;
        case paizhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/paizhang_red.png"));
            break;
        case gongbin:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/gongbin_red.png"));
            break;
        case dilei:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/dilei_red.png"));
            break;
        case yingzhang:
            label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/yingzhang_red.png"));
            break;
        }
    }
    isAssigned = true;
    return side;
}

