#include <QPoint>
#include <QtTest/QtTest>

#include "src/lib/model/maze.h"
#include "src/lib/service/generator/generator.h"
#include "src/lib/service/solver/solver.h"

class TestSolver : public QObject {
  Q_OBJECT

 private:
  // helper: verify path doesn't cross any walls
  bool isPathValid(const MazeData& maze, const std::vector<QPoint>& path) {
    if (path.empty()) return true;

    for (size_t i = 0; i < path.size() - 1; ++i) {
      QPoint from = path[i];
      QPoint to = path[i + 1];

      int fr = from.x(), fc = from.y();
      int tr = to.x(), tc = to.y();

      // must be adjacent
      int dr = std::abs(tr - fr);
      int dc = std::abs(tc - fc);
      if (dr + dc != 1) {
        return false;  // not adjacent
      }

      // check wall crossing
      if (tc == fc + 1 && maze.cells[fr][fc].rightWall) return false;
      if (tc == fc - 1 && maze.cells[tr][tc].rightWall) return false;
      if (tr == fr + 1 && maze.cells[fr][fc].bottomWall) return false;
      if (tr == fr - 1 && maze.cells[tr][tc].bottomWall) return false;
    }

    return true;
  }
  MazeData createSimpleMaze() {
    MazeData maze;
    maze.rows = 3;
    maze.cols = 3;
    maze.isGenerated = true;
    maze.cells.assign(3, std::vector<MazeCell>(3, {false, false}));

    // right walls
    maze.cells[0][0].rightWall = false;  // can pass (0,0) -> (0,1)
    maze.cells[0][1].rightWall = true;   // blocked (0,1) -x-> (0,2)
    maze.cells[0][2].rightWall = true;   // boundary
    maze.cells[1][0].rightWall = false;
    maze.cells[1][1].rightWall = false;
    maze.cells[1][2].rightWall = true;  // boundary
    maze.cells[2][0].rightWall = false;
    maze.cells[2][1].rightWall = false;
    maze.cells[2][2].rightWall = true;  // boundary

    // bottom walls
    maze.cells[0][0].bottomWall = false;  // can pass (0,0) -> (1,0)
    maze.cells[0][1].bottomWall = true;   // blocked (0,1) -x-> (1,1)
    maze.cells[0][2].bottomWall = false;  // can pass (0,2) <-> (1,2)
    maze.cells[1][0].bottomWall = false;
    maze.cells[1][1].bottomWall = false;
    maze.cells[1][2].bottomWall = false;
    maze.cells[2][0].bottomWall = true;  // boundary
    maze.cells[2][1].bottomWall = true;  // boundary
    maze.cells[2][2].bottomWall = true;  // boundary

    return maze;
  }

  // helper: create an impossible maze (cell isolated)
  MazeData createIsolatedCellMaze() {
    MazeData maze;
    maze.rows = 2;
    maze.cols = 2;
    maze.isGenerated = true;
    maze.cells.assign(2, std::vector<MazeCell>(2, {true, true}));
    // all walls = all cells isolated
    return maze;
  }

 private slots:
  void testSameStartEnd() {
    MazeData maze = createSimpleMaze();
    Solver solver;

    auto path = solver.solve(maze, QPoint(1, 1), QPoint(1, 1));

    QCOMPARE(path.size(), 1u);
    QCOMPARE(path[0], QPoint(1, 1));
  }

  void testAdjacentCells() {
    MazeData maze = createSimpleMaze();
    Solver solver;

    // (0,0) to (1,0) - no wall between them
    auto path = solver.solve(maze, QPoint(0, 0), QPoint(1, 0));

    QCOMPARE(path.size(), 2u);
    QCOMPARE(path[0], QPoint(0, 0));
    QCOMPARE(path[1], QPoint(1, 0));
    QVERIFY(isPathValid(maze, path));
  }

  void testKnownPath() {
    MazeData maze = createSimpleMaze();
    Solver solver;

    // from (0,0) to (0,2)
    // must go: (0,0) -> (1,0) -> (2,0) -> (2,1) -> (2,2) -> (1,2) -> (0,2)
    // or similar, avoiding walls
    auto path = solver.solve(maze, QPoint(0, 0), QPoint(0, 2));

    QVERIFY(!path.empty());
    QCOMPARE(path.front(), QPoint(0, 0));
    QCOMPARE(path.back(), QPoint(0, 2));
    QVERIFY(isPathValid(maze, path));
  }

  void testNoPath() {
    MazeData maze = createIsolatedCellMaze();
    Solver solver;

    auto path = solver.solve(maze, QPoint(0, 0), QPoint(1, 1));

    QVERIFY(path.empty());
  }

