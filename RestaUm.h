#ifndef RESTAUM_H
#define RESTAUM_H

#include <QMainWindow>

namespace Ui {
class RestaUm;
}

class RestaUm : public QMainWindow
{
    Q_OBJECT

public:
    explicit RestaUm(QWidget *parent = 0);
    ~RestaUm();

private:
    Ui::RestaUm *ui;

private slots:
    void trocarModo(QAction* acao);
    void mostrarSobre();
};

#endif // RESTAUM_H
