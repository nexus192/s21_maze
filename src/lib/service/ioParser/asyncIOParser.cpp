#include "asyncIOParser.h"

#include <QFile>
#include <QFutureWatcher>
#include <QTextStream>
#include <QtConcurrent>

#include "src/lib/model/maze.h"

AsyncIOParser::AsyncIOParser(QObject* parent) : QObject(parent) {}

ParseResult AsyncIOParser::parseMazeFile(const QString& filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return {{}, "file not found: " + filePath};
  }

  QTextStream in(&file);

  int rows = 0, cols = 0;
  in >> rows >> cols;

  if (in.status() != QTextStream::Ok) {
    return {{}, "failed to read dimensions"};
  }
  if (rows <= 0 || cols <= 0 || rows > 50 || cols > 50) {
    return {
        {},
        QString("invalid dimensions: %1x%2 (max 50x50)").arg(rows).arg(cols)};
  }

  MazeData maze;
  maze.rows = rows;
  maze.cols = cols;
  maze.cells.assign(rows, std::vector<MazeCell>(cols, {false, false}));

  // parse right walls matrix
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      int val;
      in >> val;
      if (in.status() != QTextStream::Ok) {
        return {{},
                QString("unexpected end of file at right wall [%1,%2]")
                    .arg(r)
                    .arg(c)};
      }
      if (val != 0 && val != 1) {
        return {{},
                QString("invalid value %1 at right wall [%2,%3]")
                    .arg(val)
                    .arg(r)
                    .arg(c)};
      }
      maze.cells[r][c].rightWall = (val == 1);
    }
  }

  // parse bottom walls matrix
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      int val;
      in >> val;
      if (in.status() != QTextStream::Ok) {
        return {{},
                QString("unexpected end of file at bottom wall [%1,%2]")
                    .arg(r)
                    .arg(c)};
      }
      if (val != 0 && val != 1) {
        return {{},
                QString("invalid value %1 at bottom wall [%2,%3]")
                    .arg(val)
                    .arg(r)
                    .arg(c)};
      }
      maze.cells[r][c].bottomWall = (val == 1);
    }
  }

  maze.isGenerated = true;
  return {maze, {}};
}

void AsyncIOParser::loadMazeAsync(const QUrl& fileUrl, MazeModel* model) {
  if (!model) {
    emit loadingFinished(false, "null model");
    return;
  }

  QString filePath = fileUrl.toLocalFile();
  if (filePath.isEmpty()) {
    emit loadingFinished(false, "invalid file URL");
    return;
  }

  emit loadingStarted();

  auto* watcher = new QFutureWatcher<ParseResult>(this);

  connect(watcher, &QFutureWatcher<ParseResult>::finished, this,
          [this, model, watcher]() {
            ParseResult result = watcher->result();

            if (result.isValid()) {
              model->setMazeData(std::move(result.data));
              emit loadingFinished(true, {});
            } else {
              emit loadingFinished(false, result.error);
            }

            watcher->deleteLater();
          });

  currentTask_ = QtConcurrent::run(&AsyncIOParser::parseMazeFile, filePath);
  watcher->setFuture(currentTask_);
}
