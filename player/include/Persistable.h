#ifndef PERSISTABLE_H
#define PERSISTABLE_H

#include <QMetaObject>
#include <QMetaProperty>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class Persistable : public QObject {
    Q_OBJECT
    Q_PROPERTY(long long id READ id WRITE setId)
public:
    Persistable()
        : _id(-1)
    {}

    virtual ~Persistable() {}

    virtual QString table() const = 0;

    long long id() const { return _id; }
    void setId(long long val) { _id = val; }
private:
    long long _id;
};

class TestPersistable : public Persistable {
    Q_OBJECT
    Q_PROPERTY(int test READ test WRITE setTest)
    Q_PROPERTY(QString name READ name WRITE setName)
public:
    TestPersistable() : Persistable() {}
    virtual ~TestPersistable() override {}

    int test() const { return _test; }
    void setTest(int val) { _test = val; }
    QString name() const { return _name; }
    void setName(const QString &name) { _name = name; }

    virtual QString table() const override { return "tests"; }
private:
    QString _name;
    int _test;
};

#endif // PERSISTABLE_H
