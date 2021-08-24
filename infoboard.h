#ifndef INFOBOARD_H
#define INFOBOARD_H

#include <QGroupBox>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


class Infoboard: public QWidget {
    Q_OBJECT

public:
    explicit Infoboard(QWidget *parent = nullptr);
    void addGameLog0(QString);
    void addGameLog1(int, int, int, int);
    void addGameLog2(int, int, int, int, int);
    void addGameLog3(int, int, int, int, int, int, int);
    void addGameLog4(int, int, int, int, int, int, int);
    void addGameLog5(int, int, int);
    void addGameLog6(int, int);
    ~Infoboard();

private:
    int yourColor;
    int enemyColor;
    int nowPlayer;
    int timeRemain;

    QGridLayout *grid;
    QGroupBox *groupBox;
    QHBoxLayout *firstStats;
    QHBoxLayout *secondStats;
    QVBoxLayout *vbox;
    QLabel *isPlayer1Turn;
    QLabel *player1;
    QLabel *isPlayer2Turn;
    QLabel *player2;

    QGroupBox *groupBox2;
    QLabel *timeRemainLabel;
    QHBoxLayout *hbox;

    void createUI();
    void createStatsGroup();
    void createTimerGroup();

    QTextEdit* text;

    QMap<int, QString> chessName = {{1, "Field Marshal"}, {2, "General"}, {3, "Major General"}, {4, "Brigadier General"}, {5, "Colonel"},
                                   {6, "Major"}, {7, "Captain"}, {8, "Lieutenant"}, {9, "Sapper"}, {10, "Landmine"},
                                   {11, "Bomb"}, {12, "Flag"}};
    QMap<int, QString> colorID = {{0, "#666666"}, {1, "#af2c0b"}, {2, "#1c75a1"}};

    QString transferColor(int);
    QString transferChessName(int);
    QString transferPos(int);
    QString transferNumber(int);

public slots:
    void getColorDecided(int, int);
    void getPlayerDecided();
    void getTimeRemainDecided(int);
};

#endif // INFOBOARD_H
