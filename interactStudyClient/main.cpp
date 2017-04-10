#include "start.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    start *startWindow = new start();
    startWindow->show();
    a.exec();

    delete startWindow;
    return 0;
}
