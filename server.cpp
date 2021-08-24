#include "server.h"

Server::Server(QDialog *parent) : QDialog(parent) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
    serverUI();
    ready[0] = ready[1] = false;
}

void Server::onNewConnection() {
    for (int i = 0; i < 2; i++)
    if (clientSocket[i] == nullptr){
        clientSocket[i] = server->nextPendingConnection();
        connect(clientSocket[i], &QTcpSocket::readyRead, [=](){this->onReadyRead(i);});
        connect(clientSocket[i], &QTcpSocket::disconnected, [=](){this->onDisconnect(i);});
        qDebug() << "New Connection: " << i << "\n";
        break;
    }
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
    emit start();
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
        int x, y;
        in >> cmd;

        switch(cmd) {
        case 'c':
            in >> x >> y;
            emit moved(x, y);
            break;
        case 's':
            ready[id] = 1;
            if (ready[0] + ready[1] == 2) gameStart();
            break;
        default:
            break;
        }

        qDebug() << "recv: " << msg;
    }
}

void Server::sendDataSlot(QString msg, int id) {
    QByteArray str = msg.toUtf8();
    str.append('\n');
    if (clientSocket[id] != nullptr) {
        clientSocket[id]->write(str);
        qDebug() << str;
    }
}
