#include "settingsmanager.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
    , m_settings(QStringLiteral("Lab9Task3"), QStringLiteral("GuessNumberX"))
{
    ensureUuid();
    incrementLaunchCount();
    qDebug() << "[SettingsManager] Initialized. Launch =" << launchCount()
             << "UUID =" << appUuid();
}

void SettingsManager::ensureUuid()
{
    if (m_settings.value(QStringLiteral("app/uuid")).toString().isEmpty()) {
        m_settings.setValue(QStringLiteral("app/uuid"),
                            QUuid::createUuid().toString(QUuid::WithoutBraces));
    }
}

int SettingsManager::launchCount() const
{
    return m_settings.value(QStringLiteral("app/launchCount"), 0).toInt();
}

QString SettingsManager::appUuid() const
{
    return m_settings.value(QStringLiteral("app/uuid")).toString();
}

void SettingsManager::incrementLaunchCount()
{
    m_settings.setValue(QStringLiteral("app/launchCount"), launchCount() + 1);
    emit settingsChanged();
}

int SettingsManager::totalAttempts() const { return m_settings.value(QStringLiteral("game/totalAttempts"), 0).toInt(); }
int SettingsManager::totalScore() const   { return m_settings.value(QStringLiteral("game/totalScore"), 0).toInt(); }
int SettingsManager::bestScore() const    { return m_settings.value(QStringLiteral("game/bestScore"), 0).toInt(); }
int SettingsManager::gamesPlayed() const  { return m_settings.value(QStringLiteral("game/gamesPlayed"), 0).toInt(); }

QString SettingsManager::currentLocale() const
{
    return m_settings.value(QStringLiteral("app/locale"), QStringLiteral("en")).toString();
}

void SettingsManager::setCurrentLocale(const QString &locale)
{
    if (locale == currentLocale()) return;
    if (locale != QLatin1String("en") &&
        locale != QLatin1String("ru") &&
        locale != QLatin1String("be")) {
        qWarning() << "[SettingsManager] Unsupported locale:" << locale;
        return;
    }
    m_settings.setValue(QStringLiteral("app/locale"), locale);
    emit settingsChanged();
}

void SettingsManager::saveGameResult(int attempts, int score)
{
    try {
        if (attempts < 0) {
            throw std::invalid_argument("Attempts cannot be negative");
        }
        if (score < 0) {
            throw std::invalid_argument("Score cannot be negative");
        }
        m_settings.setValue(QStringLiteral("game/totalAttempts"), totalAttempts() + attempts);
        m_settings.setValue(QStringLiteral("game/totalScore"), totalScore() + score);
        m_settings.setValue(QStringLiteral("game/gamesPlayed"), gamesPlayed() + 1);
        if (score > bestScore()) {
            m_settings.setValue(QStringLiteral("game/bestScore"), score);
        }
        m_settings.sync();
        emit settingsChanged();
        qDebug() << "[SettingsManager] Saved attempts=" << attempts << "score=" << score;
    } catch (const std::exception &e) {
        qCritical() << "[SettingsManager] Error saving game result:" << e.what();
    }
}

void SettingsManager::resetStats()
{
    m_settings.remove(QStringLiteral("game/totalAttempts"));
    m_settings.remove(QStringLiteral("game/totalScore"));
    m_settings.remove(QStringLiteral("game/bestScore"));
    m_settings.remove(QStringLiteral("game/gamesPlayed"));
    m_settings.remove(QStringLiteral("game/history"));
    m_settings.sync();
    emit settingsChanged();
    qDebug() << "[SettingsManager] Stats reset";
}

QString SettingsManager::formattedSettings() const
{
    return QStringLiteral("Launches: %1\nUUID: %2\nGames: %3\nAttempts: %4\nScore: %5\nBest: %6\nLocale: %7")
        .arg(launchCount())
        .arg(appUuid())
        .arg(gamesPlayed())
        .arg(totalAttempts())
        .arg(totalScore())
        .arg(bestScore())
        .arg(currentLocale());
}

QVariantList SettingsManager::historyJson() const
{
    const QString raw = m_settings.value(QStringLiteral("game/history")).toString();
    if (raw.isEmpty()) return {};
    QJsonParseError err;
    const auto doc = QJsonDocument::fromJson(raw.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isArray()) {
        qWarning() << "[SettingsManager] History parse error:" << err.errorString();
        return {};
    }
    QVariantList out;
    out.reserve(doc.array().size());
    for (const auto &v : doc.array()) {
        out.append(v.toObject().toVariantMap());
    }
    return out;
}

void SettingsManager::appendHistory(int secret, int attempts, int score, bool won)
{
    QJsonArray arr;
    const QString raw = m_settings.value(QStringLiteral("game/history")).toString();
    if (!raw.isEmpty()) {
        const auto doc = QJsonDocument::fromJson(raw.toUtf8());
        if (doc.isArray()) arr = doc.array();
    }
    QJsonObject entry;
    entry[QStringLiteral("secret")] = secret;
    entry[QStringLiteral("attempts")] = attempts;
    entry[QStringLiteral("score")] = score;
    entry[QStringLiteral("won")] = won;
    entry[QStringLiteral("ts")] = QDateTime::currentDateTime().toString(Qt::ISODate);
    arr.prepend(entry);
    // keep last 50
    while (arr.size() > 50) arr.removeLast();
    m_settings.setValue(QStringLiteral("game/history"),
                        QString::fromUtf8(QJsonDocument(arr).toJson(QJsonDocument::Compact)));
    m_settings.sync();
    emit settingsChanged();
}
