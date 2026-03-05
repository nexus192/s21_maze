#include <QPoint>
#include <QtTest/QtTest>

#include "src/lib/model/maze.h"
#include "src/lib/service/generator/generator.h"
#include "src/lib/service/solver/solver.h"

namespace {

MazeData MakeMaze(int rows, int cols,
                  const std::vector<std::vector<bool>>& right,
                  const std::vector<std::vector<bool>>& bottom) {
  MazeData maze;
  maze.rows = rows;
  maze.cols = cols;
  maze.cells.resize(rows, std::vector<MazeCell>(cols));

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      maze.cells[r][c].rightWall = right[r][c];
      maze.cells[r][c].bottomWall = bottom[r][c];
    }
  }

  return maze;
}

}  // namespace

class TestSolver : public QObject {
  Q_OBJECT

 private:
  MazeData CreateCorridorMaze5x5() {
    return MakeMaze(5, 5,
                    {
                        {true, true, true, true, true},
                        {true, true, true, true, true},
                        {false, false, false, true, true},
                        {true, true, true, true, true},
                        {true, true, true, false, true},
                    },
                    {
                        {false, true, true, true, true},
                        {false, true, true, true, true},
                        {true, true, true, false, false},
                        {true, true, true, false, false},
                        {true, true, true, true, true},
                    });
  }

 private slots:

  // ---- Edge cases ----

  void testSameStartEnd() {
    MazeData maze = MakeMaze(2, 2,
                             {
                                 {false, true},
                                 {false, true},
                             },
                             {
                                 {false, false},
                                 {true, true},
                             });

    Solver solver;

    auto path = solver.solve(maze, QPoint(1, 1), QPoint(1, 1));

    QCOMPARE(path.size(), 1u);
    QCOMPARE(path[0], QPoint(1, 1));
  }

  void testInvalidStartBounds() {
    MazeData maze = MakeMaze(2, 2,
                             {
                                 {false, true},
                                 {false, true},
                             },
                             {
                                 {false, false},
                                 {true, true},
                             });

    Solver solver;

    QVERIFY(solver.solve(maze, QPoint(-1, 0), QPoint(0, 0)).empty());
    QVERIFY(solver.solve(maze, QPoint(0, -1), QPoint(0, 0)).empty());
    QVERIFY(solver.solve(maze, QPoint(10, 0), QPoint(0, 0)).empty());
    QVERIFY(solver.solve(maze, QPoint(0, 10), QPoint(0, 0)).empty());
  }

  void testInvalidEndBounds() {
    MazeData maze = MakeMaze(2, 2,
                             {
                                 {false, true},
                                 {false, true},
                             },
                             {
                                 {false, false},
                                 {true, true},
                             });

    Solver solver;

    QVERIFY(solver.solve(maze, QPoint(0, 0), QPoint(-1, 0)).empty());
    QVERIFY(solver.solve(maze, QPoint(0, 0), QPoint(3, 0)).empty());
  }

  void testUninitializedMaze() {
    MazeData maze;
    Solver solver;

    auto path = solver.solve(maze, QPoint(0, 0), QPoint(1, 1));

    QVERIFY(path.empty());
  }

  // ---- Deterministic solver tests (5x5) ----

