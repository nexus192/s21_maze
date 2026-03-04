#include "generator.h"

#include <QRandomGenerator>
#include <algorithm>
#include <unordered_map>

#include "src/lib/model/maze.h"

void Generator::generate(MazeData& maze, int rows, int cols) {
  maze.rows = rows;
  maze.cols = cols;
  maze.cells.assign(rows, std::vector<MazeCell>(cols, {true, true}));

  sets_.assign(cols, kNoSet);

  nextSetId_ = 1;

  for (int row = 0; row < rows - 1; ++row) {
    assignNewSets(cols);
    mergeRandomRight(maze, row);
    createBottomPassages(maze, row, false);
    prepareNextRow(maze, row);
  }

  assignNewSets(cols);
  processLastRow(maze);
}

void Generator::assignNewSets(int cols) {
  for (int col = 0; col < cols; ++col) {
    if (sets_[col] == kNoSet) {
      sets_[col] = nextSetId_++;
    }
  }
}

void Generator::mergeRandomRight(MazeData& maze, int row) {
  int cols = maze.cols;

  for (int col = 0; col < cols - 1; ++col) {
    if (sets_[col] != sets_[col + 1] &&
        QRandomGenerator::global()->bounded(2)) {
      maze.cells[row][col].rightWall = false;
      int oldSet = sets_[col + 1];
      int newSet = sets_[col];
      for (int c = 0; c < cols; ++c) {
        if (sets_[c] == oldSet) sets_[c] = newSet;
      }
    }
  }
}

void Generator::createBottomPassages(MazeData& maze, int row, bool isLastRow) {
  if (isLastRow) return;

  int cols = maze.cols;

  std::unordered_map<int, std::vector<int>> setMembers;
  for (int c = 0; c < cols; ++c) {
    setMembers[sets_[c]].push_back(c);
  }

  for (auto& [setId, members] : setMembers) {
    std::shuffle(members.begin(), members.end(), *QRandomGenerator::global());

    maze.cells[row][members[0]].bottomWall = false;

    for (size_t i = 1; i < members.size(); ++i) {
      if (QRandomGenerator::global()->bounded(2)) {
        maze.cells[row][members[i]].bottomWall = false;
      }
    }
  }
}

void Generator::prepareNextRow(const MazeData& maze, int row) {
  // cells with bottom wall start fresh (set = 0), others keep their set
  for (int c = 0; c < maze.cols; ++c) {
    if (maze.cells[row][c].bottomWall) {
      sets_[c] = kNoSet;
    }
  }
}

void Generator::processLastRow(MazeData& maze) {
  int row = maze.rows - 1;

  // last row: merge all adjacent cells of different sets
  for (int col = 0; col < maze.cols - 1; ++col) {
    if (sets_[col] != sets_[col + 1]) {
      maze.cells[row][col].rightWall = false;
      int oldSet = sets_[col + 1];
      int newSet = sets_[col];
      for (int c = 0; c < maze.cols; ++c) {
        if (sets_[c] == oldSet) sets_[c] = newSet;
      }
    }
  }
  // last row always has bottom walls
}
