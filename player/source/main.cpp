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

    pers.setName("An updated name!");
    resp.store(&pers);

    auto* pastResp = resp.fetch<TestPersistable>(3);
    qDebug() << "Fetched name is: " << pastResp->name();

    return a.exec();
}
