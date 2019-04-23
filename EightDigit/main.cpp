#include "eightdigit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EightDigit w;
    w.show();

    return a.exec();
}
