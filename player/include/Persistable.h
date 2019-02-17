#ifndef PERSISTABLE_H
#define PERSISTABLE_H

#include <QMetaObject>
#include <QMetaProperty>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

class Persistable : public QObject {
    Q_OBJECT
    Q_PROPERTY(long long id READ id WRITE setId)
public:
    Persistable(QObject* parent = nullptr)
        : QObject(parent), _id(-1)
    {}

    virtual ~Persistable() {}

    virtual QString table() const = 0;

    long long id() const { return _id; }
    void setId(long long val) { _id = val; }
private:
    long long _id;
};

#endif // PERSISTABLE_H
