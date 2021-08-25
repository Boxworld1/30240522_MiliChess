#include "infoboard.h"
#include <QGridLayout>
#include <QRadioButton>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>

Infoboard::Infoboard(QWidget *parent): QWidget(parent) {

    yourColor = enemyColor = 0;
    yourMine = enemyMine = 3;
    nowPlayer = 0;
    timeRemain = 20;

    createUI();
    createStatsGroup();
    createTimerGroup();

    addGameLog("----- Hints -----");
    addGameLog("Use \"Connect\"->\"Create Connection\" to setup server!");
    addGameLog("Use \"Connect\"->\"Connect to Server\" to connect a server!");
    addGameLog("If you are ready, click \"Play\"->\"Start\".");
    addGameLog("----- New Game -----");
}

void Infoboard::createUI() {
    grid = new QGridLayout;

    groupBox = new QGroupBox(tr("Player Stats"));
    firstStats = new QHBoxLayout();
    isPlayer1Turn = new QLabel();
    player1 = new QLabel();
    secondStats = new QHBoxLayout();
    isPlayer2Turn = new QLabel();
    player2 = new QLabel();
    vbox = new QVBoxLayout;

    groupBox2 = new QGroupBox(tr("Time left"));
    timeRemainLabel = new QLabel();
    hbox = new QHBoxLayout;
    text = new QTextEdit();

    text->setReadOnly(true);

    isPlayer1Turn->setFixedSize(30, 30);
    player1->setFixedSize(90, 30);
    isPlayer2Turn->setFixedSize(30, 30);
    player2->setFixedSize(90, 30);

    firstStats->addWidget(isPlayer1Turn);
    firstStats->addWidget(player1);
    firstStats->setContentsMargins(0, 0, 0, 5);
    secondStats->addWidget(isPlayer2Turn);
    secondStats->addWidget(player2);
    secondStats->setContentsMargins(0, 0, 0, 0);

    vbox->addLayout(firstStats);
    vbox->addLayout(secondStats);
    groupBox->setLayout(vbox);

    hbox->addWidget(timeRemainLabel);
    groupBox2->setLayout(hbox);

    grid->addWidget(text, 1, 0, 4, 2);
    grid->addWidget(groupBox, 0, 0);
    grid->addWidget(groupBox2, 0, 1);

    setLayout(grid);
    this->setFixedSize(300, 561);

}

void Infoboard::setFirstPlayer(int id) {
    nowPlayer = id;
    createStatsGroup();
}

void Infoboard::setYourMine() {
    yourMine--;
    createStatsGroup();
}

void Infoboard::setEnemyMine() {
    enemyMine--;
    createStatsGroup();
}

void Infoboard::getColorDecided(int your, int enemy) {
    yourColor = your;
    enemyColor = enemy;
    createStatsGroup();
}

void Infoboard::getPlayerDecided(int nPlayer) {
    qDebug() << "My id = " << id << " Now = " << nowPlayer << "\n";
    nowPlayer = nPlayer;
    createStatsGroup();
}

void Infoboard::getTimeRemainDecided(int _timeRemain) {
    timeRemain = _timeRemain;
    createTimerGroup();
}

void Infoboard::createStatsGroup() {

    isPlayer1Turn->setStyleSheet("QLabel { background-color : " + colorID[yourColor] + "; color: white; border-radius: 15px}");
    if (nowPlayer != id) {
        isPlayer1Turn->setStyleSheet("QLabel {}");
    }

    player1->setStyleSheet("QLabel { background-color : " + colorID[yourColor] + "; color : white; border-radius: 3px}");
    player1->setAlignment(Qt::AlignCenter);
    player1->setText("You (" + QString::number(yourMine) + ")");

    isPlayer2Turn->setStyleSheet("QLabel { background-color : " + colorID[enemyColor] + "; color: white; border-radius: 15px}");
    if (nowPlayer == id) {
        isPlayer2Turn->setStyleSheet("QLabel {}");
    }

    player2->setStyleSheet("QLabel { background-color : " + colorID[enemyColor] + "; color : white; border-radius: 3px}");
    player2->setAlignment(Qt::AlignCenter);
    player2->setText("Enemy (" + QString::number(enemyMine) + ")");

}

void Infoboard::createTimerGroup() {
    timeRemainLabel->setStyleSheet("QLabel { background-color : #666666; color : white; border-radius: 3px; font-size: 40px}");
    timeRemainLabel->setAlignment(Qt::AlignCenter);
    timeRemainLabel->setText(transferNumber(timeRemain));
}

QString Infoboard::transferColor(int color) {
    if (color == 1) return "red";
    if (color == 2) return "blue";
    return "TBD";
}

QString Infoboard::transferChessName(int type) {
    return chessName[type];
}

QString Infoboard::transferPos(int pos) {
    QString posX = transferNumber(pos % 5 + 1);
    QString posY = QChar(pos / 5 + 65);
    return QString(posY + posX);
}

QString Infoboard::transferNumber(int num) {
    QString target;
    do{
        target = QChar(num % 10 + 48) + target;
        num /= 10;
    } while (num);
    return target;
}

void Infoboard::addGameLog(QString newText) {
    text->append(newText);
}

void Infoboard::addGameLog1(int turn, int color, int type, int pos) {
    QString gameLog;
    gameLog = "[Turn" + transferNumber(turn) + "] A " + transferColor(color) + " " + transferChessName(type) + " is generated at " + transferPos(pos) + ".";
    text->append(gameLog);
}

void Infoboard::addGameLog2(int turn, int color, int type, int pos1, int pos2) {
    QString gameLog;
    gameLog = "[Turn" + transferNumber(turn) + "] A " + transferColor(color) + " " + transferChessName(type) + " moves from " + transferPos(pos1) + " to " + transferPos(pos2) + ".";
    text->append(gameLog);
}

void Infoboard::addGameLog3(int turn, int color1, int type1, int color2, int type2, int pos1, int pos2) {
    QString gameLog;
    gameLog = "[Turn" + transferNumber(turn) + "] The " + transferColor(color1) + " " + transferChessName(type1) + " at " + transferPos(pos1) + " wiped out the ";
    gameLog = gameLog + transferColor(color2) + " " + transferChessName(type2) + " at " + transferPos(pos2) + ".";
    text->append(gameLog);
}

void Infoboard::addGameLog4(int turn, int color1, int type1, int color2, int type2, int pos1, int pos2) {
    QString gameLog;
    gameLog = "[Turn" + transferNumber(turn) + "] The " + transferColor(color1) + " " + transferChessName(type1) + " at " + transferPos(pos1) + " and the ";
    gameLog = gameLog + transferColor(color2) + " " + transferChessName(type2) + " at " + transferPos(pos2) + " perish together!";
    text->append(gameLog);
}

void Infoboard::addGameLog5(int turn, int color, int type) {
    QString gameLog;
    gameLog = "[Turn" + transferNumber(turn) + "] The " + transferColor(color) + " " + transferChessName(type) + " got the flag!";
    text->append(gameLog);
}

void Infoboard::addGameLog6(int turn, int color) {
    QString gameLog;
    gameLog = "[Turn" + transferNumber(turn) + "] " + transferColor(color) + " side timeout!";
    text->append(gameLog);
}

void Infoboard::setID(int id) {
    this->id = id;
}

Infoboard::~Infoboard() {

}
