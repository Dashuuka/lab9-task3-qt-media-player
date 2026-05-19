#pragma once

#include "filesystemscanner.h"
#include "playlistmodel.h"
#include "playlistrepository.h"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(bool playing READ playing NOTIFY playingChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
    explicit AppController(PlaylistModel *model, QObject *parent = nullptr);

    qint64 position() const;
    qint64 duration() const;
    bool playing() const;
    int currentIndex() const;
    QString errorMessage() const;

    Q_INVOKABLE void scanFolder(const QUrl &folderUrl);
    Q_INVOKABLE void addFile(const QUrl &fileUrl);
    Q_INVOKABLE void playIndex(int index);
    Q_INVOKABLE void togglePlayback();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void next();
    Q_INVOKABLE void previous();
    Q_INVOKABLE void seek(qint64 positionMs);
    Q_INVOKABLE void removeAt(int index);
    Q_INVOKABLE QString formatTime(qint64 value) const;

signals:
    void positionChanged();
    void durationChanged();
    void playingChanged();
    void currentIndexChanged();
    void errorMessageChanged();

private:
    void setError(const QString &message);
    void persist();

    PlaylistModel *m_model;
    FileSystemScanner m_scanner;
    PlaylistRepository m_repository;
    QMediaPlayer m_player;
    QAudioOutput m_audio;
    int m_currentIndex = -1;
    QString m_errorMessage;
};
