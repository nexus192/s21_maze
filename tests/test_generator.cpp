#include <QPoint>
#include <QQueue>
#include <QSet>
#include <QtTest/QtTest>

#include "src/lib/model/maze.h"
#include "src/lib/service/generator/generator.h"

class TestGenerator : public QObject {
  Q_OBJECT

 private:
  // helper: count reachable cells from (0,0) using BFS
  int countReachableCells(const MazeData& maze) {
    if (maze.rows == 0 || maze.cols == 0) return 0;

    QSet<QPair<int, int>> visited;
    QQueue<QPair<int, int>> queue;

    queue.enqueue({0, 0});
    visited.insert({0, 0});

    while (!queue.isEmpty()) {
      auto [r, c] = queue.dequeue();

      // right
      if (c + 1 < maze.cols && !maze.cells[r][c].rightWall) {
        if (!visited.contains({r, c + 1})) {
          visited.insert({r, c + 1});
          queue.enqueue({r, c + 1});
        }
      }
      // left
      if (c - 1 >= 0 && !maze.cells[r][c - 1].rightWall) {
        if (!visited.contains({r, c - 1})) {
          visited.insert({r, c - 1});
          queue.enqueue({r, c - 1});
        }
      }
      // down
      if (r + 1 < maze.rows && !maze.cells[r][c].bottomWall) {
        if (!visited.contains({r + 1, c})) {
          visited.insert({r + 1, c});
          queue.enqueue({r + 1, c});
        }
      }
      // up
      if (r - 1 >= 0 && !maze.cells[r - 1][c].bottomWall) {
        if (!visited.contains({r - 1, c})) {
          visited.insert({r - 1, c});
          queue.enqueue({r - 1, c});
        }
      }
    }

    return visited.size();
  }

  // helper: count total passages (absent walls) in maze
  // for a perfect maze (spanning tree): passages = cells - 1
  int countPassages(const MazeData& maze) {
    int passages = 0;

    for (int r = 0; r < maze.rows; ++r) {
      for (int c = 0; c < maze.cols; ++c) {
        // count right passages (skip rightmost column - must be wall)
        if (c < maze.cols - 1 && !maze.cells[r][c].rightWall) {
          ++passages;
        }
        // count bottom passages (skip bottom row - must be wall)
        if (r < maze.rows - 1 && !maze.cells[r][c].bottomWall) {
          ++passages;
        }
      }
    }

    return passages;
  }

  // helper: verify boundary walls are intact
  bool boundaryWallsIntact(const MazeData& maze) {
    // rightmost column must have right walls
    for (int r = 0; r < maze.rows; ++r) {
      if (!maze.cells[r][maze.cols - 1].rightWall) {
        return false;
      }
    }
    // bottom row must have bottom walls
    for (int c = 0; c < maze.cols; ++c) {
      if (!maze.cells[maze.rows - 1][c].bottomWall) {
        return false;
      }
    }
    return true;
  }

 private slots:
  void testDimensions_data() {
    QTest::addColumn<int>("rows");
    QTest::addColumn<int>("cols");

    QTest::newRow("small") << 5 << 5;
    QTest::newRow("rectangular_wide") << 5 << 10;
    QTest::newRow("rectangular_tall") << 10 << 5;
    QTest::newRow("medium") << 20 << 20;
    QTest::newRow("max_size") << 50 << 50;
  }

  void testDimensions() {
    QFETCH(int, rows);
    QFETCH(int, cols);

    Generator gen;
    MazeData maze;
    gen.generate(maze, rows, cols);

    QCOMPARE(maze.rows, rows);
    QCOMPARE(maze.cols, cols);
    QCOMPARE(maze.isGenerated, true);
    QCOMPARE(static_cast<int>(maze.cells.size()), rows);

    for (const auto& row : maze.cells) {
      QCOMPARE(static_cast<int>(row.size()), cols);
    }
  }

  void testBoundaryWalls_data() {
    QTest::addColumn<int>("rows");
    QTest::addColumn<int>("cols");

    QTest::newRow("5x5") << 5 << 5;
    QTest::newRow("10x15") << 10 << 15;
    QTest::newRow("1x10") << 1 << 10;
    QTest::newRow("10x1") << 10 << 1;
  }

  void testBoundaryWalls() {
    QFETCH(int, rows);
    QFETCH(int, cols);

    Generator gen;
    MazeData maze;
    gen.generate(maze, rows, cols);

    QVERIFY2(boundaryWallsIntact(maze), "boundary walls must be intact");
  }

  void testConnectivity_data() {
    QTest::addColumn<int>("rows");
    QTest::addColumn<int>("cols");

    QTest::newRow("3x3") << 3 << 3;
    QTest::newRow("10x10") << 10 << 10;
    QTest::newRow("7x13") << 7 << 13;
    QTest::newRow("20x20") << 20 << 20;
    QTest::newRow("50x50") << 50 << 50;
  }

