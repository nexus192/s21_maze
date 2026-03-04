#ifndef S21_GENERATOR_H_
#define S21_GENERATOR_H_

#include <vector>

struct MazeData;

class Generator {
 public:
  void generate(MazeData& maze, int rows, int cols);

 private:
  static constexpr int kNoSet = 0;

  std::vector<int> sets_;  // set id for each cell in current row
  int nextSetId_ = 1;

  void assignNewSets(int cols);
  void mergeRandomRight(MazeData& maze, int row);
  void createBottomPassages(MazeData& maze, int row, bool isLastRow);
  void prepareNextRow(const MazeData& maze, int row);
  void processLastRow(MazeData& maze);
};

#endif  // S21_GENERATOR_H_
