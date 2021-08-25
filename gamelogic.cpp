#include "gamelogic.h"
#include "chess.h"
#include "chessboard.h"
#include <QMessageBox>
#include <QChar>
#include <QTimer>

class Chess;

GameLogic::GameLogic(QWidget *parent): QWidget(parent) {
    flag = false;
    chess1 = nullptr;
    turnCount = 1;
}

void GameLogic::setChessboard(Chessboard* chessboard) {
    this->chessboard = chessboard;
}

void GameLogic::setInfoboard(Infoboard* infoboard) {
    this->infoboard = infoboard;
}

void GameLogic::setPlayer(int first) {
    if (nowPlayer->getID() == first) return;
    else {
        Player* p = nowPlayer;
        nowPlayer = nextPlayer;
        nextPlayer = p;
    }
}

void GameLogic::setNowPlayer(Player* player) {
    nowPlayer = player;
}

void GameLogic::setNextPlayer(Player* player) {
    nextPlayer = player;
}

void GameLogic::setID(int id) {
    this->id = id;
}

void GameLogic::checkIsFirst() {
    if (nowPlayer->getID() == id) {
        QMessageBox::information(this, "", "You are the initiative!");
    }
}
void GameLogic::changePlayer() {

    turnCount++;

    Player* p = nowPlayer;
    nowPlayer = nextPlayer;
    nextPlayer = p;

    if (chess1 != nullptr) chess1->changeHighlight(false);
    chess1 = nullptr;
    flag = false;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 12; j++)
            chessboard->changeMovableFlag(i, j, false);

    emit render(1);

}

void GameLogic::dfs(int u, int type, bool flag, int cnt, bool isChessBetween) {
    visitable[u] = 1;
    if (isChessBetween) return;
    for (int i = 0; i < route[u].size(); i++) {
        int v = route[u][i].first, routeType = route[u][i].second;
        if (visitable[v]) continue;

        targetChess = chessboard->get(v % 5, v / 5);

        if (!isChessBetween)
            if (cnt == 0 ||
                    (routeType == type && routeType == 1 && cnt < 1) ||
                    (routeType == type && routeType != 1) ||
                    (flag == true && type != 1 && routeType != 1)) {

                if (targetChess == nullptr) dfs(v, routeType, flag, cnt + 1, isChessBetween);
                else if (targetChess->getStatus() == 1) dfs(v, routeType, flag, cnt + 1, true);

            }
    }
}

void GameLogic::checkGameboard() {
    int type1 = chess1->getType();
    for (int i = 0; i < 60; i++)
        if (visitable[i] > 0) {

            Chess* chess2 = chessboard->get(i % 5, i / 5);

            if (chess2 == nullptr) {
                visitable[i] = 1;
                continue;
            } else if (postID[i] || chess2->getColor() == nowPlayer->getColor()) {
                visitable[i] = 0;
                continue;
            }

            int type2 = chess2->getType();


            switch(type1) {
            case 11:
                switch(type2) {
                case 12:
                    if (nextPlayer->getLandminesRemains() == 0) visitable[i] = 10;
                    else visitable[i] = 0;
                    break;
                case 10:
                    visitable[i] = 5;
                    break;
                default:
                    visitable[i] = 3;
                    break;
                }
                break;
            case 9:
                switch(type2) {
                case 12:
                    if (nextPlayer->getLandminesRemains() == 0) visitable[i] = 10;
                    else visitable[i] = 0;
                    break;
                case 10:
                    visitable[i] = 4;
                    break;
                case 9:
                    visitable[i] = 3;
                    break;
                default:
                    visitable[i] = 0;
                    break;
                }
                break;
            default:
                switch(type2) {
                case 12:
                    if (nextPlayer->getLandminesRemains() == 0) visitable[i] = 10;
                    else visitable[i] = 0;
                    break;
                case 11: case 10:
                    visitable[i] = 0;
                    break;
                default:
                    if (type1 == type2) visitable[i] = 3;
                    else if (type1 < type2) visitable[i] = 2;
                    else visitable[i] = 0;
                    break;
                }
                break;
            }
        }
}

QString GameLogic::QS(int x) {
    return QString::number(x);
}

void GameLogic::moveChess1(Chess* chess, int x, int y) {

    if (chess == nullptr) return;

    if (chess->getStatus() == 0) {

        emit sendData("d " + QString::number(x) + " " + QString::number(y));

    } else if (chess->getColor() == nowPlayer->getColor()) {

        int type = chess->getType();
        if (type == 10 || type == 12) return;

        else {
            chess1 = chess;
            chess1->changeHighlight(true);
            flag = true;

            memset(visitable, 0, sizeof(visitable));

            bool dfsFlag = false;
            if (chess->getType() == 9) dfsFlag = true;

            dfs(x + 5 * y, 0, dfsFlag, 0, false);

            checkGameboard();

            visitable[x + 5 * y] = 0;
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 12; j++) {
                    chessboard->changeMovableFlag(i, j, visitable[i + 5 * j]);
                }

        }

    }

}

