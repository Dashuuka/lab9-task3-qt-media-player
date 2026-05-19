#include "playlistrepository.h"
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QUuid>

PlaylistRepository::PlaylistRepository(const QString &databasePath, QObject *parent)
    : QObject(parent),
      m_databasePath(databasePath),
      m_connectionName("playlist_" + QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

PlaylistRepository::~PlaylistRepository()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool PlaylistRepository::open()
{
    if (m_db.isOpen()) {
        return true;
    }

    if (m_databasePath.isEmpty()) {
        QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(base);
        m_databasePath = base + "/playlist.sqlite";
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    m_db.setDatabaseName(m_databasePath);
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        qWarning("[Repository] SQLite open error");
        return false;
    }

    QSqlQuery query(m_db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS playlist (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT NOT NULL, file_path TEXT NOT NULL, url TEXT NOT NULL, duration_ms INTEGER NOT NULL DEFAULT 0)")) {
        m_lastError = query.lastError().text();
        qWarning("[Repository] SQLite schema error");
        return false;
    }
    return true;
}

bool PlaylistRepository::save(const QList<AudioTrack> &tracks)
{
    if (!open()) {
        return false;
    }
    QSqlQuery clear(m_db);
    if (!clear.exec("DELETE FROM playlist")) {
        m_lastError = clear.lastError().text();
        qWarning("[Repository] SQLite clear error");
        return false;
    }

    QSqlQuery insert(m_db);
    insert.prepare("INSERT INTO playlist(title, file_path, url, duration_ms) VALUES(?, ?, ?, ?)");
    for (const AudioTrack &track : tracks) {
        insert.addBindValue(track.title);
        insert.addBindValue(track.filePath);
        insert.addBindValue(track.url.toString());
        insert.addBindValue(track.durationMs);
        if (!insert.exec()) {
            m_lastError = insert.lastError().text();
            qWarning("[Repository] SQLite insert error");
            return false;
        }
    }
    return true;
}

QList<AudioTrack> PlaylistRepository::load()
{
    QList<AudioTrack> tracks;
    if (!open()) {
        return tracks;
    }
    QSqlQuery query("SELECT title, file_path, url, duration_ms FROM playlist ORDER BY id", m_db);
    while (query.next()) {
        tracks.append(AudioTrack{
            query.value(0).toString(),
            query.value(1).toString(),
            QUrl(query.value(2).toString()),
            query.value(3).toLongLong()
        });
    }
    if (query.lastError().isValid()) {
        m_lastError = query.lastError().text();
        qWarning("[Repository] SQLite read error");
    }
    return tracks;
}

QString PlaylistRepository::lastError() const
{
    return m_lastError;
}