  void testConnectivity() {
    QFETCH(int, rows);
    QFETCH(int, cols);

    Generator gen;
    MazeData maze;
    gen.generate(maze, rows, cols);

    int totalCells = rows * cols;
    int reachable = countReachableCells(maze);

    QCOMPARE(reachable, totalCells);
  }

  void testPerfectMazeProperty_data() {
    // a perfect maze is a spanning tree: edges = nodes - 1
    QTest::addColumn<int>("rows");
    QTest::addColumn<int>("cols");

    QTest::newRow("3x3") << 3 << 3;
    QTest::newRow("10x10") << 10 << 10;
    QTest::newRow("15x8") << 15 << 8;
    QTest::newRow("20x20") << 20 << 20;
  }

  void testPerfectMazeProperty() {
    QFETCH(int, rows);
    QFETCH(int, cols);

    Generator gen;
    MazeData maze;
    gen.generate(maze, rows, cols);

    int totalCells = rows * cols;
    int passages = countPassages(maze);

    // spanning tree property: edges = vertices - 1
    QCOMPARE(passages, totalCells - 1);
  }

  void testSingleCell() {
    Generator gen;
    MazeData maze;
    gen.generate(maze, 1, 1);

    QCOMPARE(maze.rows, 1);
    QCOMPARE(maze.cols, 1);
    QVERIFY(maze.cells[0][0].rightWall);
    QVERIFY(maze.cells[0][0].bottomWall);
    QCOMPARE(countReachableCells(maze), 1);
  }

  void testSingleRow() {
    Generator gen;
    MazeData maze;
    gen.generate(maze, 1, 10);

    QCOMPARE(maze.rows, 1);
    QCOMPARE(maze.cols, 10);

    // all cells in single row must be connected horizontally
    // so no internal right walls except the last column
    for (int c = 0; c < 9; ++c) {
      QVERIFY2(
          !maze.cells[0][c].rightWall,
          qPrintable(QString("cell [0,%1] should not have right wall").arg(c)));
    }
    QVERIFY(maze.cells[0][9].rightWall);  // boundary

    // all bottom walls must exist (boundary)
    for (int c = 0; c < 10; ++c) {
      QVERIFY(maze.cells[0][c].bottomWall);
    }

    QCOMPARE(countReachableCells(maze), 10);
    QCOMPARE(countPassages(maze), 9);  // n-1 for single row
  }

  void testSingleColumn() {
    Generator gen;
    MazeData maze;
    gen.generate(maze, 10, 1);

    QCOMPARE(maze.rows, 10);
    QCOMPARE(maze.cols, 1);

    // all cells in single column must be connected vertically
    for (int r = 0; r < 9; ++r) {
      QVERIFY2(!maze.cells[r][0].bottomWall,
               qPrintable(
                   QString("cell [%1,0] should not have bottom wall").arg(r)));
    }
    QVERIFY(maze.cells[9][0].bottomWall);  // boundary

    // all right walls must exist (boundary)
    for (int r = 0; r < 10; ++r) {
      QVERIFY(maze.cells[r][0].rightWall);
    }

    QCOMPARE(countReachableCells(maze), 10);
    QCOMPARE(countPassages(maze), 9);
  }

  void testDeterministicWithSeed() {
    // verify generator produces different mazes (randomness works)
    // run multiple times and check we don't get identical results
    Generator gen;
    MazeData maze1, maze2;

    gen.generate(maze1, 10, 10);
    gen.generate(maze2, 10, 10);

    // count differences
    int differences = 0;
    for (int r = 0; r < 10; ++r) {
      for (int c = 0; c < 10; ++c) {
        if (maze1.cells[r][c].rightWall != maze2.cells[r][c].rightWall)
          ++differences;
        if (maze1.cells[r][c].bottomWall != maze2.cells[r][c].bottomWall)
          ++differences;
      }
    }

    // extremely unlikely to be identical (unless RNG broken)
    QVERIFY2(differences > 0,
             "two generated mazes should differ (randomness check)");
  }

  void testMultipleGenerations() {
    // stress test: generate many mazes, all should be valid
    Generator gen;

    for (int i = 0; i < 100; ++i) {
      MazeData maze;
      int rows = 5 + (i % 20);
      int cols = 5 + ((i * 7) % 20);

      gen.generate(maze, rows, cols);

      QVERIFY2(maze.isGenerated,
               qPrintable(QString("iteration %1: maze not generated").arg(i)));
      QVERIFY2(
          boundaryWallsIntact(maze),
          qPrintable(QString("iteration %1: boundary walls broken").arg(i)));
      QCOMPARE(countReachableCells(maze), rows * cols);
      QCOMPARE(countPassages(maze), rows * cols - 1);
    }
  }
};

QTEST_MAIN(TestGenerator)
#include "test_generator.moc"
