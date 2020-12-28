#include "jackwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JackWindow w;
    w.show();
    return a.exec();
}