  void testInvalidStartBounds() {
    MazeData maze = createSimpleMaze();
    Solver solver;

    auto path = solver.solve(maze, QPoint(-1, 0), QPoint(0, 0));
    QVERIFY(path.empty());

    path = solver.solve(maze, QPoint(0, -1), QPoint(0, 0));
    QVERIFY(path.empty());

    path = solver.solve(maze, QPoint(100, 0), QPoint(0, 0));
    QVERIFY(path.empty());

    path = solver.solve(maze, QPoint(0, 100), QPoint(0, 0));
    QVERIFY(path.empty());
  }

  void testInvalidEndBounds() {
    MazeData maze = createSimpleMaze();
    Solver solver;

    auto path = solver.solve(maze, QPoint(0, 0), QPoint(-1, 0));
    QVERIFY(path.empty());

    path = solver.solve(maze, QPoint(0, 0), QPoint(3, 0));
    QVERIFY(path.empty());
  }

  void testUninitializedMaze() {
    MazeData maze;  // default: isGenerated = false
    Solver solver;

    auto path = solver.solve(maze, QPoint(0, 0), QPoint(1, 1));
    QVERIFY(path.empty());
  }

  void testCornerToCorner_data() {
    QTest::addColumn<int>("rows");
    QTest::addColumn<int>("cols");

    QTest::newRow("5x5") << 5 << 5;
    QTest::newRow("10x10") << 10 << 10;
    QTest::newRow("20x20") << 20 << 20;
    QTest::newRow("10x5") << 10 << 5;
  }

  void testCornerToCorner() {
    QFETCH(int, rows);
    QFETCH(int, cols);

    Generator gen;
    MazeData maze;
    gen.generate(maze, rows, cols);

    Solver solver;

    // top-left to bottom-right
    auto path = solver.solve(maze, QPoint(0, 0), QPoint(rows - 1, cols - 1));

    QVERIFY2(!path.empty(),
             "perfect maze must have path between any two cells");
    QCOMPARE(path.front(), QPoint(0, 0));
    QCOMPARE(path.back(), QPoint(rows - 1, cols - 1));
    QVERIFY(isPathValid(maze, path));
  }

  void testPathOptimality() {
    // for a perfect maze, there's exactly one path
    // BFS guarantees shortest path in unweighted graph
    // we verify path length is reasonable (at minimum manhattan distance)
    Generator gen;
    MazeData maze;
    gen.generate(maze, 10, 10);

    Solver solver;
    QPoint start(0, 0);
    QPoint end(9, 9);

    auto path = solver.solve(maze, start, end);

    QVERIFY(!path.empty());

    // minimum possible: manhattan distance + 1 (inclusive)
    int manhattan =
        std::abs(end.x() - start.x()) + std::abs(end.y() - start.y());

    QVERIFY2(static_cast<int>(path.size()) >= manhattan + 1,
             "path cannot be shorter than manhattan distance");
  }

  void testRandomPointsInPerfectMaze() {
    Generator gen;
    MazeData maze;
    gen.generate(maze, 20, 20);

    Solver solver;

    // test 50 random point pairs
    for (int i = 0; i < 50; ++i) {
      int sr = QRandomGenerator::global()->bounded(20);
      int sc = QRandomGenerator::global()->bounded(20);
      int er = QRandomGenerator::global()->bounded(20);
      int ec = QRandomGenerator::global()->bounded(20);

      auto path = solver.solve(maze, QPoint(sr, sc), QPoint(er, ec));

      QVERIFY2(!path.empty(),
               qPrintable(QString("no path from (%1,%2) to (%3,%4)")
                              .arg(sr)
                              .arg(sc)
                              .arg(er)
                              .arg(ec)));
      QCOMPARE(path.front(), QPoint(sr, sc));
      QCOMPARE(path.back(), QPoint(er, ec));
      QVERIFY(isPathValid(maze, path));
    }
  }

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

    // verify QVariantList format
    QVariantList pathList = solver.path();
    QVERIFY(pathList.size() > 0);

    QVariantMap firstPoint = pathList.first().toMap();
    QCOMPARE(firstPoint["row"].toInt(), 0);
    QCOMPARE(firstPoint["col"].toInt(), 0);

    QVariantMap lastPoint = pathList.last().toMap();
    QCOMPARE(lastPoint["row"].toInt(), 4);
    QCOMPARE(lastPoint["col"].toInt(), 4);

    solver.clearPath();
    QVERIFY(!solver.hasSolution());
    QVERIFY(solver.path().isEmpty());
  }

  void testSolverWithNullMaze() {
    Solver solver;
    // no setMazeData called

    solver.solveMaze(0, 0, 1, 1);

    QVERIFY(!solver.hasSolution());
  }
};

QTEST_MAIN(TestSolver)
#include "test_solver.moc"
