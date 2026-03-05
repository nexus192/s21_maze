#ifndef S21_SOLVER_H_
#define S21_SOLVER_H_

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
  enum class Direction { Right, Left, Down, Up };
  static constexpr std::array<std::pair<Direction, QPoint>, 4> kDirections = {{
      {Direction::Right, {0, 1}},
      {Direction::Left, {0, -1}},
      {Direction::Down, {1, 0}},
      {Direction::Up, {-1, 0}},
  }};

  bool canMove(const MazeData& maze, QPoint from, Direction direction) const;

  const MazeData* maze_ = nullptr;
  std::vector<QPoint> currentPath_;
};

#endif  // S21_SOLVER_H_
