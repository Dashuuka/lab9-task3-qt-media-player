#include "appcontroller.h"
#include "playlistmodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    PlaylistModel playlistModel;
    AppController appController(&playlistModel);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("playlistModel", &playlistModel);
    engine.rootContext()->setContextProperty("appController", &appController);
    engine.loadFromModule("Lab9MediaPlayer", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    return app.exec();
}
