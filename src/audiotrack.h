#pragma once

#include <QUrl>
#include <QString>

struct AudioTrack {
    QString title;
    QString filePath;
    QUrl url;
    qint64 durationMs = 0;

    bool isValid() const;
};
