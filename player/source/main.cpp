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

    // Sample code
    TestPersistable pers;
    pers.setName("Cool Name");
    pers.setTest(414);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("oboe.db");

    if (!db.open()) {
        qDebug() << db.lastError();
    }

    Repository resp;

    resp.store(&pers);

    return a.exec();
}
