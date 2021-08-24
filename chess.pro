QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chess.cpp \
    chessboard.cpp \
    clickablelabel.cpp \
    client.cpp \
    gameboard.cpp \
    gamelogic.cpp \
    infoboard.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    server.cpp

HEADERS += \
    chess.h \
    chessboard.h \
    clickablelabel.h \
    client.h \
    gameboard.h \
    gamelogic.h \
    infoboard.h \
    mainwindow.h \
    player.h \
    server.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    img/素材/png/工兵_红.png

RESOURCES += \
    img.qrc \
    img.qrc
