#ifndef PLAYER_H
#define PLAYER_H

class Player{
    int timeoutCount;
    int landminesRemains;
    int color;
    int lastColor;
public:
    Player();
    void changeLastColor(int);
    void changeColor(int);
    void setTimeoutCount();
    void lostLandmines();
    int getColor();
    int getTimeoutCount();
    int getLandminesRemains();
    int getLastColor();
};

#endif // PLAYER_H
