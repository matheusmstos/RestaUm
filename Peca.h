#ifndef PECA_H
#define PECA_H

#include <QPushButton>

class Peca : public QPushButton
{
    Q_OBJECT
    Q_ENUMS(Type)

public:
    enum Type {
        Empty,
        Filled,
        Selected,
        Jumpable
    };

    explicit Peca(QWidget *parent = 0);

signals:

public slots:

private:
    Peca::Type m_type;

private slots:
    void updateIcon();
};

#endif // PECA_H
