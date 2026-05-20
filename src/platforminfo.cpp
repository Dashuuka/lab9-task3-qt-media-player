#include "platforminfo.h"
#include <QDebug>

PlatformInfo::PlatformInfo(QObject *parent) : QObject(parent)
{
#if defined(Q_OS_ANDROID) || defined(TARGET_ANDROID)
    m_isAndroid = true;
    m_name = QStringLiteral("Android");
#elif defined(Q_OS_IOS) || defined(TARGET_IOS)
    m_isIos = true;
    m_name = QStringLiteral("iOS");
#elif defined(Q_OS_WASM) || defined(TARGET_WEB) || defined(__EMSCRIPTEN__)
    m_isWeb = true;
    m_name = QStringLiteral("Web");
#else
    m_isLinux = true;
    m_name = QStringLiteral("Linux");
#endif
    qDebug() << "[PlatformInfo] target =" << m_name;
}

QString PlatformInfo::webSkin(int width) const
{
    return width < 720 ? QStringLiteral("android") : QStringLiteral("linux");
}
