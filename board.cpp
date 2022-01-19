#include "board.h"
#include "square.h"
#include "piece.h"
#include <algorithm>

board::board(QVector<QLabel*>& labels, QObject *parent) : QObject(parent)
{
    squares = new square**[12];
    for (int i = 0; i < 12; i++) {
        squares[i] = new square*[5];
    }
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 5; j++) {
            squares[i][j] = nullptr;
        }
    }
    for (int i = 1; i < 11; i++) {
        squares[i][0] = new square(i, 0, 2, this);
        squares[i][4] = new square(i, 4, 2, this);
    }
    for (int j = 1; j < 4; j++) {
        squares[1][j] = new square(1, j, 2, this);
        squares[5][j] = new square(5, j, 2, this);
        squares[6][j] = new square(6, j, 2, this);
        squares[10][j] = new square(10, j, 2, this);
    }
    squares[2][1] = new square(2, 1, 1, this);
    squares[2][3] = new square(2, 3, 1, this);
    squares[4][1] = new square(4, 1, 1, this);
    squares[4][3] = new square(4, 3, 1, this);
    squares[7][1] = new square(7, 1, 1, this);
    squares[7][3] = new square(7, 3, 1, this);
    squares[9][1] = new square(9, 1, 1, this);
    squares[9][3] = new square(9, 3, 1, this);
    squares[3][2] = new square(3, 2, 1, this);
    squares[8][2] = new square(8, 2, 1, this);
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 5; j++) {
            if (squares[i][j] == nullptr) {
                squares[i][j] = new square(i, j, 0, this);
            }
            //add label to each square
            squares[i][j]->label = labels[squares[i][j]->index];
        }
    }

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 5; j++) {
            if (squares[i][j]->type == 1) {
                squares[i][j]->isAssigned = true;
            }
            else {
                squares[i][j]->label->setPixmap(QPixmap("C:/Users/xzc/Desktop/Junqi/res/png/unknown_place.png"));
            }
        }
    }

    //connect the graph
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 4; j++) {
            connect(squares[i][j], squares[i][j + 1]);
        }
    }
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 5; i++) {
            connect(squares[i][j], squares[i + 1][j]);
        }
        for (int i = 6; i < 11; i++) {
            connect(squares[i][j], squares[i + 1][j]);
        }
    }
    for (int j = 0; j < 5; j += 2) {
        connect(squares[5][j], squares[6][j]);
    }
    for (int j = 1; j <= 3; j += 2) {
        for (int i = 2; i <= 4; i += 2) {
            connect(squares[i][j], squares[i - 1][j - 1]);
            connect(squares[i][j], squares[i - 1][j + 1]);
            connect(squares[i][j], squares[i + 1][j - 1]);
            connect(squares[i][j], squares[i + 1][j + 1]);
        }
        for (int i = 7; i <= 9; i += 2) {
            connect(squares[i][j], squares[i - 1][j - 1]);
            connect(squares[i][j], squares[i - 1][j + 1]);
            connect(squares[i][j], squares[i + 1][j - 1]);
            connect(squares[i][j], squares[i + 1][j + 1]);
        }
    }

    haveRedJunqi = haveBlueJunqi = true;
    haveRedDilei = haveBlueDilei = 3;
}

int board::isMovable(square *s1, square *s2)
{
    if (s1 == s2 || s1->p == nullptr || s1->p->type == junqi || s1->p->type == dilei) return 0;
    if (s1->p->side == 0 && s2->p != nullptr && s2->p->type == junqi && haveRedDilei > 0) return 0;
    if (s1->p->side == 1 && s2->p != nullptr && s2->p->type == junqi && haveBlueDilei > 0) return 0;
    if (s1->type != 2 || s2->type != 2) {    //s1 or s2 is not railway
        if (!isConnect(s1, s2)) {    //not in adjs
            return 0;
        }
        if (s2->p == nullptr) {
            if (s2->isAssigned) {
                return 4;
            }
            return 0;
        }
        if (s2->type == 1) {    //s2 is a camp
            return 0;
        }
        return s1->p->compare(s2->p);
    }
    //s1 and s2 are railways
    if (s1->p->type != gongbin) {
        if (s1->i == s2->i) {
            int min_j = s1->j < s2->j ? s1->j : s2->j;
            int max_j = s1->j > s2->j ? s1->j : s2->j;
            for (int j = min_j + 1; j < max_j; j++) {
                if (squares[s1->i][j]->type != 2 || squares[s1->i][j]->p != nullptr || !squares[s1->i][j]->isAssigned) {
                    return 0;
                }
            }
            if (s2->p == nullptr) {
                if (s2->isAssigned) {
                    return 4;
                }
                return 0;
            }
            return s1->p->compare(s2->p);
        }
        if (s1->j == s2->j) {
            int min_i = s1->i < s2->i ? s1->i : s2->i;
            int max_i = s1->i > s2->i ? s1->i : s2->i;
            if (min_i == 5 && max_i == 6 && (s1->j == 1 || s1->j == 3)) {
                return 0;
            }
            for (int i = min_i + 1; i < max_i; i++) {
                if (squares[i][s1->j]->type != 2 || squares[i][s1->j]->p != nullptr || !squares[i][s1->j]->isAssigned) {
                    return 0;
                }
            }
            if (s2->p == nullptr) {
                if (s2->isAssigned) {
                    return 4;
                }
                return 0;
            }
            return s1->p->compare(s2->p);
        }
        return 0;
    }
    //s1->p->type == gongbin
    vector<square*> visit;
    if (dfs(s1, s2, false, visit)) {
        if (s2->p == nullptr) {
            if (s2->isAssigned) {
                return 4;
            }
            return 0;
        }
        return s1->p->compare(s2->p);
    }

    return 0;
}

