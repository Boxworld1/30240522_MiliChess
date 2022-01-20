#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <vector>

class Chess;

class Chessboard{
    Chess* chess[5][12];
    bool movableFlag[5][12];
    constexpr static int postx[10] = {1, 1, 1, 1, 2, 2, 3, 3, 3, 3};
    constexpr static int posty[10] = {2, 4, 7, 9, 3, 8, 2, 4, 7, 9};

public:
    Chessboard();
    bool isPost(int, int);
    void render();
    Chess* get(int, int);
    int getMovableFlag(int, int);
    void changeMovableFlag(int, int, int);
    void changeChess(Chess*, int, int);
};


#endif // CHESSBOARD_H
