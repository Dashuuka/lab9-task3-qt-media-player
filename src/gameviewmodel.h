#ifndef GAMEVIEWMODEL_H
#define GAMEVIEWMODEL_H

#include <QObject>
#include <QQmlEngine>
#include "gamemodel.h"
#include "settingsmanager.h"

class GameViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString statusText READ statusText NOTIFY statusChanged)
    Q_PROPERTY(QString attemptsText READ attemptsText NOTIFY statusChanged)
    Q_PROPERTY(int currentAttempts READ currentAttempts NOTIFY statusChanged)
    Q_PROPERTY(int maxAttempts READ maxAttempts NOTIFY statusChanged)
    Q_PROPERTY(bool gameFinished READ gameFinished NOTIFY statusChanged)
    Q_PROPERTY(bool gameWon READ gameWon NOTIFY statusChanged)
    Q_PROPERTY(QString imageSource READ imageSource NOTIFY statusChanged)
    Q_PROPERTY(QString animationType READ animationType NOTIFY animationRequested)
    Q_PROPERTY(int secretNumber READ secretNumber NOTIFY statusChanged)

    // Mirrored from SettingsManager
    Q_PROPERTY(int launchCount READ launchCount NOTIFY statsChanged)
    Q_PROPERTY(int gamesPlayed READ gamesPlayed NOTIFY statsChanged)
    Q_PROPERTY(int totalAttempts READ totalAttempts NOTIFY statsChanged)
    Q_PROPERTY(int totalScore READ totalScore NOTIFY statsChanged)
    Q_PROPERTY(int bestScore READ bestScore NOTIFY statsChanged)
    Q_PROPERTY(QString appUuid READ appUuid NOTIFY statsChanged)

public:
    explicit GameViewModel(QObject *parent = nullptr);
    void setSettingsManager(SettingsManager *sm);
    SettingsManager *settingsManager() const { return m_settings; }

    QString statusText() const { return m_statusText; }
    QString attemptsText() const { return m_attemptsText; }
    int currentAttempts() const { return m_model.currentAttempts(); }
    int maxAttempts() const { return m_model.maxAttempts(); }
    bool gameFinished() const { return m_model.isGameFinished(); }
    bool gameWon() const { return m_model.isWon(); }
    QString imageSource() const { return m_imageSource; }
    QString animationType() const { return m_animationType; }
    int secretNumber() const { return m_model.secretNumber(); }

    int launchCount() const;
    int gamesPlayed() const;
    int totalAttempts() const;
    int totalScore() const;
    int bestScore() const;
    QString appUuid() const;

    Q_INVOKABLE void submitGuess(const QString &input);
    Q_INVOKABLE void submitGuessInt(int value);
    Q_INVOKABLE void newGame();
    Q_INVOKABLE void setMaxAttempts(int n);

    // Test hooks
    void seed(quint32 s) { m_model.seed(s); refreshStatus(); }
    GameModel *model() { return &m_model; }

signals:
    void statusChanged();
    void statsChanged();
    void animationRequested();
    void showResultDialog(const QString &title, const QString &message);
    void inputError(const QString &message);

private:
    GameModel m_model;
    SettingsManager *m_settings = nullptr;
    QString m_statusText;
    QString m_attemptsText;
    QString m_imageSource;
    QString m_animationType;

    void updateAttemptsText();
    void refreshStatus();
};

#endif
