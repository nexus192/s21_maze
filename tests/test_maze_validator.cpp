#include <QtTest/QtTest>

#include "src/lib/model/maze.h"
#include "src/lib/service/generator/generator.h"
#include "tests/utils/maze_validator.h"

namespace {

MazeData MakeMaze(int rows, int cols,
                  const std::vector<std::vector<bool>>& right,
                  const std::vector<std::vector<bool>>& bottom) {
  MazeData maze;
  maze.rows = rows;
  maze.cols = cols;
  maze.isGenerated = true;
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

class TestMazeValidator : public QObject {
  Q_OBJECT

 private slots:
  void perfectMaze_2x2() {
    MazeData maze = MakeMaze(2, 2,
                             {
                                 {true, true},
                                 {false, true},
                             },
                             {
                                 {false, false},
                                 {true, true},
                             });

    QVERIFY(MazeValidator::IsMazePerfect(maze));
  }
  void brokenBoundaryWall_returnsFalse() {
    MazeData maze = MakeMaze(2, 2,
                             {
                                 {false, false},  // rightmost wall missing
                                 {false, true},
                             },
                             {
                                 {false, false},
                                 {true, true},
                             });

    QVERIFY(!MazeValidator::IsMazePerfect(maze));
  }
  void brokenBottomWall_returnsFalse() {
    MazeData maze = MakeMaze(2, 2,
                             {
                                 {false, true},  // rightmost wall missing
                                 {false, true},
                             },
                             {
                                 {false, false},
                                 {true, false},
                             });

    QVERIFY(!MazeValidator::IsMazePerfect(maze));
  }
  void disconnectedMaze_returnsFalse() {
    MazeData maze = MakeMaze(2, 2,
                             {
                                 {true, true},
                                 {true, true},
                             },
                             {
                                 {true, true},
                                 {true, true},
                             });

    QVERIFY(!MazeValidator::IsMazePerfect(maze));
  }
  void mazeWithCycle_returnsFalse() {
    MazeData maze = MakeMaze(2, 2,
                             {
                                 {false, true},
                                 {false, true},
                             },
                             {
                                 {false, false},
                                 {true, true},
                             });

    // add extra passage → cycle
    maze.cells[0][1].bottomWall = false;

    QVERIFY(!MazeValidator::IsMazePerfect(maze));
  }
};

QTEST_MAIN(TestMazeValidator)
#include "test_maze_validator.moc"
