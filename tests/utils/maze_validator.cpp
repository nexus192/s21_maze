#include "maze_validator.h"

bool MazeValidator::IsMazePerfect(const MazeData& maze) {
  if (!BoundaryWallsIntact(maze)) return false;
  if (CountReachableCells(maze) != maze.rows * maze.cols) return false;
  if (CountPassages(maze) != maze.rows * maze.cols - 1) return false;
  return true;
}

// helper: count reachable cells from (0,0) using BFS
int MazeValidator::CountReachableCells(const MazeData& maze) {
  if (maze.rows == 0 || maze.cols == 0) return 0;

  std::vector<std::vector<bool>> visited(maze.rows,
                                         std::vector<bool>(maze.cols, false));
  std::queue<std::pair<int, int>> queue;

  queue.push({0, 0});
  visited[0][0] = true;
  int count = 1;

  auto try_visit = [&visited, &queue, &count](int r, int c) {
    if (!visited[r][c]) {
      visited[r][c] = true;
      queue.push({r, c});
      ++count;
    }
  };

  while (!queue.empty()) {
    auto [r, c] = queue.front();
    queue.pop();

    // right
    if (c + 1 < maze.cols && !maze.cells[r][c].rightWall) {
      try_visit(r, c + 1);
    }
    // left
    if (c > 0 && !maze.cells[r][c - 1].rightWall) {
      try_visit(r, c - 1);
    }
    // down
    if (r + 1 < maze.rows && !maze.cells[r][c].bottomWall) {
      try_visit(r + 1, c);
    }
    // up
    if (r > 0 && !maze.cells[r - 1][c].bottomWall) {
      try_visit(r - 1, c);
    }
  }

  return count;
}

// helper: count total passages (absent walls) in maze
// for a perfect maze (spanning tree): passages = cells - 1
int MazeValidator::CountPassages(const MazeData& maze) {
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
bool MazeValidator::BoundaryWallsIntact(const MazeData& maze) {
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
