#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "src/lib/model/maze.h"
#include "src/lib/service/ioParser/asyncIOParser.h"
#include "src/lib/service/solver/solver.h"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  app.setWindowIcon(QIcon(":/icons/labyrinth-logo.svg"));

  MazeModel mazeModel;
  AsyncIOParser parser;
  Solver solver;

  // connect solver to maze data
  QObject::connect(&mazeModel, &MazeModel::mazeChanged, [&]() {
    solver.setMazeData(&mazeModel.mazeData());
    solver.clearPath();
  });

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

  engine.rootContext()->setContextProperty("mazeModel", &mazeModel);
  engine.rootContext()->setContextProperty("mazeParser", &parser);
  engine.rootContext()->setContextProperty("mazeSolver", &solver);
  engine.loadFromModule("s21_maze", "Main");

  return app.exec();
}
