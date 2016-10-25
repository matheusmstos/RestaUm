#include "Peca.h"

Peca::Peca(QWidget *parent)
    : QPushButton(parent),
      m_state(Peca::Empty) {
    this->updateIcon();
}

Peca::~Peca() {
}

void Peca::setState(Peca::State state) {
    if (m_state != state) {
        m_state = state;
        this->updateIcon();

        emit stateChanged(state);
    }
}


void Peca::updateIcon() {
    switch (m_state) {
        case Peca::Empty:
            this->setIcon(QPixmap(":/empty"));
            break;
        case Peca::Filled:
            this->setIcon(QPixmap(":/filled"));
            break;
        case Peca::Selected:
            this->setIcon(QPixmap(":/selected"));
            break;
        case Peca::Jumpable:
            this->setIcon(QPixmap(":/jumpable"));
            break;
    }
}

Peca::State Peca::getState() {
    return m_state;
}

int Peca::getX() {
    return this->x;
}

int Peca::getY() {
    return this->y;
}

void Peca::setX(int n) {
    this->x = n;
}

void Peca::setY(int n) {
    this->y = n;
}

