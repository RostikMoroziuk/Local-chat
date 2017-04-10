#include "interactserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    interactServer w;

    return a.exec();
}
