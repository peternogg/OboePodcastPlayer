#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

#include "Persistable.h"
#include "Podcast.h"

class Repository {
public:
    bool store(Persistable* object);

    template <typename T>
    std::vector<T*> fetchAll() const {
        char const* const SELECT_QUERY = "SELECT * FROM %1";
        std::vector<T*> objects;
        objects.push_back(new T());

        QSqlQuery query;
        if (!query.prepare(QString(SELECT_QUERY).arg(objects.back()->table()))) {
            qDebug() << query.lastError();
            throw std::exception(); // FIXME: find something better
        }

        if (!query.exec()) {
            qDebug() << query.lastError();
            throw std::exception();
        }

        if (!query.first()) {
            if (auto err = query.lastError(); err.isValid()) {
                qDebug() << err;
                throw std::exception();
            }
            return {}; // Nothing in the search set
        }

        do {
            fillObject(objects.back(), query.record());
            objects.push_back(new T());
        } while(query.next());

        // Clean up the extra object from the vector
        delete objects.back();
        objects.pop_back();

        return objects;
    }

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

        fillObject(newObject, query.record());

        return newObject;
    }

    template <typename Iter>
    bool storeAll(Iter begin, Iter end) {
        bool storeOk = true;
        for (; storeOk && begin != end; begin++) {
            storeOk &= store(*begin);
        }

        return storeOk;
    }

private:
    bool insert(Persistable* object);
    bool update(Persistable* object);

    template <typename T>
    void fillObject(T* object, QSqlRecord record) const {
        auto const metaobj = object->metaObject();
        for (int index = metaobj->propertyOffset() - 1; index < metaobj->propertyCount(); index++) {
            auto prop = metaobj->property(index);
            prop.write(object, record.value(prop.name()));
        }
    }
};

#endif // REPOSITORY_H
