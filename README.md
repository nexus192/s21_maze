# s21_maze

A Qt6/QML application for generating and solving perfect mazes using Eller's algorithm and BFS pathfinding.

## Features

- **Generate perfect mazes** (4-50×50) with no isolated areas or loops
- **Load/save mazes** from text files
- **Visual pathfinding** with BFS algorithm
- **Interactive point selection** for start/end positions
- Cross-platform (macOS, Linux)

## Building

```bash
make              # build, install to bin/, and run tests
make install      # build and install only
make tests        # run unit tests
make coverage     # generate test coverage report
make clean        # remove build artifacts
make uninstall    # remove installed files
make dist         # create distribution tarball
```

Executable location: `bin/apps21_maze` (or `bin/apps21_maze.app` on macOS)

## Usage

### Generate a maze

1. Click "Generate maze"
2. Enter dimensions (5-50 rows/cols)
3. Click "Set Start" and click a cell
4. Click "Set End" and click a cell
5. Path automatically displayed if solution exists

### Load a maze

1. Click "Load from file"
2. Select a `.txt` file in the maze format (see below)
3. Set start/end points to solve

### Save a maze

1. Generate or load a maze
2. Click "Save"
3. Choose location for `.txt` file

## Maze File Format

```
<rows> <cols>
<right_walls_matrix>

<bottom_walls_matrix>
```

Example (4×4 maze):

```
4 4
0 0 1 0
1 0 1 1
0 1 0 1
0 0 0 1

1 0 1 0
0 0 1 0
1 1 0 1
1 1 1 1
```

- `1` = wall present, `0` = no wall
- Right walls matrix: wall to the right of each cell
- Bottom walls matrix: wall below each cell
- See `examples/` for more samples

## Architecture

- **Generator**: Eller's algorithm for perfect maze generation
- **Solver**: BFS pathfinding with Qt integration
- **Model**: QAbstractListModel for QML data binding
- **I/O**: Asynchronous file operations with QtConcurrent
- **UI**: QML with custom components

## Requirements

- Qt 6.5+
- CMake 3.16+
- C++20 compiler
- gcovr (optional, for coverage reports)

## Testing

Unit tests cover:

- Maze generation properties (spanning tree, connectivity)
- Pathfinding correctness
- File I/O operations

Run with: `make tests` or `make coverage`
