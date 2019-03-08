#include "OboeWindow.h"
#include <QApplication>
#include <QSqlDatabase>

#include "Persistable.h"
#include "Repository.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    static auto const DATABASE_NAME = "oboe.db";
    // Prep database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DATABASE_NAME);
    db.open();

    OboeWindow w;
    w.show();

    return a.exec();
}
