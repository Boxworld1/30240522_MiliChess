#include "mainwindow.h"
#include "gamelogic.h"
#include <QToolBar>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {

    createGameStats();
    createUI();
    createActions();
    createMenus(); 
    createNetWork();

}

void MainWindow::createNetWork() {

    server = new Server();
    client = new Client();
    connect(gameboard, &Gameboard::sendMoved, client, &Client::sendDataSlot);
    connect(server, &Server::moved, gamelogic, &GameLogic::moveChess);
    connect(server, &Server::start, gameboard, &Gameboard::show);

}

void MainWindow::createGameStats() {

    chessboard = new Chessboard();
    infoboard = new Infoboard();
    gameboard = new Gameboard();
    gamelogic = new GameLogic();

    gameboard->setChessboard(chessboard);
    gamelogic->setChessboard(chessboard);
    gamelogic->setInfoboard(infoboard);

    connect(gamelogic, &GameLogic::colorDecided, infoboard, &Infoboard::getColorDecided);
    connect(gamelogic, &GameLogic::playerDecided, infoboard, &Infoboard::getPlayerDecided);
    connect(gamelogic, &GameLogic::timeRemainDecided, infoboard, &Infoboard::getTimeRemainDecided);

}
void MainWindow::createUI() {
    setWindowTitle(tr("Chess"));

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(gameboard);
    layout->addWidget(infoboard);
    widget->setLayout(layout);

//    this->setFixedSize(700, 561);
}

void MainWindow::createActions() {

    createConnectionAct = new QAction(tr("Create Connection"), this);
    connect(createConnectionAct, &QAction::triggered, this, &MainWindow::createConnection);

    connectToServerAct = new QAction(tr("Connect to Server"), this);
    connect(connectToServerAct, &QAction::triggered, this, &MainWindow::connectToServer);

    startAct = new QAction(tr("Start"), this);
    connect(startAct, &QAction::triggered, this, &MainWindow::start);

    surrenderAct = new QAction(tr("Surrender"), this);
    connect(surrenderAct, &QAction::triggered, this, &MainWindow::surrender);

}

void MainWindow::createMenus() {

    connectMenu = menuBar()->addMenu(tr("&Connect"));
    connectMenu->addAction(createConnectionAct);
    connectMenu->addAction(connectToServerAct);

    playMenu = menuBar()->addMenu(tr("&Play"));
    playMenu->addAction(startAct);
    playMenu->addAction(surrenderAct);

}

void MainWindow::createConnection() {
    server->showUI();
}

void MainWindow::connectToServer() {
    client->showUI();
}

void MainWindow::start() {
    client->sendDataSlot("s");
}

void MainWindow::surrender() {

}

MainWindow::~MainWindow() {

}
