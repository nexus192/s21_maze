#include <QtTest/QtTest>

#include "src/lib/model/maze.h"
#include "src/lib/service/generator/generator.h"

class TestGenerator : public QObject {
  Q_OBJECT

 private slots:

  void testGeneratorIsNotDeterministic() {
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
      QVERIFY(MazeValidator::IsMazePerfect(maze));
    }
  }
};

QTEST_MAIN(TestGenerator)
#include "test_generator.moc"
