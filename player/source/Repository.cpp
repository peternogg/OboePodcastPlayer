#include "Repository.h"

static char const* const INSERT_QUERY = "INSERT INTO %1(%2) VALUES (%3)";
static char const* const UPDATE_QUERY = "UPDATE %1 SET %2 WHERE id = :id";
static char const* const SETTINGS_UPDATE_QUERY = "UPDATE Settings SET value = :value WHERE name = :name";
static char const* const SETTINGS_FETCH_QUERY = "SELECT value FROM Settings WHERE name = :name";

bool Repository::store(Persistable *object) {
    if (object->id() < 0)
        return insert(object);
    else
        return update(object);
}

bool Repository::storeSetting(const QString &name, const QVariant &value) {
    QSqlQuery query;
    if (!query.prepare(SETTINGS_UPDATE_QUERY)) {
        qDebug() << query.lastError();
        return false;
    }

    query.bindValue(":name", name);
    query.bindValue(":value", value);

    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}

QVariant Repository::fetchSetting(QString const& name) {
    QSqlQuery query;
    if (!query.prepare(SETTINGS_FETCH_QUERY)) {
        qDebug() << query.lastError();
        return {};
    }

    query.bindValue(":name", name);

    if (!query.exec()) {
        qDebug() << query.lastError();
        return {};
    }

    if (!query.first()) {
        qDebug() << query.lastError();
        return {};
    }

    return query.value(0);
}

bool Repository::insert(Persistable *object) {
    auto const metaobj = object->metaObject();
    QString names, params;
    // Build column name list and placeholders for query
    // They need to be built as two separate lists because they're used in two places
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

    return true;
}

bool Repository::update(Persistable *object) {
    auto const metaobj = object->metaObject();
    QString columnSpec;

    for (int index = metaobj->propertyOffset(); index < metaobj->propertyCount() - 1; index++) {
        auto const name = metaobj->property(index).name();
        columnSpec.append(QString("%1 = :%1, ").arg(name));
    }

    columnSpec.append(QString("%1 = :%1").arg(
                          metaobj->property(metaobj->propertyCount() - 1).name()));

    QSqlQuery query;
    if (!query.prepare(QString(UPDATE_QUERY).arg(object->table(), columnSpec))) {
        qDebug() << query.lastError();
        return false;
    }
    for (int index = metaobj->propertyOffset(); index < metaobj->propertyCount(); index++) {
        auto const prop = metaobj->property(index);
        query.bindValue(QString(":%1").arg(prop.name()), prop.read(object).toString());
    }

    query.bindValue(":id", object->id());

    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }

    return true;
}
