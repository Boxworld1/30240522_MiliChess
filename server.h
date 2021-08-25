#ifndef SERVER_H
#define SERVER_H

#include <QtWidgets>
#include <QtNetwork>

class Server : public QDialog {
    Q_OBJECT
private:
    QTcpServer *server;
    QVector<QTcpSocket*> clientSocket = {nullptr, nullptr};

    QString getLocalIP();
    QLabel *label;
    QDialogButtonBox *buttons;
    QVBoxLayout *vbox;

    void gameStart();
    void onDisconnect(int);
    void onReadyRead(int);
    void serverUI();

    bool ready[2];
    int id;

public:
    explicit Server(QDialog *parent = nullptr);
    void setID(int);
    void showUI();
    void sendDataSlot(QString, int);

signals:
    void moved(int, int);
    void start();
    void changeChessNULL(QString, int, int);
    void changeChess(QString, int, int, int, int, int);
    void surrender(int);
    void endGame(int);
    void flopChess(int, int);
    void initialize(int);
    void timerStart(int);
    void endTurn();
    void selfConnect(QString);
    void mineBoomed(int);

private slots:
    void onNewConnection();

};

#endif // SERVER_H
