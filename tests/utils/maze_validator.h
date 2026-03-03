#include "src/lib/model/maze.h"

class MazeValidator {
 public:
  static bool IsMazePerfect(const MazeData& maze);

 private:
  static int CountPassages(const MazeData& maze);
  static int CountReachableCells(const MazeData& maze);
  static bool BoundaryWallsIntact(const MazeData& maze);
};
