#pragma once

#include "audiotrack.h"
#include <QObject>
#include <QSqlDatabase>

class PlaylistRepository : public QObject {
    Q_OBJECT
public:
    explicit PlaylistRepository(const QString &databasePath = QString(), QObject *parent = nullptr);
    ~PlaylistRepository() override;

    bool open();
    bool save(const QList<AudioTrack> &tracks);
    QList<AudioTrack> load();
    QString lastError() const;

private:
    QString m_databasePath;
    QString m_connectionName;
    QString m_lastError;
    QSqlDatabase m_db;
};
