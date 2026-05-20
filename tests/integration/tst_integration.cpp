#include <QtTest/QtTest>
#include <QGuiApplication>
#include <QSignalSpy>
#include "gamemodel.h"
#include "gameviewmodel.h"
#include "settingsmanager.h"
#include "drophandler.h"

class TestIntegration : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QCoreApplication::setOrganizationName("Lab9Task3Test");
        QCoreApplication::setApplicationName("IntegrationTests");
        // Wipe any prior state for repeatable runs
        QSettings s("Lab9Task3", "GuessNumberX");
        s.clear();
    }

    // 1) Full win flow: VM + Settings update + history append
    void testWinFlowUpdatesEverything()
    {
        SettingsManager sm;
        GameViewModel vm;
        vm.setSettingsManager(&sm);
        vm.seed(99);
        const int games0 = sm.gamesPlayed();
        const int score0 = sm.totalScore();
        const int best0  = sm.bestScore();
        const int secret = vm.secretNumber();

        QSignalSpy statsSpy(&vm, &GameViewModel::statsChanged);
        QSignalSpy dialogSpy(&vm, &GameViewModel::showResultDialog);

        vm.submitGuessInt(secret);

        QVERIFY(vm.gameFinished());
        QVERIFY(vm.gameWon());
        QCOMPARE(sm.gamesPlayed(), games0 + 1);
        QVERIFY(sm.totalScore() > score0);
        QVERIFY(sm.bestScore() >= best0);
        QVERIFY(dialogSpy.count() >= 1);
        QVERIFY(statsSpy.count() >= 1);
    }

    // 2) Losing flow saves score=0 but still appends history
    void testLossFlowAppendsHistory()
    {
        SettingsManager sm;
        sm.resetStats();
        GameViewModel vm;
        vm.setSettingsManager(&sm);
        vm.setMaxAttempts(3);
        vm.seed(50);
        const int secret = vm.secretNumber();
        const int wrong = (secret == 0) ? 1 : 0;
        for (int i = 0; i < 3; ++i) vm.submitGuessInt(wrong);
        QVERIFY(vm.gameFinished());
        QVERIFY(!vm.gameWon());
        QCOMPARE(sm.gamesPlayed(), 1);
        const auto hist = sm.historyJson();
        QCOMPARE(hist.size(), 1);
        const auto first = hist.first().toMap();
        QCOMPARE(first.value("won").toBool(), false);
        QCOMPARE(first.value("secret").toInt(), secret);
    }

    // 3) Invalid input emits inputError without crashing
    void testInvalidInputEmitsError()
    {
        SettingsManager sm;
        GameViewModel vm;
        vm.setSettingsManager(&sm);
        QSignalSpy errSpy(&vm, &GameViewModel::inputError);
        vm.submitGuess("not a number");
        QCOMPARE(errSpy.count(), 1);
        QVERIFY(!vm.gameFinished());
    }

    // 4) Out-of-range input via int path emits error too
    void testOutOfRangeInt()
    {
        SettingsManager sm;
        GameViewModel vm;
        vm.setSettingsManager(&sm);
        QSignalSpy errSpy(&vm, &GameViewModel::inputError);
        vm.submitGuessInt(150);
        QCOMPARE(errSpy.count(), 1);
        vm.submitGuessInt(-1);
        QCOMPARE(errSpy.count(), 2);
    }

    // 5) DropHandler extracts seed correctly
    void testDropHandlerSeed()
    {
        DropHandler h;
        QSignalSpy dropped(&h, &DropHandler::fileDropped);
        QSignalSpy errs(&h, &DropHandler::dropError);

        h.processUrl("file:///tmp/seed_42.txt");
        QCOMPARE(dropped.count(), 1);
        QCOMPARE(dropped.first().at(1).toInt(), 42);

        h.processUrl("file:///tmp/no_digits.txt");
        QCOMPARE(errs.count(), 1);

        h.processUrl("");
        QCOMPARE(errs.count(), 2);
    }

    // 6) Settings persistence round-trip
    void testSettingsPersistRoundtrip()
    {
        {
            SettingsManager sm1;
            sm1.resetStats();
            sm1.saveGameResult(5, 60);
            QCOMPARE(sm1.gamesPlayed(), 1);
            QCOMPARE(sm1.totalAttempts(), 5);
            QCOMPARE(sm1.totalScore(), 60);
            QCOMPARE(sm1.bestScore(), 60);
        }
        {
            SettingsManager sm2;
            QCOMPARE(sm2.gamesPlayed(), 1);
            QCOMPARE(sm2.totalScore(), 60);
            QCOMPARE(sm2.bestScore(), 60);
        }
    }

    // 7) Locale persists through settings
    void testLocalePersistence()
    {
        SettingsManager sm;
        sm.setCurrentLocale("ru");
        QCOMPARE(sm.currentLocale(), QString("ru"));
        sm.setCurrentLocale("xx"); // ignored
        QCOMPARE(sm.currentLocale(), QString("ru"));
        sm.setCurrentLocale("en");
        QCOMPARE(sm.currentLocale(), QString("en"));
    }
};

QTEST_MAIN(TestIntegration)
#include "tst_integration.moc"
