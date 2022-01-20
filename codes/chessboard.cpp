#include "chessboard.h"
#include "chess.h"
#include <random>
#include <algorithm>
#include <iterator>

bool Chessboard::isPost(int x, int y) {
    for (int k = 0; k < 10; k++)
        if (x == postx[k] && y == posty[k]) return true;
    return false;
}

Chessboard::Chessboard() {

    const int num[13] = {0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 2, 1};

    std::vector<std::pair<int, int> > vec;

    for (int color = 1; color <= 2; color++)
        for (int type = 1; type < 13; type++)
            for (int k = 0; k < num[type]; k++) vec.push_back(std::make_pair(type, color));

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vec.begin(), vec.end(), g);

    int cnt = 0;
    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 12; y++) {
            if (isPost(x, y)) chess[x][y] = nullptr;
            else chess[x][y] = new Chess(x, y, vec[cnt].first, vec[cnt++].second, 0);
        }

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 12; j++) {
            changeMovableFlag(i, j, false);
        }
}

Chess* Chessboard::get(int x, int y) {
    return chess[x][y];
}

void Chessboard::changeChess(Chess* newChess, int x, int y) {
    chess[x][y] = newChess;
    if (newChess != nullptr) newChess->changePos(x, y);
}

void Chessboard::changeMovableFlag(int x, int y, int flag) {
    movableFlag[x][y] = flag;
}

int Chessboard::getMovableFlag(int x, int y) {
    return movableFlag[x][y];
}
