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
        if (clientSocket == nullptr) clientSocket = new QTcpSocket();
        clientSocket->connectToHost(QHostAddress(serverIP), 8888);
        if (clientSocket->waitForConnected(1000)) {
            QMessageBox::information(this, "", "Connection succeeded!");
            isConnected = true;
        } else {
            onDisconnection();
            QMessageBox::warning(this, "", "Connection failed!");
        }
    }
}

void Client::onDisconnection() {
    if (clientSocket == nullptr) return;
    sendDataSlot("m 0");
    isConnected = false;
    clientSocket->disconnectFromHost();
    clientSocket = nullptr;
    QMessageBox::information(this, "", "Disconnection succeeded!");
}

void Client::onReadyRead() {

    if (clientSocket == nullptr) return;

    while(clientSocket && clientSocket->canReadLine()) {
        QString msg = clientSocket->readLine();
        QTextStream in(&msg);

        char cmd;
        int x, y, status, color, type, a[10];
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
        case 'm': // message
            in >> x;
            switch(x) {
            case 0:
                QMessageBox::information(this, "", "Your color is Red!");
                break;
            case 1:
                QMessageBox::information(this, "", "Your color is Blue!");
                break;
            case 2:
                QMessageBox::information(this, "", "Server Closed!");
                onDisconnection();
                break;
            case 3:
                in >> y;
                QMessageBox::information(this, "", "Client #" + QString::number(y) + " Closed!");
                break;
            default:
                break;
            }
            break;

        case 'w': // warning
            in >> x;
            switch(x) {
            case 0:
                QMessageBox::warning(this, "", "Time out!");
                break;
            case 1:
                QMessageBox::warning(this, "", "You can't surrender now!");
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

        case 'l':
            in >> x;
            emit mineBoomed(x);
            break;

        case 'z':
            emit gameEnded();
            break;

        case '0':
            emit sendMessage(msg.remove(0, 2));
            break;

        case '1':
            for (int i = 0; i < 4; i++) in >> a[i];
            emit sendMessage1(a[0], a[1], a[2], a[3]);
            break;

        case '2':
            for (int i = 0; i < 5; i++) in >> a[i];
            emit sendMessage2(a[0], a[1], a[2], a[3], a[4]);
            break;

        case '3':
            for (int i = 0; i < 7; i++) in >> a[i];
            emit sendMessage3(a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
            break;

        case '4':
            for (int i = 0; i < 7; i++) in >> a[i];
            emit sendMessage4(a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
            break;

        case '5':
            for (int i = 0; i < 3; i++) in >> a[i];
            emit sendMessage5(a[0], a[1], a[2]);
            break;

        case '6':
            for (int i = 0; i < 2; i++) in >> a[i];
            emit sendMessage6(a[0], a[1]);
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
