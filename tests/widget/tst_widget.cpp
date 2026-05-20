#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>
#include <QObject>
#include <QCoreApplication>

#include "gameviewmodel.h"
#include "settingsmanager.h"
#include "platforminfo.h"
#include "localizer.h"
#include "drophandler.h"

// QuickTest setup: registers the same context properties the real
// application provides, so .qml test files can drive the VM directly.
class Setup : public QObject
{
    Q_OBJECT
public:
    Setup() = default;

public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        QCoreApplication::setOrganizationName("Lab9Task3Test");
        QCoreApplication::setApplicationName("WidgetTests");

        m_settings = new SettingsManager(this);
        m_settings->resetStats();

        m_vm = new GameViewModel(this);
        m_vm->setSettingsManager(m_settings);
        m_vm->seed(123);

        m_platform = new PlatformInfo(this);
        m_localizer = new Localizer(this);
        m_localizer->setLocale("en");
        m_drop = new DropHandler(this);

        auto *ctx = engine->rootContext();
        ctx->setContextProperty("gameVm", m_vm);
        ctx->setContextProperty("settingsMgr", m_settings);
        ctx->setContextProperty("platformInfo", m_platform);
        ctx->setContextProperty("localizer", m_localizer);
        ctx->setContextProperty("dropHandler", m_drop);
    }

private:
    SettingsManager *m_settings = nullptr;
    GameViewModel   *m_vm = nullptr;
    PlatformInfo    *m_platform = nullptr;
    Localizer       *m_localizer = nullptr;
    DropHandler     *m_drop = nullptr;
};

QUICK_TEST_MAIN_WITH_SETUP(WidgetTests, Setup)

#include "tst_widget.moc"
