#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QDebug>

#include "settingsmanager.h"
#include "gameviewmodel.h"
#include "platforminfo.h"
#include "localizer.h"
#include "drophandler.h"

static void setStyleForPlatform(const PlatformInfo &p)
{
    if (p.isAndroid()) {
        QQuickStyle::setStyle(QStringLiteral("Material"));
    } else if (p.isIos()) {
        QQuickStyle::setStyle(QStringLiteral("iOS"));
    } else if (p.isWeb()) {
        // Web uses Material on small viewports, Fusion on large — picked in QML
        QQuickStyle::setStyle(QStringLiteral("Material"));
    } else {
        QQuickStyle::setStyle(QStringLiteral("Fusion"));
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("Lab9Task3"));
    app.setApplicationName(QStringLiteral("GuessNumberX"));
    app.setApplicationVersion(QStringLiteral("1.0.0"));

    PlatformInfo platform;
    setStyleForPlatform(platform);

    SettingsManager settings;
    GameViewModel viewModel;
    DropHandler dropper;
    Localizer localizer;

    viewModel.setSettingsManager(&settings);

    // Persisted locale wins over system locale if set
    const QString persistedLocale = settings.currentLocale();
    if (!persistedLocale.isEmpty() && localizer.supportedLocales().contains(persistedLocale)) {
        localizer.setLocale(persistedLocale);
    } else {
        localizer.initFromSystem();
    }

    QQmlApplicationEngine engine;
    localizer.setEngine(&engine);

    auto *ctx = engine.rootContext();
    ctx->setContextProperty(QStringLiteral("settingsMgr"), &settings);
    ctx->setContextProperty(QStringLiteral("gameVm"), &viewModel);
    ctx->setContextProperty(QStringLiteral("platformInfo"), &platform);
    ctx->setContextProperty(QStringLiteral("localizer"), &localizer);
    ctx->setContextProperty(QStringLiteral("dropHandler"), &dropper);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() {
            qCritical() << "[main] QML object creation failed!";
            QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    QObject::connect(&localizer, &Localizer::localeChanged,
                     &settings, [&]() { settings.setCurrentLocale(localizer.currentLocale()); });

    engine.loadFromModule("GuessNumberX", "Main");
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "[main] No root objects";
        return -1;
    }
    return QCoreApplication::exec();
}
