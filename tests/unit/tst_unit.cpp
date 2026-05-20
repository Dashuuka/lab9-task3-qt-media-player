#include <QtTest/QtTest>
#include <QGuiApplication>
#include "gamemodel.h"
#include "settingsmanager.h"
#include "platforminfo.h"
#include "localizer.h"

class TestUnit : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        // Use a private QSettings scope so tests don't pollute real settings.
        QCoreApplication::setOrganizationName("Lab9Task3Test");
        QCoreApplication::setApplicationName("UnitTests");
    }

    // 1) Initial state invariants
    void testInitialState()
    {
        GameModel m;
        QVERIFY(m.secretNumber() >= 0 && m.secretNumber() < 100);
        QCOMPARE(m.currentAttempts(), 0);
        QCOMPARE(m.maxAttempts(), 10);
        QVERIFY(!m.isGameFinished());
        QCOMPARE(m.calculateScore(), 0);
    }

    // 2) Deterministic via seed
    void testSeededReproducible()
    {
        GameModel a; a.seed(42);
        GameModel b; b.seed(42);
        QCOMPARE(a.secretNumber(), b.secretNumber());
    }

    // 3) Correct guess flow
    void testCorrectGuess()
    {
        GameModel m; m.seed(123);
        const int secret = m.secretNumber();
        const auto r = m.makeGuess(secret);
        QCOMPARE(r, GameModel::Correct);
        QVERIFY(m.isGameFinished());
        QCOMPARE(m.currentAttempts(), 1);
        QCOMPARE(m.calculateScore(), 100); // (10-1+1)*10
    }

    // 4) TooLow / TooHigh
    void testHints()
    {
        GameModel m; m.seed(5);
        const int s = m.secretNumber();
        if (s > 0) {
            QCOMPARE(m.makeGuess(s - 1), GameModel::TooLow);
        }
        m.startNewGame();
        const int s2 = m.secretNumber();
        if (s2 < 99) {
            QCOMPARE(m.makeGuess(s2 + 1), GameModel::TooHigh);
        }
    }

    // 5) Out-of-range and invalid input throw
    void testExceptions()
    {
        GameModel m;
        QVERIFY_THROWS_EXCEPTION(std::out_of_range, m.makeGuess(-1));
        QVERIFY_THROWS_EXCEPTION(std::out_of_range, m.makeGuess(100));
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, GameModel::parseInput("abc"));
        QVERIFY_THROWS_EXCEPTION(std::invalid_argument, GameModel::parseInput(""));
        QVERIFY_THROWS_EXCEPTION(std::out_of_range, m.setMaxAttempts(0));
        QVERIFY_THROWS_EXCEPTION(std::out_of_range, m.setMaxAttempts(101));
    }

    // 6) Game over after running out of attempts
    void testGameOver()
    {
        GameModel m; m.seed(7);
        const int s = m.secretNumber();
        const int wrong = (s == 0) ? 1 : 0;
        for (int i = 0; i < 9; ++i) m.makeGuess(wrong);
        QVERIFY(!m.isGameFinished());
        QCOMPARE(m.makeGuess(wrong), GameModel::GameOver);
        QVERIFY(m.isGameFinished());
        QCOMPARE(m.calculateScore(), 0);
    }

    // 7) PlatformInfo always exposes a known target
    void testPlatformInfo()
    {
        PlatformInfo p;
        QVERIFY(!p.name().isEmpty());
        const int trueCount = int(p.isAndroid()) + int(p.isIos())
                              + int(p.isLinux()) + int(p.isWeb());
        QCOMPARE(trueCount, 1);
        QCOMPARE(p.webSkin(360), QStringLiteral("android"));
        QCOMPARE(p.webSkin(1024), QStringLiteral("linux"));
    }

    // 8) Localizer accepts known locales, rejects unknown
    void testLocalizer()
    {
        Localizer l;
        QVERIFY(l.supportedLocales().contains("en"));
        QVERIFY(l.supportedLocales().contains("ru"));
        QVERIFY(l.supportedLocales().contains("be"));
        QVERIFY(!l.setLocale("xx"));         // unknown
        QVERIFY(l.setLocale("en"));          // english always loads (source)
        QCOMPARE(l.currentLocale(), QString("en"));
    }
};

QTEST_MAIN(TestUnit)
#include "tst_unit.moc"