void board::connect(square *s1, square *s2)
{
    if (isConnect(s1, s2)) return;
    s1->adjs.push_back(s2);
    s2->adjs.push_back(s1);
}

bool board::dfs(square *s1, square *s2, bool flag, vector<square*>& visit)
{
    //qDebug() << "flag:" << flag;
    if (s1 == s2) {
        flag = true;
    }
    else {
        visit.push_back(s1);
        foreach(square* n, s1->adjs) {
            if((n->type == 2 && n->p == nullptr && n->isAssigned
                    && std::find(visit.begin(), visit.end(), n) == visit.end()) || n == s2) {
                flag = dfs(n, s2, flag, visit);
                if (flag) break;
            }
        }
    }
    return flag;
}

bool board::isConnect(square *s1, square *s2)
{
    if (s1->adjs.size() <= s2->adjs.size()) {
        return std::find(s1->adjs.begin(), s1->adjs.end(), s2) != s1->adjs.end();
    }
    return std::find(s2->adjs.begin(), s2->adjs.end(), s1) != s2->adjs.end();
}

void board::move(square *s1, square *s2)
{
    int res = isMovable(s1, s2);
    if (res == 0) {
        return;
    }
    if (res == 1) {
        if (s2->p->type == junqi) {
            if (s2->p->side == 0) haveBlueJunqi = false;
            else if (s2->p->side == 1) haveRedJunqi = false;
        }
        if (s2->p->type == dilei) {
            if (s2->p->side == 0) haveBlueDilei--;
            else if (s2->p->side == 1) haveRedDilei--;
        }
        s2->p = s1->p;
        s2->label->setPixmap(*s1->label->pixmap());
        s1->p = nullptr;
        s1->label->clear();
        return;
    }
    if (res == 2) {
        s1->p = nullptr;
        s1->label->clear();
        return;
    }
    if (res == 3) {
        if (s2->p->type == junqi) {
            if (s2->p->side == 0) haveBlueJunqi = false;
            else if (s2->p->side == 1) haveRedJunqi = false;
        }
        if (s2->p->type == dilei) {
            if (s2->p->side == 0) haveBlueDilei--;
            else if (s2->p->side == 1) haveRedDilei--;
        }
        s1->p = nullptr;
        s1->label->clear();
        s2->p = nullptr;
        s2->label->clear();
        return;
    }
    if (res == 4) {
        s2->p = s1->p;
        s2->label->setPixmap(*s1->label->pixmap());
        s1->p = nullptr;
        s1->label->clear();
        return;
    }
}

bool board::isMovable(square *s1, int side)
{
    if (!s1->isAssigned) return true;
    if (s1->p == nullptr) return false;
    if (s1->p->side != side) return false;
    int x = s1->i, y = s1->j;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if ((i == x && j == y) || i < 0 || i >= 12 || j < 0 || j >= 5) continue;
            if (s1->p->type == siling) {
                if (side == 0) qDebug() << "blue" << x << y << "to" << i << j << isMovable(s1, squares[i][j]);
                else qDebug() << "red" << x << y << "to" << i << j << isMovable(s1, squares[i][j]);
            }
            if (isMovable(s1, squares[i][j]) != 0) {
                return true;
            }
        }
    }
    return false;
}

bool board::isAnyMovable(int side)
{
    if (side != 0 && side != 1) return true;
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 5; j++) {
            if (isMovable(squares[i][j], side)) return true;
        }
    }
    return false;
}

bool board::isConnect(int x1, int y1, int x2, int y2)
{
    return isConnect(squares[x1][y1], squares[x2][y2]);
}
