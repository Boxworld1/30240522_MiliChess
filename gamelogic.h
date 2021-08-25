#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <QWidget>
#include "player.h"
#include "infoboard.h"
#include "chessboard.h"

class GameLogic: public QWidget {
    Q_OBJECT
private:
    Infoboard* infoboard;
    Chessboard* chessboard;
    Chess* chess1;
    Chess* targetChess;
    Player* nowPlayer;
    Player* nextPlayer;

    bool flag;
    bool isColorGived;
    int turnCount;
    int id;

    std::vector<std::pair<int, int>> route[60] = {{{1,1},{5,1}},{{0,1},{2,1},{6,1}},{{1,1},{3,1},{7,1}},{{2,1},{4,1},{8,1}},{{3,1},{9,1}},{{0,1},{11,1},{6,2},{10,3}},{{1,1},{11,1},{5,2},{7,2}},{{2,1},{11,1},{12,1},{13,1},{6,2},{8,2}},{{3,1},{13,1},{7,2},{9,2}},{{4,1},{13,1},{8,2},{14,3}},{{11,1},{5,3},{15,3}},{{5,1},{6,1},{7,1},{10,1},{12,1},{15,1},{16,1},{17,1}},{{7,1},{11,1},{13,1},{17,1}},{{7,1},{8,1},{9,1},{12,1},{14,1},{17,1},{18,1},{19,1}},{{13,1},{9,3},{19,3}},{{11,1},{16,1},{21,1},{10,3},{20,3}},{{11,1},{15,1},{17,1},{21,1}},{{11,1},{12,1},{13,1},{16,1},{18,1},{21,1},{22,1},{23,1}},{{13,1},{17,1},{19,1},{23,1}},{{13,1},{18,1},{23,1},{14,3},{24,3}},{{21,1},{15,3},{25,3}},{{15,1},{16,1},{17,1},{20,1},{22,1},{25,1},{26,1},{27,1}},{{17,1},{21,1},{23,1},{27,1}},{{17,1},{18,1},{19,1},{22,1},{24,1},{27,1},{28,1},{29,1}},{{23,1},{19,3},{29,3}},{{21,1},{20,3},{26,2},{30,3}},{{21,1},{25,2},{27,2}},{{21,1},{22,1},{23,1},{26,2},{28,2},{32,3}},{{23,1},{27,2},{29,2}},{{23,1},{24,3},{28,2},{34,3}},{{36,1},{25,3},{31,2},{35,3}},{{36,1},{30,2},{32,2}},{{36,1},{37,1},{38,1},{27,3},{31,2},{33,2}},{{38,1},{32,2},{34,2}},{{38,1},{29,3},{33,2},{39,3}},{{36,1},{30,3},{40,3}},{{30,1},{31,1},{32,1},{35,1},{37,1},{40,1},{41,1},{42,1}},{{32,1},{36,1},{38,1},{42,1}},{{32,1},{33,1},{34,1},{37,1},{39,1},{42,1},{43,1},{44,1}},{{38,1},{34,3},{44,3}},{{36,1},{41,1},{46,1},{35,3},{45,3}},{{36,1},{40,1},{42,1},{46,1}},{{36,1},{37,1},{38,1},{41,1},{43,1},{46,1},{47,1},{48,1}},{{38,1},{42,1},{44,1},{48,1}},{{38,1},{43,1},{48,1},{39,3},{49,3}},{{46,1},{40,3},{50,3}},{{40,1},{41,1},{42,1},{45,1},{47,1},{50,1},{51,1},{52,1}},{{42,1},{46,1},{48,1},{52,1}},{{42,1},{43,1},{44,1},{47,1},{49,1},{52,1},{53,1},{54,1}},{{48,1},{44,3},{54,3}},{{46,1},{55,1},{45,3},{51,2}},{{46,1},{56,1},{50,2},{52,2}},{{46,1},{47,1},{48,1},{57,1},{51,2},{53,2}},{{48,1},{58,1},{52,2},{54,2}},{{48,1},{59,1},{49,3},{53,2}},{{50,1},{56,1}},{{51,1},{55,1},{57,1}},{{52,1},{56,1},{58,1}},{{53,1},{57,1},{59,1}},{{54,1},{58,1}}};
    constexpr static bool postID[60] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int visitable[60];

    QString QS(int);

public:
    GameLogic(QWidget *parent = nullptr);

    void setChessboard(Chessboard*);
    void setInfoboard(Infoboard*);
    void setNowPlayer(Player*);
    void setNextPlayer(Player*);
    void setID(int);

    void moveChess1(Chess*, int, int);
    void moveChess2(Chess*, int, int);

    void changeChess(Chess*, int, int);
    void failMove();
    void dfs(int, int, bool, int, bool);
    void checkGameboard();

signals:
    void colorDecided(int, int);
    void playerDecided();
    void timeRemainDecided(int);
    void sendData(QString);
    void render(int);
    void decYourMine();
    void decEnemyMine();
public slots:
    void setPlayer(int);
    void moveChess(int, int);
    void checkIsFirst();
    void changePlayer();
    void onChangeChessNULL(int, int);
    void onChangeChess(int, int, int, int, int);
    void setNewColor(int, int);
    void onMineBoomed(int);
};

#endif // GAMELOGIC_H
