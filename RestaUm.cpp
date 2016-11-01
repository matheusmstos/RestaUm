#include "RestaUm.h"
#include "ui_RestaUm.h"

#include <QDebug>
#include <QActionGroup>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QFile>

RestaUm::RestaUm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RestaUm) {

    ui->setupUi(this);

    QActionGroup* group = new QActionGroup(this);
    group->setExclusive(true);
    group->addAction(ui->actionTradicional);
    group->addAction(ui->actionCruz);
    group->addAction(ui->actionMais);
    group->addAction(ui->actionBanquinho);
    group->addAction(ui->actionFlecha);
    group->addAction(ui->actionPiramide);
    group->addAction(ui->actionLosango);

    nyanAtivado = 0;
    nPecas = 32;
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            m_pecas[r][c] =
                    this->findChild<Peca*>(
                        QString("peca%1%2").arg(r).arg(c));
            if (m_pecas[r][c]) {
                m_pecas[r][c]->setX(r);
                m_pecas[r][c]->setY(c);

                QObject::connect(
                            m_pecas[r][c],
                            SIGNAL(clicked()),
                            this,
                            SLOT(play()));

                m_pecas[r][c]->setState(Peca::Filled);
            }
        }
    }

    m_pecas[3][3]->setState(Peca::Empty);

    atualizarStatusBar();

    QObject::connect(
                group,
                SIGNAL(triggered(QAction*)),
                this,
                SLOT(trocarModo(QAction*)));

    QObject::connect(
                ui->actionSair,
                SIGNAL(triggered()),
                qApp,
                SLOT(quit()));

    QObject::connect(
                ui->actionSobre,
                SIGNAL(triggered()),
                this,
                SLOT(mostrarSobre()));
    QObject::connect(
                ui->actionNovo,
                SIGNAL(triggered()),
                this,
                SLOT(JogoNovo()));
    QObject::connect(
                ui->actionNyan,
                SIGNAL(triggered()),
                this,
                SLOT(playNyan()));

    QObject::connect(
                this,
                SIGNAL(gameOver()),
                this,
                SLOT(mostrarFimJogo()));

    this->adjustSize();
    this->setFixedSize(this->size());
}

RestaUm::~RestaUm() {
    delete ui;
}

void RestaUm::play() {
    Peca* peca = qobject_cast<Peca*>(
                QObject::sender());
    int r = peca->getX();
    int c = peca->getY();
    static int estado = 1;

    switch(estado){
    case 1:
        minha_lista.clear();
        srMoves(r, c);

        if(minha_lista.size() == 1){
            exMoves(r, c, minha_lista.front()->getX(), minha_lista.front()->getY());
            nPecas--;
        }
        else if(minha_lista.size() > 1) {
            this->newPeca = peca;
            newPeca->setState(Peca::Selected);
            if(nyanAtivado) {
                newPeca->setIcon(QPixmap(":/nyan_selected"));
            }
            foreach (Peca *p, minha_lista) {
                p->setState(Peca::Jumpable);
                if(nyanAtivado) {
                    p->setIcon(QPixmap(":/nyan_jumpable"));
                }
            }

            estado = 2;
        }
        break;

    case 2:
        if(peca->getState() != Peca::Selected && peca->getState() != Peca::Filled && peca->getState() != Peca::Empty) {
            if(minha_lista.contains(peca)) {
                exMoves(newPeca->getX(), newPeca->getY(), peca->getX(), peca->getY());
                nPecas--;
            }

            foreach (Peca *p, minha_lista) {
                if(p->getState() == Peca::Jumpable){
                    p->setState(Peca::Empty);
                    if(nyanAtivado) {
                        p->setIcon(QPixmap(":/nyan_empty"));
                    }
                }

            }
            estado = 1;
        }
        break;

    default:
        break;
    }

    atualizarStatusBar();

    if(estado == 1 && !verificaSeHaJogadas()) {
        emit gameOver();
    }

}

