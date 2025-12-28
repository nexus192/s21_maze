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

  currentLoadTask_ = QtConcurrent::run(&AsyncIOParser::parseMazeFile, filePath);
  watcher->setFuture(currentLoadTask_);
}

SaveResult AsyncIOParser::writeMazeFile(const QString& filePath,
                                        const MazeData& maze) {
  if (!maze.isGenerated) {
    return {"no maze data to save"};
  }

  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return {"cannot open file for writing: " + filePath};
  }

  QTextStream out(&file);

  // write dimensions
  out << maze.rows << " " << maze.cols << "\n";

  // write right walls matrix
  for (int r = 0; r < maze.rows; ++r) {
    for (int c = 0; c < maze.cols; ++c) {
      out << (maze.cells[r][c].rightWall ? 1 : 0);
      if (c < maze.cols - 1) out << " ";
    }
    out << "\n";
  }

  out << "\n";  // blank line separator

  // write bottom walls matrix
  for (int r = 0; r < maze.rows; ++r) {
    for (int c = 0; c < maze.cols; ++c) {
      out << (maze.cells[r][c].bottomWall ? 1 : 0);
      if (c < maze.cols - 1) out << " ";
    }
    out << "\n";
  }

  if (out.status() != QTextStream::Ok) {
    return {"write error occurred"};
  }

  return {};
}

void AsyncIOParser::saveMazeAsync(const QUrl& fileUrl, MazeModel* model) {
  if (!model) {
    emit savingFinished(false, "null model");
    return;
  }

  if (!model->isGenerated()) {
    emit savingFinished(false, "no maze to save");
    return;
  }

  QString filePath = fileUrl.toLocalFile();
  if (filePath.isEmpty()) {
    emit savingFinished(false, "invalid file URL");
    return;
  }

  emit savingStarted();

  // capture maze data for the async task
  MazeData mazeData;
  mazeData.rows = model->rows();
  mazeData.cols = model->cols();
  mazeData.isGenerated = true;
  mazeData.cells.resize(mazeData.rows);

  for (int r = 0; r < mazeData.rows; ++r) {
    mazeData.cells[r].resize(mazeData.cols);
    for (int c = 0; c < mazeData.cols; ++c) {
      int idx = r * mazeData.cols + c;
      QModelIndex modelIdx = model->index(idx);
      mazeData.cells[r][c].rightWall =
          model->data(modelIdx, MazeModel::RightWallRole).toBool();
      mazeData.cells[r][c].bottomWall =
          model->data(modelIdx, MazeModel::BottomWallRole).toBool();
    }
  }

  auto* watcher = new QFutureWatcher<SaveResult>(this);

  connect(watcher, &QFutureWatcher<SaveResult>::finished, this,
          [this, watcher]() {
            SaveResult result = watcher->result();
            emit savingFinished(result.isValid(), result.error);
            watcher->deleteLater();
          });

  currentSaveTask_ =
      QtConcurrent::run(&AsyncIOParser::writeMazeFile, filePath, mazeData);
  watcher->setFuture(currentSaveTask_);
}
