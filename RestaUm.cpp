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
          }
    }
}
    desenharTabuleiro();

    QObject::connect(
        group,
        SIGNAL(triggered(QAction*)),
        this,
        SLOT(trocarModo(QAction*)));

    QObject::connect(
                ui->actionNovo,
                SIGNAL(triggered()),
                qApp,
                SLOT(novoJogo()));

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
        SIGNAL(gameOver(int)),
        this,
        SLOT(mostrarFimJogo(int)));

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

    if(movimentoPossivel(r, c - 1)) {

    }
    if(movimentoPossivel(r, c + 1)) {

    }
    if(movimentoPossivel(r, r - 1)) {

    }
    if(movimentoPossivel(r, r + 1)) {

    }


    if(m_pecas[r][c-1]->getState() == Peca::State::Filled) {


    }

//    if (peca == ui->peca54) {
//        emit gameOver(1);
//    } else {
//        m_pecas[3][3]->setState(Peca::Filled);
//        peca->setState(Peca::Empty);
//    }

}

bool RestaUm::movimentoPossivel(int r, int c) {
            if(c < 0 || c > 7 || r < 0 || r > 7)
                return false;

}

void RestaUm::mostrarSobre() {
    QMessageBox::information(this,
       tr("Sobre"),
       tr("Resta Um\n\nAndrei Rimsa Alvares - andrei@decom.cefetmg.br"));
}

void RestaUm::mostrarFimJogo(int n) {
    if (n == 1) {
        QMessageBox::information(this,
           tr("Fim"),
           tr("Parabéns, você venceu!"));
    } else {
        QMessageBox::information(this,
           tr("Fim"),
           tr("Parabéns, você perdeu!"));
    }
}


void RestaUm::trocarModo(QAction* modo) {
    if (modo == ui->actionTradicional)
        qDebug() << "modo: tradicional";
    else if (modo == ui->actionCruz)
        qDebug() << "modo: cruz";
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

void RestaUm::desenharTabuleiro() {
    for (int r = 0; r < 7; r++) {
        for (int c = 0; c < 7; c++) {
            if (m_pecas[r][c]) {
                m_pecas[r][c]->setState(Peca::Filled);
            }
        }
    }

    m_pecas[3][3]->setState(Peca::Empty);
}
