#include "drophandler.h"
#include <QUrl>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDebug>

DropHandler::DropHandler(QObject *parent) : QObject(parent)
{
}

int DropHandler::extractSeedFromUrl(const QString &url) const
{
    QString path;
    if (url.startsWith(QLatin1String("file:"))) {
        path = QUrl(url).toLocalFile();
    } else {
        path = url;
    }
    const QString base = QFileInfo(path).fileName();
    static const QRegularExpression re(QStringLiteral("(\\d{1,4})"));
    const auto match = re.match(base);
    if (!match.hasMatch()) return -1;
    bool ok = false;
    int v = match.captured(1).toInt(&ok);
    return ok ? (v % 100) : -1;
}

void DropHandler::processUrl(const QString &url)
{
    try {
        if (url.isEmpty()) {
            throw std::invalid_argument("Empty url");
        }
        const int seed = extractSeedFromUrl(url);
        if (seed < 0) {
            m_message = tr("No digits found in '%1'").arg(url);
            qWarning() << "[DropHandler]" << m_message;
            emit dropError(m_message);
            return;
        }
        m_message = tr("Loaded %1, seed = %2").arg(url).arg(seed);
        qDebug() << "[DropHandler]" << m_message;
        emit fileDropped(url, seed);
    } catch (const std::exception &e) {
        qCritical() << "[DropHandler] Exception:" << e.what();
        emit dropError(tr("Drop failed: %1").arg(QString::fromLatin1(e.what())));
    }
}
