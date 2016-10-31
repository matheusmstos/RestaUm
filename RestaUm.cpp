#include "RestaUm.h"
#include "ui_RestaUm.h"

#include <QDebug>
#include <QActionGroup>
#include <QMessageBox>

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
            }
            else if(minha_lista.size() > 1) {
                this->newPeca = peca;
                newPeca->setState(Peca::Selected);
                foreach (Peca *p, minha_lista) {
                    p->setState(Peca::Jumpable);
                }

                estado = 2;
            }
            break;

        case 2:

                if(minha_lista.contains(peca)) {
                    exMoves(newPeca->getX(), newPeca->getY(), peca->getX(), peca->getY());
                }

                foreach (Peca *p, minha_lista) {
                    if(p->getState() == Peca::Jumpable){
                        p->setState(Peca::Empty);
                    }

                }
                   estado = 1;
            break;

        default:
            break;
    }

}

void RestaUm::srMoves(int r, int c){
    if((r-2) >= 0 && m_pecas[r-1][c] && m_pecas[r-2][c]
            && m_pecas[r-1][c]->getState() == Peca::Filled
            && m_pecas[r-2][c]->getState() == Peca::Empty){
        minha_lista << m_pecas[r-2][c];
    }

    if((r+2) <= 6 && m_pecas[r+1][c] && m_pecas[r+2][c]
            && m_pecas[r+1][c]->getState() == Peca::Filled
            && m_pecas[r+2][c]->getState() == Peca::Empty){
        minha_lista << m_pecas[r+2][c];
    }

    if((c-2) >= 0 && m_pecas[r][c-1] && m_pecas[r][c-2]
            && m_pecas[r][c-1]->getState() == Peca::Filled
            && m_pecas[r][c-2]->getState() == Peca::Empty){
        minha_lista << m_pecas[r][c-2];
    }

    if((c+2) <= 6 && m_pecas[r][c+1] && m_pecas[r][c+2]
            && m_pecas[r][c+1]->getState() == Peca::Filled
            && m_pecas[r][c+2]->getState() == Peca::Empty){
        minha_lista << m_pecas[r][c+2];
    }

}

void RestaUm::exMoves(int rp, int cp, int rl, int cl){
        m_pecas[rl][cl]->setState(Peca::Filled);
        m_pecas[(rl+rp)/2][(cl+cp)/2]->setState(Peca::Empty);
        m_pecas[rp][cp]->setState(Peca::Empty);

}

void RestaUm::mostrarSobre() {
    QMessageBox::information(this,
       tr("Sobre"),
       tr("Resta Um\n\nAndrei Rimsa Alvares - andrei@decom.cefetmg.br"));
}

void RestaUm::mostrarFimJogo() {
    QMessageBox::information(this,
       tr("Fim"),
       tr("Parabéns, você venceu!"));
}


void RestaUm::trocarModo(QAction* modo) {
    if (modo == ui->actionTradicional)
        qDebug() << "modo: tradicional";
    else if (modo == ui->actionCruz){
        qDebug() << "modo: cruz";
        Cruz();
    }
    else if (modo == ui->actionMais)
        qDebug() << "modo: mais";
    else if (modo == ui->actionBanquinho)
        qDebug() << "modo: banquinho";
    else if (modo == ui->actionFlecha)
        qDebug() << "modo: flecha";
    else if (modo == ui->actionPiramide)
        qDebug() << "modo: piramide";
    else if (modo == ui->actionLosango)
        qDebug() << "modo: losango";
}

void RestaUm::Cruz(){
    qDebug() << "Que saco";
        for (int r = 0; r < 7; r++) {
            for (int c = 0; c < 7; c++) {
                if(m_pecas[r][c])
                m_pecas[r][c]->setState(Peca::Empty);
            }
        }
        qDebug() << "ola amigos";

        m_pecas[1][3]->setState(Peca::Filled);
        m_pecas[2][3]->setState(Peca::Filled);
        m_pecas[3][3]->setState(Peca::Filled);
        m_pecas[4][3]->setState(Peca::Filled);
        m_pecas[2][2]->setState(Peca::Filled);
        m_pecas[2][4]->setState(Peca::Filled);
}

int RestaUm::getNpecas() {
    return nPecas;
}

void RestaUm::setNpecas(int nPecas) {
    this->nPecas = nPecas;
}
