#include "include/Repository.h"

static char const* const INSERT_QUERY = "INSERT INTO %1(%2) VALUES (%3)";
static char const* const UPDATE_QUERY = "UPDATE %1 SET %2";

bool Repository::store(Persistable *object) {
    if (object->id() < 0)
        return insert(object);
    else
        return update(object);
}

bool Repository::insert(Persistable *object) {
    auto const metaobj = object->metaObject();
    QString names, params;
    // Build column name list and placeholders for query
    for (int index = metaobj->propertyOffset(); index < metaobj->propertyCount() - 1; index++) {
        auto name = metaobj->property(index).name();
        names.append(name).append(", ");
        params.append(':').append(name).append(", ");
    }

    names.append(metaobj->property(metaobj->propertyCount() - 1).name());
    params.append(':').append(metaobj->property(metaobj->propertyCount() - 1).name());


    QSqlQuery query;
    if (!query.prepare(QString(INSERT_QUERY).arg(object->table(), names, params))) {
        qDebug() << query.lastError();
        return false;
    }

    // Fill out prepared query
    for(int index = metaobj->propertyOffset(); index < metaobj->propertyCount(); index++) {
        auto const& prop = metaobj->property(index);
        query.bindValue(QString(":%1").arg(prop.name()), prop.read(object).toString());
    }

    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }

    // Ok!
    object->setId(query.lastInsertId().toInt());
    qDebug() << "New id: " << object->id();

    return true;
}

bool Repository::update(Persistable *object) {
    return false;
}
