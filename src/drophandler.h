#ifndef DROPHANDLER_H
#define DROPHANDLER_H

#include <QObject>
#include <QQmlEngine>

// Receives drag-and-drop file events and extracts a seed value
// from the dropped file's name (used by the Linux build for
// "drag a file with a number in its name to seed the game").
class DropHandler : public QObject
{
    Q_OBJECT

public:
    explicit DropHandler(QObject *parent = nullptr);

    Q_INVOKABLE int extractSeedFromUrl(const QString &url) const;
    Q_INVOKABLE QString lastDropMessage() const { return m_message; }

signals:
    void fileDropped(const QString &url, int seed);
    void dropError(const QString &message);

public slots:
    void processUrl(const QString &url);

private:
    QString m_message;
};

#endif
