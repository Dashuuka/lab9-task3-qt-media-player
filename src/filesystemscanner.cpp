#include "filesystemscanner.h"
#include <QDir>
#include <QFileInfo>
#include <QUrl>

FileSystemScanner::FileSystemScanner(QObject *parent) : QObject(parent) {}

QStringList FileSystemScanner::supportedExtensions()
{
    return {"mp3", "wav", "ogg", "m4a", "flac"};
}

bool FileSystemScanner::isSupportedAudio(const QString &filePath)
{
    const QString suffix = QFileInfo(filePath).suffix().toLower();
    return supportedExtensions().contains(suffix);
}

AudioTrack FileSystemScanner::trackFromFile(const QString &filePath) const
{
    QFileInfo info(filePath);
    if (!info.exists() || !info.isFile() || !isSupportedAudio(filePath)) {
        return {};
    }
    return AudioTrack{info.completeBaseName(), info.absoluteFilePath(), QUrl::fromLocalFile(info.absoluteFilePath()), 0};
}

QList<AudioTrack> FileSystemScanner::scanFolder(const QString &folderPath) const
{
    QList<AudioTrack> tracks;
    QDir dir(folderPath);
    if (!dir.exists()) {
        qWarning("[Scanner] Folder is not available");
        return tracks;
    }

    QStringList filters;
    for (const QString &ext : supportedExtensions()) {
        filters << "*." + ext;
    }

    const QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Name);
    for (const QFileInfo &file : files) {
        AudioTrack track = trackFromFile(file.absoluteFilePath());
        if (track.isValid()) {
            tracks.append(track);
        }
    }
    return tracks;
}
