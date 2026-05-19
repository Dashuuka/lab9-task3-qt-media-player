#include "appcontroller.h"
#include <QDebug>
#include <QFileInfo>

AppController::AppController(PlaylistModel *model, QObject *parent)
    : QObject(parent), m_model(model)
{
    m_player.setAudioOutput(&m_audio);
    m_audio.setVolume(0.8);
    m_model->setTracks(m_repository.load());

    connect(&m_player, &QMediaPlayer::positionChanged, this, &AppController::positionChanged);
    connect(&m_player, &QMediaPlayer::durationChanged, this, &AppController::durationChanged);
    connect(&m_player, &QMediaPlayer::playbackStateChanged, this, &AppController::playingChanged);
    connect(&m_player, &QMediaPlayer::errorOccurred, this, [this](QMediaPlayer::Error, const QString &message) {
        setError(message.isEmpty() ? tr("Playback error") : message);
    });
    connect(m_model, &PlaylistModel::playlistChanged, this, &AppController::persist);
}

qint64 AppController::position() const { return m_player.position(); }
qint64 AppController::duration() const { return m_player.duration(); }
bool AppController::playing() const { return m_player.playbackState() == QMediaPlayer::PlayingState; }
int AppController::currentIndex() const { return m_currentIndex; }
QString AppController::errorMessage() const { return m_errorMessage; }

void AppController::scanFolder(const QUrl &folderUrl)
{
    const QString path = folderUrl.toLocalFile();
    const QList<AudioTrack> tracks = m_scanner.scanFolder(path);
    if (tracks.isEmpty()) {
        setError(tr("No supported audio files found"));
    }
    for (const AudioTrack &track : tracks) {
        m_model->addTrack(track);
    }
}

void AppController::addFile(const QUrl &fileUrl)
{
    AudioTrack track = m_scanner.trackFromFile(fileUrl.toLocalFile());
    if (!track.isValid()) {
        setError(tr("Unsupported audio format"));
        return;
    }
    m_model->addTrack(track);
}

void AppController::playIndex(int index)
{
    QUrl url = m_model->urlAt(index);
    if (!url.isValid()) {
        setError(tr("File not found"));
        return;
    }
    m_currentIndex = index;
    emit currentIndexChanged();
    m_player.setSource(url);
    m_player.play();
    emit playingChanged();
}

void AppController::togglePlayback()
{
    if (m_model->rowCount() == 0) {
        setError(tr("Playlist is empty"));
        return;
    }
    if (m_currentIndex < 0) {
        playIndex(0);
        return;
    }
    if (playing()) {
        m_player.pause();
    } else {
        m_player.play();
    }
    emit playingChanged();
}

void AppController::stop()
{
    m_player.stop();
    emit playingChanged();
}

void AppController::next()
{
    if (m_model->rowCount() == 0) {
        setError(tr("Playlist is empty"));
        return;
    }
    playIndex((m_currentIndex + 1) % m_model->rowCount());
}

void AppController::previous()
{
    if (m_model->rowCount() == 0) {
        setError(tr("Playlist is empty"));
        return;
    }
    const int nextIndex = m_currentIndex <= 0 ? m_model->rowCount() - 1 : m_currentIndex - 1;
    playIndex(nextIndex);
}

void AppController::seek(qint64 positionMs)
{
    m_player.setPosition(positionMs);
}

void AppController::removeAt(int index)
{
    if (index == m_currentIndex) {
        stop();
        m_currentIndex = -1;
        emit currentIndexChanged();
    }
    m_model->removeAt(index);
}

QString AppController::formatTime(qint64 value) const
{
    const qint64 seconds = value / 1000;
    return QString("%1:%2")
        .arg(seconds / 60)
        .arg(seconds % 60, 2, 10, QChar('0'));
}

void AppController::setError(const QString &message)
{
    m_errorMessage = message;
    qWarning() << "[AppController]" << message;
    emit errorMessageChanged();
}

void AppController::persist()
{
    if (!m_repository.save(m_model->tracks())) {
        setError(tr("SQLite error"));
    }
}
