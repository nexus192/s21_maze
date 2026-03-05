#include "solver.h"

#include <QQueue>
#include <unordered_map>

#include "src/lib/model/maze.h"

namespace {
// hash for QPoint to use in unordered_map
struct QPointHash {
  size_t operator()(const QPoint& p) const {
    return std::hash<int>()(p.x()) ^ (std::hash<int>()(p.y()) << 16);
  }
};
}  // namespace

Solver::Solver(QObject* parent) : QObject(parent) {}

void Solver::setMazeData(const MazeData* maze) { maze_ = maze; }

bool Solver::canMove(const MazeData& maze, QPoint from,
                     Direction direction) const {
  const int fromRow = from.x();
  const int fromColumn = from.y();

  switch (direction) {
    case Direction::Right:
      return !maze.cells[fromRow][fromColumn].rightWall;
    case Direction::Left:
      if (fromColumn == 0) return false;
      return !maze.cells[fromRow][fromColumn - 1].rightWall;
    case Direction::Down:
      return !maze.cells[fromRow][fromColumn].bottomWall;
    case Direction::Up:
      if (fromRow == 0) return false;
      return !maze.cells[fromRow - 1][fromColumn].bottomWall;
  }
  Q_UNREACHABLE();
}

std::vector<QPoint> Solver::solve(const MazeData& maze, QPoint start,
                                  QPoint end) {
  if (maze.cells.empty()) return {};

  // validate bounds
  if (start.x() < 0 || start.x() >= maze.rows || start.y() < 0 ||
      start.y() >= maze.cols || end.x() < 0 || end.x() >= maze.rows ||
      end.y() < 0 || end.y() >= maze.cols) {
    return {};
  }

  if (start == end) return {start};

  // bfs
  QQueue<QPoint> queue;
  std::unordered_map<QPoint, QPoint, QPointHash> parent;

  queue.enqueue(start);
  parent[start] = QPoint(-1, -1);  // sentinel for start

  while (!queue.isEmpty()) {
    QPoint current = queue.dequeue();

    if (current == end) {
      // reconstruct path
      std::vector<QPoint> path;
      for (QPoint p = end; p != QPoint(-1, -1); p = parent[p]) {
        path.push_back(p);
      }
      std::reverse(path.begin(), path.end());
      return path;
    }

    for (const auto& [direction, delta] : kDirections) {
      QPoint next(current.x() + delta.x(), current.y() + delta.y());

      if (next.x() < 0 || next.x() >= maze.rows || next.y() < 0 ||
          next.y() >= maze.cols)
        continue;

      if (parent.find(next) == parent.end() &&
          canMove(maze, current, direction)) {
        parent[next] = current;
        queue.enqueue(next);
      }
    }
  }

  return {};  // no path found
}

void Solver::solveMaze(int startRow, int startCol, int endRow, int endCol) {
  if (!maze_) {
    currentPath_.clear();
    emit pathChanged();
    return;
  }

  currentPath_ =
      solve(*maze_, QPoint(startRow, startCol), QPoint(endRow, endCol));
  emit pathChanged();
}

void Solver::clearPath() {
  currentPath_.clear();
  emit pathChanged();
}

QVariantList Solver::path() const {
  QVariantList result;
  for (const auto& p : currentPath_) {
    result.append(QVariantMap{{"row", p.x()}, {"col", p.y()}});
  }
  return result;
}

bool Solver::hasSolution() const { return !currentPath_.empty(); }
