#ifndef PLATFORMINFO_H
#define PLATFORMINFO_H

#include <QObject>
#include <QQmlEngine>

class PlatformInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool isAndroid READ isAndroid CONSTANT)
    Q_PROPERTY(bool isIos READ isIos CONSTANT)
    Q_PROPERTY(bool isLinux READ isLinux CONSTANT)
    Q_PROPERTY(bool isWeb READ isWeb CONSTANT)
    Q_PROPERTY(bool isMobile READ isMobile CONSTANT)
    Q_PROPERTY(bool isDesktop READ isDesktop CONSTANT)

public:
    explicit PlatformInfo(QObject *parent = nullptr);

    QString name() const { return m_name; }
    bool isAndroid() const { return m_isAndroid; }
    bool isIos() const { return m_isIos; }
    bool isLinux() const { return m_isLinux; }
    bool isWeb() const { return m_isWeb; }
    bool isMobile() const { return m_isAndroid || m_isIos; }
    bool isDesktop() const { return m_isLinux; }

    // For Web: decides which "skin" to use based on viewport width
    Q_INVOKABLE QString webSkin(int width) const;

private:
    QString m_name;
    bool m_isAndroid = false;
    bool m_isIos = false;
    bool m_isLinux = false;
    bool m_isWeb = false;
};

#endif
