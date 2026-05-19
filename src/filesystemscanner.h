#pragma once

#include "audiotrack.h"
#include <QList>
#include <QObject>
#include <QStringList>

class FileSystemScanner : public QObject {
    Q_OBJECT
public:
    explicit FileSystemScanner(QObject *parent = nullptr);

    Q_INVOKABLE QList<AudioTrack> scanFolder(const QString &folderPath) const;
    Q_INVOKABLE AudioTrack trackFromFile(const QString &filePath) const;

    static bool isSupportedAudio(const QString &filePath);
    static QStringList supportedExtensions();
};
