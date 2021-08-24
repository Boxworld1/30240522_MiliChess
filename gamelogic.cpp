#include "gamelogic.h"
#include "chess.h"
#include "chessboard.h"
#include <QMessageBox>
#include <QChar>
#include <QTimer>

class Chess;

GameLogic::GameLogic(QWidget *parent): QWidget(parent) {

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &GameLogic::timeUpdate);

    turnCount = 1;
    flag = false;
    chess1 = nullptr;
    nowPlayer = new Player();
    nextPlayer = new Player();
    isColorGived = false;

}

void GameLogic::timerStart(int time) {
    timeRemain = time;
    timer->start(1000);
    emit timeRemainDecided(timeRemain);
}

void GameLogic::timeUpdate() {
    timer->stop();
    timeRemain--;
    emit timeRemainDecided(timeRemain);

    if (timeRemain <= 0) {
        timeover();
    } else {
        timer->start();
    }
}

void GameLogic::timeover() {

    QMessageBox* msg = new QMessageBox("Warning", "Timeout!",
                                       QMessageBox::Critical,
                                       QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape, 0
                                       );

    infoboard->addGameLog6(turnCount, nowPlayer->getColor()); //Need Correct
    msg->show();
    timer->stop();

    nowPlayer->setTimeoutCount();
    if (nowPlayer->getTimeoutCount() >= 3) gameEnded(2);
    else changePlayer();

}

bool GameLogic::getFlag() {
    return flag;
}

void GameLogic::setChessboard(Chessboard* chessboard) {
    this->chessboard = chessboard;
}

void GameLogic::setInfoboard(Infoboard* infoboard) {
    this->infoboard = infoboard;
}

void GameLogic::changePlayer() {
    timer->stop();
    turnCount++;
    emit playerDecided();
    Player* p = nowPlayer;
    nowPlayer = nextPlayer;
    nextPlayer = p;

    if (chess1 != nullptr) chess1->changeHighlight(false);
    chess1 = nullptr;
    flag = false;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 12; j++) {
            chessboard->changeMovableFlag(i, j, false);
        }

    timerStart(20);
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

bool GameLogic::moveable(Chess* chess2, int x2, int y2) {
    if (visitable[x2 + 5 * y2]) {
        if (chess2 == nullptr) return true;
        else if (!postID[x2 + 5 * y2]) return true;
    }
    return false;
    // need correct!
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
void GameLogic::moveChess1(Chess* chess, int x, int y) {

    if (chess == nullptr) return;

    if (chess->getStatus() == 0) {

        chess->changeStatus();
        int color = chess->getColor();
        int type = chess->getType();

        infoboard->addGameLog1(turnCount, color, type, x + 5 * y);

        if (!isColorGived) {
            if (nowPlayer->getLastColor() == color) {

                nowPlayer->changeColor(color);
                nextPlayer->changeColor(3 - color);

                QString gameLog1 = "[Server] Your color is " + QString((color == 1)? "Red": "Blue") + " now!";
                infoboard->addGameLog0(gameLog1); // send message to Players!

                if (turnCount % 2 == 1) {
                    emit colorDecided(color, 3 - color);
                } else {
                    emit colorDecided(3 - color, color);
                }

                isColorGived = true;

                QMessageBox* msg = new QMessageBox("Warning", "Your Color is " + QString((color == 1)? "Red": "Blue"),
                                                   QMessageBox::Critical,
                                                   QMessageBox::Ok | QMessageBox::Default,
                                                   QMessageBox::Cancel | QMessageBox::Escape, 0
                                                   );
                msg->show();

            }

            nowPlayer->changeLastColor(color);
        }

        changePlayer();

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
    switch (visitable[pos2]) {
    case 0:
        failMove();
        if (x1 + 5 * y1 != x2 + 5 * y2)
            infoboard->addGameLog0(QString("[Warning] Illegal move!"));
        return;
    case 1:
        chessboard->changeChess(chess1, x2, y2);
        chessboard->changeChess(nullptr, x1, y1);
        infoboard->addGameLog2(turnCount, color1, type1, pos1, pos2);
        break;
    case 2:
        chessboard->changeChess(chess1, x2, y2);
        chessboard->changeChess(nullptr, x1, y1);
        infoboard->addGameLog3(turnCount, color1, type1, color2, type2, pos1, pos2);
        break;
    case 3:
        chessboard->changeChess(nullptr, x1, y1);
        chessboard->changeChess(nullptr, x2, y2);
        infoboard->addGameLog4(turnCount, color1, type1, color2, type2, pos1, pos2);
        break;
    case 4:
        nextPlayer->lostLandmines();
        chessboard->changeChess(chess1, x2, y2);
        chessboard->changeChess(nullptr, x1, y1);
        infoboard->addGameLog3(turnCount, color1, type1, color2, type2, pos1, pos2);
        infoboard->addGameLog0("[Server] The enemy remains " + QString(QChar(nextPlayer->getLandminesRemains() + 48)) + " landmine(s)! ");
        break;
    case 5:
        nextPlayer->lostLandmines();
        chessboard->changeChess(nullptr, x1, y1);
        chessboard->changeChess(nullptr, x2, y2);
        infoboard->addGameLog4(turnCount, color1, type1, color2, type2, pos1, pos2);
        infoboard->addGameLog0("[Server] The enemy remains " + QString(QChar(nextPlayer->getLandminesRemains() + 48)) + " landmine(s)! ");
        break;
    case 10:
        int color = chess1->getColor(), type = chess1->getType();
        infoboard->addGameLog5(turnCount, color, type);
        gameEnded(1);
        return;
    }
    changePlayer();
}

void GameLogic::gameEnded(int cmd) {
    timer->stop();
    infoboard->addGameLog0("[Server] Gameover! " + QString((nowPlayer->getColor() == cmd)? "Red": "Blue") + " side wins!");
    QMessageBox* msg = new QMessageBox("Congrats!", "You win!",
                                       QMessageBox::Critical,
                                       QMessageBox::Ok | QMessageBox::Default,
                                       QMessageBox::Cancel | QMessageBox::Escape, 0
                                       );
    msg->show();
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
    Chess* chess = chessboard->get(i, j);

    if (flag == false) {

        moveChess1(chess, i, j);

    } else {

        moveChess2(chess, i, j);

    }
}