void RestaUm::srMoves(int r, int c){
    if(m_pecas[r][c]) {
        if((r-2) >= 0 && m_pecas[r-1][c] && m_pecas[r-2][c]
                && m_pecas[r-1][c]->getState() == Peca::Filled
                && m_pecas[r-2][c]->getState() == Peca::Empty && m_pecas[r][c]->getState() == Peca::Filled){
            minha_lista << m_pecas[r-2][c];
        }

        if((r+2) <= 6 && m_pecas[r+1][c] && m_pecas[r+2][c]
                && m_pecas[r+1][c]->getState() == Peca::Filled
                && m_pecas[r+2][c]->getState() == Peca::Empty && m_pecas[r][c]->getState() == Peca::Filled){
            minha_lista << m_pecas[r+2][c];
        }

        if((c-2) >= 0 && m_pecas[r][c-1] && m_pecas[r][c-2]
                && m_pecas[r][c-1]->getState() == Peca::Filled
                && m_pecas[r][c-2]->getState() == Peca::Empty && m_pecas[r][c]->getState() == Peca::Filled){
            minha_lista << m_pecas[r][c-2];
        }

        if((c+2) <= 6 && m_pecas[r][c+1] && m_pecas[r][c+2]
                && m_pecas[r][c+1]->getState() == Peca::Filled
                && m_pecas[r][c+2]->getState() == Peca::Empty && m_pecas[r][c]->getState() == Peca::Filled){
            minha_lista << m_pecas[r][c+2];
        }
    }

}

void RestaUm::exMoves(int rp, int cp, int rl, int cl){
        m_pecas[rl][cl]->setState(Peca::Filled);
        m_pecas[(rl+rp)/2][(cl+cp)/2]->setState(Peca::Empty);
        m_pecas[rp][cp]->setState(Peca::Empty);

    if(nyanAtivado) {
        m_pecas[rl][cl]->setIcon(QPixmap(":/nyan_filled"));
        m_pecas[(rl+rp)/2][(cl+cp)/2]->setIcon(QPixmap(":/nyan_empty"));
        m_pecas[rp][cp]->setIcon(QPixmap(":/nyan_empty"));
    }
}

void RestaUm::mostrarSobre() {
    QMessageBox::information(this,
                             tr("Sobre"),
                             tr("Resta Um\n\nPedro Guimarães - moisespedro15@gmail.com\n"
                                "Matheus Santos - matheusmstos@yahoo.com.br"));
}

void RestaUm::mostrarFimJogo() {
    if(nPecas != 1)
        QMessageBox::information(this,
                                 tr("Fim"),
                                 tr("Poxa, você perdeu! 😭😥"));
    else {
        QMessageBox::information(this,
                                 tr("Fim"),
                                 tr("Parabéns, você ganhou! 🔝👌😱"));
    }
    JogoNovo();
}

void RestaUm::playNyan() {
    nyanAtivado = 1;
//    if(QFile::exists(":/nyan")) {
//        qDebug() << "Existe";
//    } else {
//        qDebug() << "Não existe";
//    }
//    QSoundEffect effect;
//    effect.setSource(QUrl::fromLocalFile(":/nyan"));
//    effect.setLoopCount(5);
//    effect.setVolume(0.75f);
//    effect.play();
    Nyanficar();
}

void RestaUm::trocarModo(QAction* modo) {
    if (modo == ui->actionTradicional) {
        Tradicional();
        atualizarStatusBar();
    }
    else if (modo == ui->actionCruz){
        Cruz();
        atualizarStatusBar();
    }
    else if (modo == ui->actionMais) {
        Mais();
        atualizarStatusBar();
    }
    else if (modo == ui->actionBanquinho) {
        Banquinho();
        atualizarStatusBar();
    }
    else if (modo == ui->actionFlecha) {
        Flecha();
        atualizarStatusBar();
    }
    else if (modo == ui->actionPiramide) {
        Piramide();
        atualizarStatusBar();
    }
    else if (modo == ui->actionLosango) {
        Losango();
        atualizarStatusBar();
    }

    if(nyanAtivado) {
        Nyanficar();
    }
}

void RestaUm::Cruz(){
    nPecas = 6;
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c])
                m_pecas[r][c]->setState(Peca::Empty);
        }
    }

    m_pecas[1][3]->setState(Peca::Filled);
    m_pecas[2][3]->setState(Peca::Filled);
    m_pecas[3][3]->setState(Peca::Filled);
    m_pecas[4][3]->setState(Peca::Filled);
    m_pecas[2][2]->setState(Peca::Filled);
    m_pecas[2][4]->setState(Peca::Filled);
}

void RestaUm::Mais(){
    nPecas = 9;
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c])
                m_pecas[r][c]->setState(Peca::Empty);
        }
    }

    m_pecas[1][3]->setState(Peca::Filled);
    m_pecas[2][3]->setState(Peca::Filled);
    m_pecas[3][1]->setState(Peca::Filled);
    m_pecas[3][2]->setState(Peca::Filled);
    m_pecas[3][3]->setState(Peca::Filled);
    m_pecas[3][4]->setState(Peca::Filled);
    m_pecas[3][5]->setState(Peca::Filled);
    m_pecas[4][3]->setState(Peca::Filled);
    m_pecas[5][3]->setState(Peca::Filled);

}

