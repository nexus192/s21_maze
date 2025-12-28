#pragma once

#include <QObject>
#include <QPoint>
#include <QVariantList>
#include <vector>

struct MazeData;

class Solver : public QObject {
  Q_OBJECT

  Q_PROPERTY(QVariantList path READ path NOTIFY pathChanged)
  Q_PROPERTY(bool hasSolution READ hasSolution NOTIFY pathChanged)

 public:
  explicit Solver(QObject* parent = nullptr);

  // returns path as vector of {row, col} points, empty if no solution
  std::vector<QPoint> solve(const MazeData& maze, QPoint start, QPoint end);

  Q_INVOKABLE void solveMaze(int startRow, int startCol, int endRow,
                             int endCol);
  Q_INVOKABLE void clearPath();

  QVariantList path() const;
  bool hasSolution() const;

  void setMazeData(const MazeData* maze);

 signals:
  void pathChanged();

 private:
  bool canMove(const MazeData& maze, QPoint from, QPoint to) const;

  const MazeData* maze_ = nullptr;
  std::vector<QPoint> currentPath_;
};
