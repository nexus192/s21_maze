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

bool Solver::canMove(const MazeData& maze, QPoint from, QPoint to) const {
  int fr = from.x(), fc = from.y();
  int tr = to.x(), tc = to.y();

  // bounds check
  if (tr < 0 || tr >= maze.rows || tc < 0 || tc >= maze.cols) {
    return false;
  }

  // moving right: check right wall of current cell
  if (tr == fr && tc == fc + 1) {
    return !maze.cells[fr][fc].rightWall;
  }
  // moving left: check right wall of target cell
  if (tr == fr && tc == fc - 1) {
    return !maze.cells[tr][tc].rightWall;
  }
  // moving down: check bottom wall of current cell
  if (tc == fc && tr == fr + 1) {
    return !maze.cells[fr][fc].bottomWall;
  }
  // moving up: check bottom wall of target cell
  if (tc == fc && tr == fr - 1) {
    return !maze.cells[tr][tc].bottomWall;
  }

  return false;
}

std::vector<QPoint> Solver::solve(const MazeData& maze, QPoint start,
                                  QPoint end) {
  if (!maze.isGenerated) return {};

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

  const std::array<QPoint, 4> directions = {
      QPoint(0, 1),   // right
      QPoint(0, -1),  // left
      QPoint(1, 0),   // down
      QPoint(-1, 0)   // up
  };

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

    for (const auto& dir : directions) {
      QPoint next(current.x() + dir.x(), current.y() + dir.y());

      if (parent.find(next) == parent.end() && canMove(maze, current, next)) {
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
