#pragma once

#include "audiotrack.h"
#include <QAbstractListModel>

class PlaylistModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        TitleRole = Qt::UserRole + 1,
        FilePathRole,
        UrlRole,
        DurationRole
    };

    explicit PlaylistModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addTrack(const AudioTrack &track);
    Q_INVOKABLE void removeAt(int row);
    Q_INVOKABLE void clear();
    Q_INVOKABLE QString titleAt(int row) const;
    Q_INVOKABLE QUrl urlAt(int row) const;

    QList<AudioTrack> tracks() const;
    void setTracks(const QList<AudioTrack> &tracks);

signals:
    void playlistChanged();

private:
    QList<AudioTrack> m_tracks;
};
