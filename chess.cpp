#include "chess.h"

Chess::Chess(int x, int y, int type, int color, int status): x(x), y(y), type(type), color(color), status(status), highlighted(false) {

}

int Chess::getColor() {
    return color;
}

int Chess::getStatus() {
    return status;
}

int Chess::getType() {
    return type;
}

void Chess::changeStatus() {
    status = 1;
}

bool Chess::isHighlighted() {
    return highlighted;
}

void Chess::changeHighlight(bool stat) {
    highlighted = stat;
}

int Chess::getPosX() {
    return x;
}

int Chess::getPosY() {
    return y;
}

void Chess::changePos(int x, int y) {
    this->x = x;
    this->y = y;
}

void Chess::changeInfo(int status, int color, int type) {
    this->status = status;
    this->color = color;
    this->type = type;
}