void RestaUm::Nyanficar() {
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c] && m_pecas[r][c]->getState() == Peca::Filled)
               m_pecas[r][c]->setIcon(QPixmap(":/nyan_filled"));
            if(m_pecas[r][c] && m_pecas[r][c]->getState() == Peca::Empty) {
                m_pecas[r][c]->setIcon(QPixmap(":/nyan_empty"));
            }
            if(m_pecas[r][c] && m_pecas[r][c]->getState() == Peca::Selected) {
                m_pecas[r][c]->setIcon(QPixmap(":/nyan_selected"));
            }
            if(m_pecas[r][c] && m_pecas[r][c]->getState() == Peca::Jumpable) {
                m_pecas[r][c]->setIcon(QPixmap(":/nyan_jumpable"));
            }
        }
    }
}

void RestaUm::Desnyanficar() {
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c] && m_pecas[r][c]->getState() == Peca::Filled)
               m_pecas[r][c]->setIcon(QPixmap(":/filled"));
            if(m_pecas[r][c] && m_pecas[r][c]->getState() == Peca::Empty) {
                m_pecas[r][c]->setIcon(QPixmap(":/empty"));
            }
            if(m_pecas[r][c] && m_pecas[r][c]->getState() == Peca::Selected) {
                m_pecas[r][c]->setIcon(QPixmap(":/selected"));
            }
            if(m_pecas[r][c] && m_pecas[r][c]->getState() == Peca::Jumpable) {
                m_pecas[r][c]->setIcon(QPixmap(":/jumpable"));
            }
        }
    }
}

void RestaUm::Banquinho(){
    nPecas = 11;
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c])
                m_pecas[r][c]->setState(Peca::Empty);
        }
    }

    m_pecas[0][2]->setState(Peca::Filled);
    m_pecas[0][3]->setState(Peca::Filled);
    m_pecas[0][4]->setState(Peca::Filled);
    m_pecas[1][2]->setState(Peca::Filled);
    m_pecas[1][3]->setState(Peca::Filled);
    m_pecas[1][4]->setState(Peca::Filled);
    m_pecas[2][2]->setState(Peca::Filled);
    m_pecas[2][3]->setState(Peca::Filled);
    m_pecas[2][4]->setState(Peca::Filled);
    m_pecas[3][2]->setState(Peca::Filled);
    m_pecas[3][4]->setState(Peca::Filled);
}

void RestaUm::Flecha() {
    nPecas = 17;
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c])
                m_pecas[r][c]->setState(Peca::Empty);
        }
    }

    m_pecas[0][3]->setState(Peca::Filled);
    m_pecas[1][2]->setState(Peca::Filled);
    m_pecas[1][3]->setState(Peca::Filled);
    m_pecas[1][4]->setState(Peca::Filled);
    m_pecas[2][1]->setState(Peca::Filled);
    m_pecas[2][2]->setState(Peca::Filled);
    m_pecas[2][3]->setState(Peca::Filled);
    m_pecas[2][4]->setState(Peca::Filled);
    m_pecas[2][5]->setState(Peca::Filled);
    m_pecas[3][3]->setState(Peca::Filled);
    m_pecas[4][3]->setState(Peca::Filled);
    m_pecas[5][2]->setState(Peca::Filled);
    m_pecas[5][3]->setState(Peca::Filled);
    m_pecas[5][4]->setState(Peca::Filled);
    m_pecas[6][2]->setState(Peca::Filled);
    m_pecas[6][3]->setState(Peca::Filled);
    m_pecas[6][4]->setState(Peca::Filled);

}

void RestaUm::Piramide() {
    nPecas = 16;
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c])
                m_pecas[r][c]->setState(Peca::Empty);
        }
    }
    m_pecas[1][3]->setState(Peca::Filled);
    m_pecas[2][2]->setState(Peca::Filled);
    m_pecas[2][3]->setState(Peca::Filled);
    m_pecas[2][4]->setState(Peca::Filled);

    m_pecas[3][1]->setState(Peca::Filled);
    m_pecas[3][2]->setState(Peca::Filled);
    m_pecas[3][3]->setState(Peca::Filled);
    m_pecas[3][4]->setState(Peca::Filled);
    m_pecas[3][5]->setState(Peca::Filled);
    m_pecas[4][0]->setState(Peca::Filled);
    m_pecas[4][1]->setState(Peca::Filled);
    m_pecas[4][2]->setState(Peca::Filled);
    m_pecas[4][3]->setState(Peca::Filled);
    m_pecas[4][4]->setState(Peca::Filled);
    m_pecas[4][5]->setState(Peca::Filled);
    m_pecas[4][6]->setState(Peca::Filled);

}

