#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "gameboard.h"
#include "infoboard.h"
#include "gamelogic.h"
#include "chessboard.h"
#include "client.h"
#include "server.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Infoboard* infoboard;
    Gameboard* gameboard;
    Chessboard* chessboard;
    GameLogic* gamelogic;
    Client* client;
    Server* server;

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
};

#endif // MAINWINDOW_H