void GameLogic::moveChess2(Chess* chess2, int x2, int y2) {
    int x1 = chess1->getPosX(), y1 = chess1->getPosY();
    int color1 = chess1->getColor(), type1 = chess1->getType();
    int color2, type2;
    if (chess2 != nullptr) {
        color2 = chess2->getColor();
        type2 = chess2->getType();
    }
    int pos1 = x1 + 5 * y1, pos2 = x2 + 5 * y2;
    QString msg;
    switch (visitable[pos2]) {
    case 0:
        failMove();
        if (x1 + 5 * y1 != x2 + 5 * y2)
            infoboard->addGameLog(QString("[Warning] Illegal move!"));
        return;
    case 1:
        changeChess(chess1, x2, y2);
        changeChess(nullptr, x1, y1);
        emit sendData("2 " + QS(turnCount) + " " + QS(color1) + " " + QS(type1) + " " + QS(pos1) + " " + QS(pos2));
        break;
    case 2:
        changeChess(chess1, x2, y2);
        changeChess(nullptr, x1, y1);
        emit sendData("3 " + QS(turnCount) + " " + QS(color1) + " " + QS(type1) + " " + QS(color2) + " " + QS(type2) + " " + QS(pos1) + " " + QS(pos2));
        break;
    case 3:
        changeChess(nullptr, x2, y2);
        changeChess(nullptr, x1, y1);
        emit sendData("4 " + QS(turnCount) + " " + QS(color1) + " " + QS(type1) + " " + QS(color2) + " " + QS(type2) + " " + QS(pos1) + " " + QS(pos2));
        break;
    case 4:
        emit sendData("l " + QS(nextPlayer->getID()));
        changeChess(chess1, x2, y2);
        changeChess(nullptr, x1, y1);
        emit sendData("3 " + QS(turnCount) + " " + QS(color1) + " " + QS(type1) + " " + QS(color2) + " " + QS(type2) + " " + QS(pos1) + " " + QS(pos2));
        break;
    case 5:
        emit sendData("l " + QS(nextPlayer->getID()));
        changeChess(nullptr, x1, y1);
        changeChess(nullptr, x2, y2);
        emit sendData("4 " + QS(turnCount) + " " + QS(color1) + " " + QS(type1) + " " + QS(color2) + " " + QS(type2) + " " + QS(pos1) + " " + QS(pos2));
        break;
    case 10:
        int color = chess1->getColor(), type = chess1->getType();
        changeChess(nullptr, x1, y1);
        changeChess(chess1, x2, y2);
        emit sendData("5 " + QS(turnCount) + " " + QS(color) + " " + QS(type));
        emit sendData("g 1");
        return;
    }
    emit sendData("e");
//    changePlayer();
}

void GameLogic::failMove() {
    flag = false;
    chess1->changeHighlight(false);
    chess1 = nullptr;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 12; j++) {
            chessboard->changeMovableFlag(i, j, false);
        }

}

void GameLogic::moveChess(int i, int j) {
    if (id != nowPlayer->getID()) return;

    Chess* chess = chessboard->get(i, j);
    if (flag == false) moveChess1(chess, i, j);
    else moveChess2(chess, i, j);

}

void GameLogic::changeChess(Chess* nowChess, int x, int y) {
    QString pos = QString::number(x) + " " + QString::number(y);
    if (nowChess == nullptr) {
        QString msg = "c " + pos + " -1";
        emit sendData(msg);
    } else {
        QString status = QString::number(nowChess->getStatus());
        QString color = QString::number(nowChess->getColor());
        QString type = QString::number(nowChess->getType());
        QString msg = "c " + pos + " " + status + " " + color + " " + type;
        emit sendData(msg);
    }
}

void GameLogic::onChangeChessNULL(int x, int y) {
    chessboard->changeChess(nullptr, x, y);
    emit render(1);
}

void GameLogic::onChangeChess(int x, int y, int status, int color, int type) {
    Chess *chess = new Chess(x, y, type, color, status);
    chessboard->changeChess(chess, x, y);
    emit render(1);
}

void GameLogic::setNewColor(int color, int playerID) {
    if (nowPlayer->getID() == playerID) {
        nowPlayer->changeColor(color);
        nextPlayer->changeColor(3 - color);
    } else {
        nowPlayer->changeColor(3 - color);
        nextPlayer->changeColor(color);
    }
}

void GameLogic::onMineBoomed(int playerID) {
    if (nowPlayer->getID() == playerID) {
        nowPlayer->lostLandmines();
    } else {
        nextPlayer->lostLandmines();
    }
    if (playerID == id) {
        emit decYourMine();
    } else {
        emit decEnemyMine();
    }
}