void RestaUm::Losango() {
    nPecas = 24;
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c])
                m_pecas[r][c]->setState(Peca::Empty);
        }
    }
    m_pecas[0][3]->setState(Peca::Filled);
    m_pecas[1][2]->setState(Peca::Filled);
    m_pecas[1][3]->setState(Peca::Filled);
    m_pecas[1][4]->setState(Peca::Filled);
    m_pecas[2][1]->setState(Peca::Filled);
    m_pecas[2][2]->setState(Peca::Filled);
    m_pecas[2][3]->setState(Peca::Filled);
    m_pecas[2][4]->setState(Peca::Filled);
    m_pecas[2][5]->setState(Peca::Filled);
    m_pecas[3][0]->setState(Peca::Filled);
    m_pecas[3][1]->setState(Peca::Filled);
    m_pecas[3][2]->setState(Peca::Filled);
    m_pecas[3][4]->setState(Peca::Filled);
    m_pecas[3][5]->setState(Peca::Filled);
    m_pecas[3][6]->setState(Peca::Filled);
    m_pecas[4][1]->setState(Peca::Filled);
    m_pecas[4][2]->setState(Peca::Filled);
    m_pecas[4][3]->setState(Peca::Filled);
    m_pecas[4][4]->setState(Peca::Filled);
    m_pecas[4][5]->setState(Peca::Filled);
    m_pecas[5][2]->setState(Peca::Filled);
    m_pecas[5][3]->setState(Peca::Filled);
    m_pecas[5][4]->setState(Peca::Filled);
    m_pecas[6][3]->setState(Peca::Filled);
}

void RestaUm::Tradicional() {
    setNpecas(32);
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c])
                m_pecas[r][c]->setState(Peca::Filled);
        }
    }
    m_pecas[3][3]->setState(Peca::Empty);
}

int RestaUm::getNpecas() {
    return nPecas;
}

void RestaUm::setNpecas(int nPecas) {
    this->nPecas = nPecas;
}

void RestaUm::atualizarStatusBar(){
    ui->statusBar->showMessage("Peças Remanescentes: " + QString::number(getNpecas()));

}

void RestaUm::JogoNovo() {
    nyanAtivado = 0;
    if (ui->actionTradicional->isChecked())
        Tradicional();
    else if (ui->actionCruz->isChecked())
        Cruz();
    else if (ui->actionMais->isChecked())
        Mais();
    else if (ui->actionBanquinho->isChecked())
        Banquinho();
    else if (ui->actionFlecha->isChecked())
        Flecha();
    else if (ui->actionPiramide->isChecked())
        Piramide();
    else if (ui->actionLosango->isChecked())
        Losango();
    Desnyanficar();
    atualizarStatusBar();
}

bool RestaUm::verificaSeHaJogadas() {
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if(m_pecas[r][c]) {
                if((r-2) >= 0 && m_pecas[r-1][c] && m_pecas[r-2][c]
                        && m_pecas[r-1][c]->getState() == Peca::Filled
                        && m_pecas[r-2][c]->getState() == Peca::Empty && m_pecas[r][c]->getState() == Peca::Filled){
                    return true;
                }

                if((r+2) <= 6 && m_pecas[r+1][c] && m_pecas[r+2][c]
                        && m_pecas[r+1][c]->getState() == Peca::Filled
                        && m_pecas[r+2][c]->getState() == Peca::Empty && m_pecas[r][c]->getState() == Peca::Filled){
                    return true;
                }

                if((c-2) >= 0 && m_pecas[r][c-1] && m_pecas[r][c-2]
                        && m_pecas[r][c-1]->getState() == Peca::Filled
                        && m_pecas[r][c-2]->getState() == Peca::Empty && m_pecas[r][c]->getState() == Peca::Filled){
                    return true;
                }

                if((c+2) <= 6 && m_pecas[r][c+1] && m_pecas[r][c+2]
                        && m_pecas[r][c+1]->getState() == Peca::Filled
                        && m_pecas[r][c+2]->getState() == Peca::Empty && m_pecas[r][c]->getState() == Peca::Filled){
                    return true;
                }
            }
        }
    }
    return false;
}
