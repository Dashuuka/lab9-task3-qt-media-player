#include "playlistmodel.h"
#include <QDebug>

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractListModel(parent) {}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_tracks.size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_tracks.size()) {
        return {};
    }
    const AudioTrack &track = m_tracks.at(index.row());
    switch (role) {
    case TitleRole: return track.title;
    case FilePathRole: return track.filePath;
    case UrlRole: return track.url;
    case DurationRole: return track.durationMs;
    default: return {};
    }
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    return {
        {TitleRole, "title"},
        {FilePathRole, "filePath"},
        {UrlRole, "url"},
        {DurationRole, "durationMs"}
    };
}

void PlaylistModel::addTrack(const AudioTrack &track)
{
    if (!track.isValid()) {
        qWarning("[PlaylistModel] Unsupported or invalid audio file");
        return;
    }
    beginInsertRows(QModelIndex(), m_tracks.size(), m_tracks.size());
    m_tracks.append(track);
    endInsertRows();
    emit playlistChanged();
}

void PlaylistModel::removeAt(int row)
{
    if (row < 0 || row >= m_tracks.size()) {
        qWarning("[PlaylistModel] Remove index is out of range");
        return;
    }
    beginRemoveRows(QModelIndex(), row, row);
    m_tracks.removeAt(row);
    endRemoveRows();
    emit playlistChanged();
}

void PlaylistModel::clear()
{
    beginResetModel();
    m_tracks.clear();
    endResetModel();
    emit playlistChanged();
}

QString PlaylistModel::titleAt(int row) const
{
    return row >= 0 && row < m_tracks.size() ? m_tracks.at(row).title : QString();
}

QUrl PlaylistModel::urlAt(int row) const
{
    return row >= 0 && row < m_tracks.size() ? m_tracks.at(row).url : QUrl();
}

QList<AudioTrack> PlaylistModel::tracks() const
{
    return m_tracks;
}

void PlaylistModel::setTracks(const QList<AudioTrack> &tracks)
{
    beginResetModel();
    m_tracks = tracks;
    endResetModel();
    emit playlistChanged();
}