  void testKnownCorridorPath() {
    MazeData maze = CreateCorridorMaze5x5();
    Solver solver;

    auto path = solver.solve(maze, QPoint(0, 0), QPoint(4, 4));

    std::vector<QPoint> expected = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2},
                                    {2, 3}, {3, 3}, {4, 3}, {4, 4}};

    QCOMPARE(path, expected);
  }

  void testMinimumSizeMaze5x5() {
    MazeData maze = CreateCorridorMaze5x5();
    Solver solver;

    auto path = solver.solve(maze, QPoint(0, 0), QPoint(4, 4));

    QVERIFY(!path.empty());
    QCOMPARE(path.front(), QPoint(0, 0));
    QCOMPARE(path.back(), QPoint(4, 4));
  }

  void testNoPath5x5() {
    MazeData maze = MakeMaze(5, 5,
                             {
                                 {true, true, true, true, true},
                                 {true, true, true, true, true},
                                 {true, true, true, true, true},
                                 {true, true, true, true, true},
                                 {true, true, true, true, true},
                             },
                             {
                                 {true, true, true, true, true},
                                 {true, true, true, true, true},
                                 {true, true, true, true, true},
                                 {true, true, true, true, true},
                                 {true, true, true, true, true},
                             });

    Solver solver;

    auto path = solver.solve(maze, QPoint(0, 0), QPoint(4, 4));

    QVERIFY(path.empty());
  }

  void testDeterministicMaze10x10() {
    MazeData maze = MakeMaze(10, 10,
                             {{0, 1, 0, 0, 1, 0, 0, 0, 1, 1},
                              {1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
                              {1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                              {1, 1, 0, 1, 0, 0, 1, 0, 1, 1},
                              {0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                              {0, 0, 1, 0, 1, 1, 1, 0, 0, 1},
                              {1, 1, 0, 1, 1, 0, 0, 0, 1, 1},
                              {0, 1, 1, 1, 1, 1, 1, 0, 1, 1},
                              {1, 1, 1, 0, 1, 0, 1, 0, 0, 1},
                              {1, 0, 0, 0, 0, 0, 0, 0, 1, 1}},
                             {{0, 0, 1, 0, 0, 1, 1, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 1, 0, 1, 0, 1, 1, 1, 0},
                              {0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
                              {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
                              {1, 1, 0, 0, 0, 0, 0, 1, 0, 0},
                              {0, 0, 1, 0, 0, 1, 1, 1, 0, 0},
                              {0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
                              {0, 0, 0, 1, 0, 0, 1, 1, 0, 0},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}});

    Solver solver;

    QPoint start(0, 0);
    QPoint end(9, 9);

    auto path = solver.solve(maze, start, end);

    const std::vector<QPoint> expected = {
        QPoint(0, 0), QPoint(1, 0), QPoint(2, 0), QPoint(3, 0), QPoint(4, 0),
        QPoint(4, 1), QPoint(5, 1), QPoint(5, 2), QPoint(6, 2), QPoint(6, 3),
        QPoint(7, 3), QPoint(8, 3), QPoint(8, 4), QPoint(9, 4), QPoint(9, 5),
        QPoint(9, 6), QPoint(9, 7), QPoint(9, 8), QPoint(8, 8), QPoint(8, 9),
        QPoint(9, 9)};

    QCOMPARE(path, expected);
  }

  // ---- Generator integration ----

  void testGeneratorIntegration() {
    Generator gen;
    MazeData maze;

    gen.generate(maze, 10, 10);

    Solver solver;

    QPoint start(0, 0);
    QPoint end(9, 9);

    auto path = solver.solve(maze, start, end);

    QVERIFY(!path.empty());
    QCOMPARE(path.front(), start);
    QCOMPARE(path.back(), end);
  }

  // ---- QML interface ----

  void testSolverQmlInterface() {
    Generator gen;
    MazeData maze;

    gen.generate(maze, 5, 5);

    Solver solver;
    solver.setMazeData(&maze);

    QVERIFY(!solver.hasSolution());
    QVERIFY(solver.path().isEmpty());

    solver.solveMaze(0, 0, 4, 4);

    QVERIFY(solver.hasSolution());
    QVERIFY(!solver.path().isEmpty());

    QVariantList pathList = solver.path();

    QVariantMap first = pathList.first().toMap();
    QCOMPARE(first["row"].toInt(), 0);
    QCOMPARE(first["col"].toInt(), 0);

    QVariantMap last = pathList.last().toMap();
    QCOMPARE(last["row"].toInt(), 4);
    QCOMPARE(last["col"].toInt(), 4);

    solver.clearPath();

    QVERIFY(!solver.hasSolution());
    QVERIFY(solver.path().isEmpty());
  }

  void testSolverWithNullMaze() {
    Solver solver;

    solver.solveMaze(0, 0, 1, 1);

    QVERIFY(!solver.hasSolution());
  }
};

QTEST_MAIN(TestSolver)
#include "test_solver.moc"