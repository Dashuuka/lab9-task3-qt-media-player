#ifndef LOCALIZER_H
#define LOCALIZER_H

#include <QObject>
#include <QQmlEngine>
#include <QTranslator>
#include <QPointer>

class QCoreApplication;

class Localizer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString currentLocale READ currentLocale NOTIFY localeChanged)
    Q_PROPERTY(QStringList supportedLocales READ supportedLocales CONSTANT)

public:
    explicit Localizer(QObject *parent = nullptr);

    QString currentLocale() const { return m_locale; }
    QStringList supportedLocales() const { return {"en", "ru", "be"}; }

    Q_INVOKABLE bool setLocale(const QString &locale);
    Q_INVOKABLE QString displayName(const QString &locale) const;

    void initFromSystem();
    void setEngine(class QQmlEngine *engine);

signals:
    void localeChanged();

private:
    QString m_locale;
    QTranslator m_translator;
    QPointer<QQmlEngine> m_engine;

    bool loadInternal(const QString &locale);
};

#endif
