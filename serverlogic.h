#ifndef SERVERLOGIC_H
#define SERVERLOGIC_H
#include "player.h"
#include "chessboard.h"
#include "chess.h"
#include <QWidget>
#include <QTimer>

class ServerLogic : public QWidget {
    Q_OBJECT
private:
    QTimer* timer;
    Player* nowPlayer;
    Player* nextPlayer;
    Chessboard* chessboard;

    int timeRemain;
    int turnCount;
    bool isColorGived;

public:
    explicit ServerLogic(QWidget *parent = nullptr);
    void setChessboard(Chessboard*);
    void setNowPlayer(Player*);
    void setNextPlayer(Player*);

    void gameEnded(int, int);
    void timerStart(int);
    void timeover();
    void changePlayer();

signals:
    void timeRemainDecided(int);
    void sendData(QString, int);

public slots:
    void timeUpdate();
    void endGame(int);
    void changeChessNULL(int, int);
    void changeChess(int, int, int, int, int);
    void getSurrender(int);
    void flopChess(int, int);
};

#endif // SERVERLOGIC_H
