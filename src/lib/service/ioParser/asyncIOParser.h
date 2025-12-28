#pragma once

#include <QFuture>
#include <QObject>
#include <QString>
#include <QUrl>

#include "src/lib/model/maze.h"

class MazeModel;

struct ParseResult {
  MazeData data;
  QString error;

  bool isValid() const { return error.isEmpty(); }
};

struct SaveResult {
  QString error;  // empty = success
  bool isValid() const { return error.isEmpty(); }
};

class AsyncIOParser : public QObject {
  Q_OBJECT

 public:
  explicit AsyncIOParser(QObject* parent = nullptr);

  Q_INVOKABLE void loadMazeAsync(const QUrl& fileUrl, MazeModel* model);
  Q_INVOKABLE void saveMazeAsync(const QUrl& fileUrl, MazeModel* model);

  // sync versions for testing
  static ParseResult parseMazeFile(const QString& filePath);
  static SaveResult writeMazeFile(const QString& filePath,
                                  const MazeData& maze);

 signals:
  void loadingStarted();
  void loadingFinished(bool success, const QString& errorMsg);
  void savingStarted();
  void savingFinished(bool success, const QString& errorMsg);

 private:
  QFuture<ParseResult> currentLoadTask_;
  QFuture<SaveResult> currentSaveTask_;
};
