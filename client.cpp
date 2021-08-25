#include "client.h"
#include <QDebug>
#include <QMessageBox>

Client::Client(QDialog *parent): QDialog(parent) {
    clientSocket = new QTcpSocket();

    connect(clientSocket, &QTcpSocket::connected, this, &Client::onConnection);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Client::onDisconnection);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

    createUI();
    isConnected = false;
}

void Client::setIP(QString ip) {
    textBox->setText(ip);
    onConnection();
}

void Client::showUI() {
    show();
}

Client::~Client() {

}

void Client::createUI() {
    grid = new QGridLayout();

    label = new QLabel(tr("Enter IP:"));
    textBox = new QLineEdit();
    textBox->setInputMask("000.000.000.000; ");

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    buttons->setCenterButtons(true);


    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        onConnection();
        done(QDialog::Accepted);
    });

    connect(buttons, &QDialogButtonBox::rejected, this, [this]() {
        onDisconnection();
        done(QDialog::Rejected);
    });

    grid->addWidget(label, 0, 0, 1, 1);
    grid->addWidget(textBox, 0, 1, 1, 1);
    grid->addWidget(buttons, 1, 0, 1, 2);
    setLayout(grid);

    this->setFixedSize(225, 100);

}

void Client::onConnection() {
    serverIP = textBox->text();
    if (!isConnected) {
        clientSocket->connectToHost(QHostAddress(serverIP), 8888);
        if (clientSocket->waitForConnected(1000)) {
            QMessageBox::information(this, "", "Connection succeeded!");
            isConnected = true;
        } else {
            clientSocket->disconnect();
            QMessageBox::warning(this, "", "Connection failed!");
        }

    }
}

void Client::onDisconnection() {
    if (clientSocket == nullptr) return;
    clientSocket->disconnectFromHost();
    clientSocket = nullptr;
    QMessageBox::information(this, "", "Disconnection succeeded!");
}

void Client::onReadyRead() {

    if (clientSocket == nullptr) return;

    while(clientSocket->canReadLine()) {
        QString msg = clientSocket->readLine();
        QTextStream in(&msg);

        char cmd;
        int x, y, status, color, type;
        in >> cmd;

        switch(cmd) {
        case 'b': // change color
            in >> x >> y;
            emit changeColor(x, y); // x = color, y = id;
            emit colorDecided(x, 3 - x);
            break;

        case 'i': // change id
            in >> x;
            emit changeID(x);
            break;

        case 'o': // change player
            emit onChangePlayer();
            break;

        case 'g': // game over!
            in >> x;
            switch(x) {
            case 0:
                QMessageBox::information(this, "", "Congrats! You win!");
                break;
            case 1:
                QMessageBox::information(this, "", "You Lose!");
                break;
            default:
                break;
            }
            break;
        case 'c':
            in >> x >> y >> status;
            if (status == -1) {
                emit changeChessNULL(x, y);//set nullptr
            } else {
                in >> color >> type;
                emit changeChess(x, y, status, color, type);//set chess
            }
            break;
        case 'w': // warning
            in >> x;
            switch(x) {
            case 0:
                QMessageBox::warning(this, "", "Time out!");
                break;
            default:
                break;
            }
            break;

        case 't': // timer
            in >> x;
            emit timeRemainDecided(x);
            break;

        case 'p': // now player
            in >> x;
            emit playerDecided(x);
            break;

        case 'q': // change first player
            in >> x;
            emit firstPlayer(x);
            break;

        case 's': // game start
            emit start();
            break;

        case '0': // send msg
            emit sendMessage(msg);
            break;

        default:
            break;
        }
        qDebug() << "client recv: " << msg;
    }
}

void Client::sendDataSlot(QString msg) {
    QByteArray str = msg.toUtf8();
    str.append('\n');
    if (clientSocket != nullptr) {
        clientSocket->write(str);
        qDebug() << "client send: " << str;
    }
}
