#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "gameboard.h"
#include "infoboard.h"
#include "gamelogic.h"
#include "serverlogic.h"
#include "chessboard.h"
#include "client.h"
#include "server.h"
#include "player.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    int id;
    int first;
    bool gameEnded;

    Infoboard* infoboard;
    Gameboard* gameboard;
    Chessboard* chessboard;
    GameLogic* gamelogic;
    ServerLogic* serverlogic;
    Client* client;
    Server* server;
    QVector<Player*> player = {nullptr, nullptr};

    void createGameStats();
    void createUI();
    void createActions();
    void createMenus();
    void createNetWork();

    QToolBar* toolBar;
    QMenu *connectMenu;
    QMenu *playMenu;
    QAction *createConnectionAct;
    QAction *connectToServerAct;
    QAction *startAct;
    QAction *surrenderAct;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createConnection();
    void connectToServer();
    void start();
    void surrender();

public slots:
    void newID(int);
    void onGameEnded();
};

#endif // MAINWINDOW_H
