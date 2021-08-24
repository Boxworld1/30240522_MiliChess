#include "player.h"

Player::Player(): timeoutCount(0), color(0), landminesRemains(3), lastColor(0) {

}

void Player::changeColor(int color) {
    this->color = color;
}

void Player::changeLastColor(int color) {
    this->lastColor = color;
}
void Player::lostLandmines() {
    landminesRemains--;
}

int Player::getTimeoutCount() {
    return timeoutCount;
}

int Player::getColor() {
    return color;
}

int Player::getLandminesRemains() {
    return landminesRemains;
}

int Player::getLastColor() {
    return lastColor;
}

void Player::setTimeoutCount() {
    timeoutCount++;
}
