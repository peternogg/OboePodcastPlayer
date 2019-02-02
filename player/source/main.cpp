#include "OboeWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OboeWindow w;
    w.show();

    return a.exec();
}
