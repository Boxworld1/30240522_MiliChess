#include "server.h"

Server::Server(QDialog *parent) : QDialog(parent) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
    serverUI();
    ready[0] = ready[1] = false;
    id = 0;
}

void Server::onNewConnection() {
    for (int i = 0; i < 2; i++)
    if (clientSocket[i] == nullptr){
        clientSocket[i] = server->nextPendingConnection();
        connect(clientSocket[i], &QTcpSocket::readyRead, [=](){this->onReadyRead(i);});
        connect(clientSocket[i], &QTcpSocket::disconnected, [=](){this->onDisconnect(i);});
        sendDataSlot("i " + QString::number(i), i);
        if (i != id) {
            emit initialize(i);
        }
        qDebug() << "New Connection: " << i << "\n";
        break;
    }
}

void Server::setID(int id) {
    this->id = id;
}

void Server::showUI() {
    show();
}

void Server::serverUI() {

    label = new QLabel();
    QString IP = getLocalIP();
    label->setText("Host IP: " + IP);
    label->setAlignment(Qt::AlignCenter);

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    buttons->setCenterButtons(true);

    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        server->listen(QHostAddress::Any, 8888);
        emit selfConnect(getLocalIP());
        done(QDialog::Accepted);
    });

    connect(buttons, &QDialogButtonBox::rejected, this, [this]() {
        onDisconnect(0);
        onDisconnect(1);
        done(QDialog::Rejected);
    });

    vbox = new QVBoxLayout(this);

    vbox->addWidget(label);
    vbox->addWidget(buttons);
    this->setFixedSize(225, 100);

}

QString Server::getLocalIP() {
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             return address.toString();
    }
    return "";
}

void Server::gameStart() {
    for (int i = 0; i < 2; i++)
        sendDataSlot("s", i);
    emit timerStart(20);
}

void Server::onDisconnect(int id) {
    clientSocket[id] = nullptr;
    qDebug() << id << " disconnected\n";
}

void Server::onReadyRead(int id) {

    if (clientSocket[id] == nullptr) return;

    while(clientSocket[id]->canReadLine()) {
        QString msg = clientSocket[id]->readLine();
        QTextStream in(&msg);

        char cmd;
        int x, y, status, color, type;
        in >> cmd;

        switch(cmd) {
        case 'c':
            in >> x >> y >> status;
            if (status == -1) {
                emit changeChessNULL(msg, x, y);//set nullptr
            } else {
                in >> color >> type;
                emit changeChess(msg, x, y, status, color, type);//set chess
            }
            break;

        case 'd':
            in >> x >> y;
            emit flopChess(x, y);
            break;
        case 'e':
            emit endTurn();
            break;
        case 'f':
            emit surrender(id);
            break;

        case 'g':
            in >> x;
            emit endGame(x);
            break;

        case 's':
            ready[id] = 1;
            if (ready[0] + ready[1] == 2) gameStart();
            break;

        case 'l':
            in >> x;
            emit mineBoomed(x);
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
            sendDataSlot(msg, 0);
            sendDataSlot(msg, 1);
            break;
        default:

            break;
        }

        qDebug() << "server recv: " << msg;
    }
}

void Server::sendDataSlot(QString msg, int id) {
    QByteArray str = msg.toUtf8();
    str.append('\n');
    if (clientSocket[id] != nullptr) {
        clientSocket[id]->write(str);
        qDebug() << "server send: " << str;
    }
}
