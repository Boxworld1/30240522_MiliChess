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

public:
    explicit Server(QDialog *parent = nullptr);
    void showUI();
    void sendDataSlot(QString, int);

signals:
    void moved(int, int);
    void start();
    void changeChessNULL(int, int);
    void changeChess(int, int, int, int, int);
    void surrender(int);
    void endGame(int);
    void flopChess(int, int);

private slots:
    void onNewConnection();

};

#endif // SERVER_H
