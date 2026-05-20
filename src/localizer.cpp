#include "localizer.h"
#include <QCoreApplication>
#include <QLocale>
#include <QQmlEngine>
#include <QDebug>

Localizer::Localizer(QObject *parent) : QObject(parent)
{
}

void Localizer::setEngine(QQmlEngine *engine)
{
    m_engine = engine;
}

void Localizer::initFromSystem()
{
    // 1) try system default
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &lang : uiLanguages) {
        const QString base = lang.left(2).toLower();
        if (supportedLocales().contains(base)) {
            if (loadInternal(base)) return;
        }
    }
    // 2) fall back to English
    loadInternal(QStringLiteral("en"));
}

bool Localizer::loadInternal(const QString &locale)
{
    QCoreApplication::removeTranslator(&m_translator);
    const QString base = QStringLiteral("lab9_") + locale;
    // Qt 6 standard location for qt_add_translations() qm files
    const QStringList candidates = {
        QStringLiteral(":/i18n/") + base,
        QStringLiteral(":/qt/qml/GuessNumberX/i18n/") + base,
        QStringLiteral("i18n/") + base,
    };
    bool ok = false;
    for (const QString &p : candidates) {
        if (m_translator.load(p)) { ok = true; break; }
    }
    if (!ok && locale == QLatin1String("en")) {
        // English is the source language: no translation needed.
        m_locale = locale;
        emit localeChanged();
        if (m_engine) m_engine->retranslate();
        return true;
    }
    if (!ok) {
        qWarning() << "[Localizer] Failed to load translation for" << locale;
        return false;
    }
    QCoreApplication::installTranslator(&m_translator);
    m_locale = locale;
    qDebug() << "[Localizer] Loaded" << locale;
    emit localeChanged();
    if (m_engine) m_engine->retranslate();
    return true;
}

bool Localizer::setLocale(const QString &locale)
{
    if (!supportedLocales().contains(locale)) {
        qWarning() << "[Localizer] Unsupported locale:" << locale;
        return false;
    }
    if (locale == m_locale) return true;
    return loadInternal(locale);
}

QString Localizer::displayName(const QString &locale) const
{
    if (locale == QLatin1String("en")) return QStringLiteral("English");
    if (locale == QLatin1String("ru")) return QStringLiteral("Русский");
    if (locale == QLatin1String("be")) return QStringLiteral("Беларуская");
    return locale;
}
