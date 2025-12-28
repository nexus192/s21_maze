#pragma once

#include <QFuture>
#include <QObject>
#include <QString>
#include <QUrl>

#include "src/lib/model/maze.h"

class MazeModel;

struct ParseResult {
  MazeData data;
  QString error;  // empty = success

  bool isValid() const { return error.isEmpty(); }
};

class AsyncIOParser : public QObject {
  Q_OBJECT

 public:
  explicit AsyncIOParser(QObject* parent = nullptr);

  Q_INVOKABLE void loadMazeAsync(const QUrl& fileUrl, MazeModel* model);

  // sync version for testing
  static ParseResult parseMazeFile(const QString& filePath);

 signals:
  void loadingStarted();
  void loadingFinished(bool success, const QString& errorMsg);

 private:
  QFuture<ParseResult> currentTask_;
};
