#include <QtTest>

// add necessary includes here
#include "RSSFeed.h"

class OboeTests : public QObject
{
    Q_OBJECT

public:
    OboeTests();
    ~OboeTests();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

OboeTests::OboeTests()
{

}

OboeTests::~OboeTests()
{

}

void OboeTests::initTestCase()
{

}

void OboeTests::cleanupTestCase()
{

}

void OboeTests::test_case1()
{
    QCOMPARE(1, 1);
}

QTEST_APPLESS_MAIN(OboeTests)

#include "tst_oboetests.moc"
