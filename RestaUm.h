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
    void srMoves(int r, int c);
    void exMoves(int rp, int cp, int rl, int cl);
    QList<Peca*> minha_lista;
    void Cruz();
    void Mais();
    void Banquinho();
    void Flecha();
    void Piramide();
    void Losango();
    void Tradicional();
    void atualizarStatusBar();

signals:
    void gameOver();

private:
    Ui::RestaUm *ui;
    Peca* m_pecas[7][7];
    Peca* newPeca;
    int nPecas;
    int nyanAtivado;

private slots:
    void play();
    void mostrarSobre();
    void mostrarFimJogo();
    void trocarModo(QAction* modo);
    int getNpecas();
    void setNpecas(int nPecas);
    bool verificaSeHaJogadas();
    void JogoNovo();
    void playNyan();
    void Nyanficar();
    void Desnyanficar();
    void pontosExtras();
};

#endif // RESTAUM_H
