#include "OboeWindow.h"
#include <QApplication>
#include <QSqlDatabase>

#include "Persistable.h"
#include "Repository.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OboeWindow w;
    w.show();

    return a.exec();
}
