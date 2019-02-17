#ifndef TESTPERSISTABLE_H
#define TESTPERSISTABLE_H

#include <QObject>
#include "Persistable.h"

class TestPersistable : public Persistable {
    Q_OBJECT
    Q_PROPERTY(int test READ test WRITE setTest)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QDateTime date READ date WRITE setDate)
public:
    TestPersistable() : Persistable() {}
    virtual ~TestPersistable() override {}

    int test() const { return _test; }
    void setTest(int val) { _test = val; }
    QString name() const { return _name; }
    void setName(const QString &name) { _name = name; }
    QDateTime date() { return _date; }
    void setDate(const QDateTime& date) { _date = date; }

    virtual QString table() const override { return "tests"; }
private:
    QString _name;
    QDateTime _date;
    int _test;
};

#endif // TESTPERSISTABLE_H
