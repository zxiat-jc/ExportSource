#include "ExportSource.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ExportSource w;
    w.show();
    return a.exec();
}
