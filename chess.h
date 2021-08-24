#ifndef CHESS_H
#define CHESS_H
#include "chessboard.h"

class Chess{
    int type;
    int status;
    int color; //red = 1, blue = 2
    int x, y;
    bool highlighted;
    Chessboard* parent;
public:
    Chess(Chessboard*, int, int, int, int);
    int getColor();
    int getStatus();
    int getType();
    int getPosX();
    int getPosY();
    bool isHighlighted();
    void changeHighlight(bool);
    void changePos(int, int);
    void changeStatus();
};

#endif // CHESS_H
