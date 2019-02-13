#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "Persistable.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>


class Repository {
public:
    bool store(Persistable* object);

    template <typename T>
    T* fetch(long long id) const {
        char const* const SELECT_QUERY = "SELECT * FROM %1 WHERE id = :id";
        T* newObject = new T();

        QSqlQuery query;
        if (!query.prepare(QString(SELECT_QUERY).arg(newObject->table()))) {
            qDebug() << query.lastError();
            return nullptr; // FIXME: memory leak
        }

        // Bind ID
        query.bindValue(":id", id);

        if (!query.exec()) {
            qDebug() << query.lastError();
            return nullptr; // FIXME: memory leak
        }

        // Fill newObject with values from query
        if (!query.first()) {
            qDebug() << "No rows returned for " << newObject->table() << "(" << id << ")";
            return nullptr; // FIXME: memory leak
        }

        auto const record = query.record();
        auto const metaobj = newObject->metaObject();
        for (int index = metaobj->propertyOffset(); index < metaobj->propertyCount(); index++) {
            auto prop = metaobj->property(index);
            prop.write(newObject, record.value(prop.name()));
        }

        return newObject;
    }

private:
    bool insert(Persistable* object);
    bool update(Persistable* object);
};

#endif // REPOSITORY_H
