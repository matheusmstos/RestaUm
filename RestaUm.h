#ifndef RESTAUM_H
#define RESTAUM_H

#include <Peca.h>
#include <QMainWindow>

namespace Ui {
    class RestaUm;
}

class RestaUm : public QMainWindow {
    Q_OBJECT

public:
    explicit RestaUm(QWidget *parent = 0);
    ~RestaUm();

signals:
    void gameOver(int n);

private:
    Ui::RestaUm *ui;
    Peca* m_pecas[7][7];
    bool direcoes[3];

private slots:
    void play();
    void mostrarSobre();
    void mostrarFimJogo(int n);
    void trocarModo(QAction* modo);
    void desenharTabuleiro();
    void checandoMovimentos(int r, int c);
    bool movimentoPossivel(int r, int c, int direcao);
    void comivel(int r, int c);
};

#endif // RESTAUM_H
