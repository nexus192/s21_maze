#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "src/lib/model/maze.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    MazeModel mazeModel;

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("mazeModel", &mazeModel);
    engine.loadFromModule("s21_maze", "Main");

    return app.exec();
}
