#include "RestaUm.h"
#include "ui_restaum.h"

#include <QMessageBox>
#include <QDebug>
#include <QActionGroup>

RestaUm::RestaUm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RestaUm)
{
    ui->setupUi(this);

    QActionGroup* group =
            new QActionGroup(this);
       group->addAction(ui->actionTradicional);
       group->addAction(ui->actionBanquinho);
       group->addAction(ui->actionFlecha);
       group->addAction(ui->actionLosango);
       group->addAction(ui->actionCruz);
       group->addAction(ui->actionPiramide);
       group->addAction(ui->actionMais);


    QObject::connect(ui->actionSair,
                     SIGNAL(triggered()),
                     qApp, SLOT(quit()));

    QObject::connect(ui->actionSobre,
                     SIGNAL(triggered()),
                     this,
                     SLOT(mostrarSobre()));

    QObject::connect(group,
                     SIGNAL(triggered(QAction*)),
                     this,
                     SLOT(trocarModo(QAction*)));
}

RestaUm::~RestaUm()
{
    delete ui;
}

void RestaUm::trocarModo(QAction *acao)
{
    if(acao == ui->actionTradicional){
        qDebug() << "Modo Tradicional";
    }else if(acao == ui->actionBanquinho){
        qDebug() << "Modo Banquinho";
    }else if(acao == ui->actionCruz){
        qDebug() << "Modo Cruz";
    }else if(acao == ui->actionFlecha){
        qDebug() << "Modo Flecha";
    }else if(acao == ui->actionLosango){
        qDebug() << "Modo Losango";
    }else if(acao == ui->actionPiramide){
        qDebug() << "Modo Piramide";
    }else if(acao == ui->actionMais){
        qDebug() << "Modo Mais";
    }
}

void RestaUm::mostrarSobre()
{
    QMessageBox::information(this, tr("Sobre"), tr("Estamos em obra"));
}
