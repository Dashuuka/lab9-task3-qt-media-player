#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <QUuid>
#include <QQmlEngine>

class SettingsManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int launchCount READ launchCount NOTIFY settingsChanged)
    Q_PROPERTY(QString appUuid READ appUuid NOTIFY settingsChanged)

    Q_PROPERTY(int totalAttempts READ totalAttempts NOTIFY settingsChanged)
    Q_PROPERTY(int totalScore READ totalScore NOTIFY settingsChanged)
    Q_PROPERTY(int bestScore READ bestScore NOTIFY settingsChanged)
    Q_PROPERTY(int gamesPlayed READ gamesPlayed NOTIFY settingsChanged)

    Q_PROPERTY(QString currentLocale READ currentLocale WRITE setCurrentLocale NOTIFY settingsChanged)

public:
    explicit SettingsManager(QObject *parent = nullptr);

    int launchCount() const;
    QString appUuid() const;
    void incrementLaunchCount();

    int totalAttempts() const;
    int totalScore() const;
    int bestScore() const;
    int gamesPlayed() const;

    QString currentLocale() const;
    void setCurrentLocale(const QString &locale);

    Q_INVOKABLE void saveGameResult(int attempts, int score);
    Q_INVOKABLE void resetStats();
    Q_INVOKABLE QString formattedSettings() const;
    Q_INVOKABLE QVariantList historyJson() const;
    Q_INVOKABLE void appendHistory(int secret, int attempts, int score, bool won);

signals:
    void settingsChanged();

private:
    QSettings m_settings;
    void ensureUuid();
};

#endif
