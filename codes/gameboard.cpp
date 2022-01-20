#include "gameboard.h"
#include "chess.h"
#include "chessboard.h"
#include "clickablelabel.h"
#include "infoboard.h"
#include <QPainter>
#include <QPen>
#include <QVector>

Gameboard::Gameboard(QWidget *parent): QWidget(parent) {

    createUI();
    isGameStarted = false;
    isGameEnded = false;

}

void Gameboard::setChessboard(Chessboard* chessboard) {
    this->chessboard = chessboard;
    render(0);
}

void Gameboard::setGameLogic(GameLogic* gamelogic) {
    this->gamelogic = gamelogic;
}

void Gameboard::createUI() {
    QLabel* background = new QLabel(this);
    QPixmap backgroundimage(":/img/newchessboard.png");
    this->setFixedSize(435, 551);

    background->setGeometry(0, 0, 435, 551);
    background->setScaledContents(true);
    background->setPixmap(backgroundimage);
}

Gameboard::~Gameboard() {

}

void Gameboard::render(int cmd) {

    int x0 = 15, y0 = 5, dx = 84, dy = 38, dt = 120 - 38;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 12; j++) {

            Chess* chess = chessboard->get(i, j);  

            int x = x0 + dx * i;
            int y = y0 + dy * j;
            if (j >= 6) y += dt;

            if (cmd == 0) {
                chessLabel[i][j] = new ClickableLabel(this);
                chessLabel[i][j]->setGeometry(x, y, 71, 31);
                chessLabel[i][j]->setScaledContents(true);

                connect(chessLabel[i][j], &ClickableLabel::clicked, this, [=](){onChessClicked(i, j);});
            }

            if (chess != nullptr) {

                QString filename = ":/img/";

                if (chess->getStatus() == 0) filename += "tbd";
                else {
                    filename += dict[chess->getType()];

                    if (chess->getColor() == 1) filename += "r";
                    else filename += "b";

                    if (chess->isHighlighted()) filename += "hl";
                }

                filename += ".png";
                QPixmap chessimage(filename);

                if (chessboard->getMovableFlag(i, j)) {

                    QPainter painter;
                    painter.begin(&chessimage);

                    QPen pen(Qt::yellow);
                    pen.setWidth(25);
                    pen.setStyle(Qt::DashLine);
                    pen.setDashPattern(QVector<qreal>() << 0.4 << 2.3);

                    painter.setPen(pen);
                    painter.drawRect(0, 0, chessimage.width(), chessimage.height());

                    painter.end();
                }

                chessLabel[i][j]->setPixmap(chessimage);

            } else {

                chessLabel[i][j]->setPixmap(QPixmap());

            }

            if (chessboard->getMovableFlag(i, j) && chessboard->get(i, j) == nullptr) {

                chessLabel[i][j]->setPixmap(QPixmap(":/img/moveable.png"));

            }
        }

    update();

}

void Gameboard::onChessClicked(int i, int j) {

    if (!isGameStarted) return;
    if (isGameEnded) return;
    emit tryMoving(i, j);
    render(1);

}

void Gameboard::show() {
    qDebug() << "Start render\n";
    isGameStarted = true;
    render(1);
}

void Gameboard::gameEnded() {
    isGameEnded = true;
}
