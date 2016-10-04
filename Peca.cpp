#include "Peca.h"

Peca::Peca(QWidget *parent) : QPushButton(parent), m_type(Peca::Empty) {
    this->updateIcon();
}

void Peca::updateIcon()
{
    switch(m_type)
    {
        case Peca::Empty:
            this ->setIcon(QPixmap(":/empty"));
            break;
        case Peca::Filled:
            this ->setIcon(QPixmap(":/filled"));
            break;
        case Peca::Selected:
            this ->setIcon(QPixmap(":/selected"));
            break;
        case Peca::Jumpable:
            this ->setIcon(QPixmap(":/jumpable"));
            break;

    }

}
