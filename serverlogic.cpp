#include "serverlogic.h"
#include <QMessageBox>
#include <QObject>

ServerLogic::ServerLogic(QWidget *parent) : QWidget(parent) {

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &ServerLogic::timeUpdate);

    isColorGived = false;
    turnCount = 1;
}

void ServerLogic::setChessboard(Chessboard* chessboard) {
    this->chessboard = chessboard;
}

void ServerLogic::setNowPlayer(Player* player) {
    nowPlayer = player;
}

void ServerLogic::setNextPlayer(Player* player) {
    nextPlayer = player;
}

void ServerLogic::timerStart(int time) {
    timeRemain = time;
    timer->start(1000);
    emit timeRemainDecided(timeRemain);
}

void ServerLogic::timeUpdate() {
    timer->stop();
    timeRemain--;
    emit timeRemainDecided(timeRemain);

    if (timeRemain <= 0) {
        timeover();
    } else {
        timer->start();
    }
}

void ServerLogic::changePlayer() {
    timer->stop();

    for (int i = 0; i < 2; i++)
        emit sendData("o", i);

    turnCount++;
//    emit playerDecided();
    Player* p = nowPlayer;
    nowPlayer = nextPlayer;
    nextPlayer = p;

    timerStart(20);
}

void ServerLogic::timeover() {

    emit sendData("w 0", nowPlayer->getID());

    for (int i = 0; i < 2; i++)
        emit sendData("6 " + QString::number(turnCount) + " " + QString::number(nowPlayer->getColor()), i);

    timer->stop();

    nowPlayer->setTimeoutCount();
    if (nowPlayer->getTimeoutCount() >= 3) gameEnded(1, nowPlayer->getID());
    else changePlayer();

}

void ServerLogic::gameEnded(int cmd, int id) {
    timer->stop();
    if (cmd == 0) {
        emit sendData("0 [Server] Game over! You Win !", id);
        emit sendData("g 0", id);
        emit sendData("0 [Server] Game over! You Lose!", 1 - id);
        emit sendData("g 1", 1 - id);
    } else {
        emit sendData("0 [Server] Game over! You Win !", 1 - id);
        emit sendData("g 0", 1 - id);
        emit sendData("0 [Server] Game over! You Lose!", id);
        emit sendData("g 1", id);
    }
}

void ServerLogic::changeChessNULL(int x, int y) {
    chessboard->changeChess(nullptr, x, y);
}

void ServerLogic::changeChess(int x, int y, int status, int color, int type) {
    Chess *chess = chessboard->get(x, y);
    chess->changePos(x, y);
    chess->changeInfo(status, color, type);
    chessboard->changeChess(chess, x, y);
}

void ServerLogic::flopChess(int x, int y) {
    Chess* chess = chessboard->get(x, y);

    chess->changeStatus();
    int color = chess->getColor();
    int type = chess->getType();

    for (int i = 0; i < 2; i++)
        emit sendData("1 " + QString::number(turnCount) + " " + QString::number(color) + " " + QString::number(type) + " " + QString::number(x + 5 * y), i);

    if (!isColorGived) {
        if (nowPlayer->getLastColor() == color) {

            nowPlayer->changeColor(color);
            emit sendData("b " + QString::number(color), nowPlayer->getID());
            emit sendData("0 [Server] Your color is " + QString((color == 1)? "Red": "Blue") + " now!", nowPlayer->getID());

            nextPlayer->changeColor(3 - color);
            emit sendData("b " + QString::number(3 - color) + " ", nextPlayer->getID());
            emit sendData("0 [Server] Your color is " + QString((3 - color == 1)? "Red": "Blue") + " now!", nextPlayer->getID());

//            if (turnCount % 2 == 1) {
//                emit colorDecided(color, 3 - color);
//            } else {
//                emit colorDecided(3 - color, color);
//            }

            isColorGived = true;
//            QMessageBox::warning(this, "", "Your Color is " + QString((color == 1)? "Red": "Blue"));

        }

        nowPlayer->changeLastColor(color);
    }

    changePlayer();
}

void ServerLogic::getSurrender(int id) {
    gameEnded(1, id);
}

void ServerLogic::endGame(int id) {
    gameEnded(0, nowPlayer->getID());
}
