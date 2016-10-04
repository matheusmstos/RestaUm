#include "RestaUm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RestaUm w;
    w.show();

    return a.exec();
}
