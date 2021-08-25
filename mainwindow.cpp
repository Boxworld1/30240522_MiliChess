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

    connect(gamelogic, &GameLogic::sendData, client, &Client::sendDataSlot);
    connect(serverlogic, &ServerLogic::sendData, server, &Server::sendDataSlot);

    connect(client, &Client::changeID, this, &MainWindow::newID);
    connect(client, &Client::colorDecided, infoboard, &Infoboard::getColorDecided);
    connect(client, &Client::playerDecided, infoboard, &Infoboard::getPlayerDecided);
    connect(client, &Client::timeRemainDecided, infoboard, &Infoboard::getTimeRemainDecided);
    connect(client, &Client::sendMessage, infoboard, &Infoboard::addGameLog);
    connect(client, &Client::onChangePlayer, gamelogic, &GameLogic::changePlayer);
    connect(client, &Client::changeChessNULL, gamelogic, &GameLogic::onChangeChessNULL);
    connect(client, &Client::changeChess, gamelogic, &GameLogic::onChangeChess);
    connect(client, &Client::start, gameboard, &Gameboard::show);
    connect(client, &Client::start, gamelogic, &GameLogic::checkIsFirst);
    connect(client, &Client::firstPlayer, gamelogic, &GameLogic::setPlayer);
    connect(client, &Client::changeColor, gamelogic, &GameLogic::setNewColor);

    connect(server, &Server::selfConnect, client, &Client::setIP);
    connect(server, &Server::changeChessNULL, serverlogic, &ServerLogic::onChangeChessNULL);
    connect(server, &Server::changeChess, serverlogic, &ServerLogic::onChangeChess);
    connect(server, &Server::surrender, serverlogic, &ServerLogic::getSurrender);
    connect(server, &Server::endGame, serverlogic, &ServerLogic::endGame);
    connect(server, &Server::flopChess, serverlogic, &ServerLogic::flopChess);
    connect(server, &Server::initialize, serverlogic, &ServerLogic::initialize);
    connect(server, &Server::timerStart, serverlogic, &ServerLogic::timerStart);
    connect(server, &Server::endTurn, serverlogic, &ServerLogic::changePlayer);
}

void MainWindow::createGameStats() {
    id = 10;

    chessboard = new Chessboard();
    infoboard = new Infoboard();
    gameboard = new Gameboard();
    gamelogic = new GameLogic();
    serverlogic = new ServerLogic();

    for (int i = 0; i < 2; i++)
        player[i] = new Player(i);

    srand(time(NULL));
    first = rand() %2;

    gameboard->setChessboard(chessboard);
    gamelogic->setChessboard(chessboard);
    gamelogic->setInfoboard(infoboard);
    serverlogic->setChessboard(chessboard);

    infoboard->setFirstPlayer(first);

    gamelogic->setNowPlayer(player[first]);
    gamelogic->setNextPlayer(player[1 - first]);
    serverlogic->setNowPlayer(player[first]);
    serverlogic->setNextPlayer(player[1 - first]);

    connect(gamelogic, &GameLogic::render, gameboard, &Gameboard::render);
    connect(gameboard, &Gameboard::tryMoving, gamelogic, &GameLogic::moveChess);
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
    client->sendDataSlot("f");
}

void MainWindow::newID(int id) {
    this->id = id;
    gamelogic->setID(id);
    infoboard->setID(id);
    qDebug() << "my ID is "<< id << "\n";
}

MainWindow::~MainWindow() {

}
