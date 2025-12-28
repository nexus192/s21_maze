#include "maze.h"
#include "src/lib/service/generator/generator.h"

MazeModel::MazeModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int MazeModel::rowCount(const QModelIndex &) const
{
    return maze_.rows * maze_.cols;
}

QVariant MazeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !maze_.isGenerated)
        return {};

    int flatIndex = index.row();
    int r = flatIndex / maze_.cols;
    int c = flatIndex % maze_.cols;

    if (r >= maze_.rows || c >= maze_.cols)
        return {};

    const auto &cell = maze_.cells[r][c];

    switch (role) {
    case RightWallRole:
        return cell.rightWall;
    case BottomWallRole:
        return cell.bottomWall;
    }
    return {};
}

QHash<int, QByteArray> MazeModel::roleNames() const
{
    return {{RightWallRole, "rightWall"}, {BottomWallRole, "bottomWall"}};
}

int MazeModel::rows() const { return maze_.rows; }
int MazeModel::cols() const { return maze_.cols; }
bool MazeModel::isGenerated() const { return maze_.isGenerated; }

void MazeModel::generate(int rows, int cols)
{
    beginResetModel();
    Generator gen;
    gen.generate(maze_, rows, cols);
    endResetModel();
    emit mazeChanged();
}

void MazeModel::clear()
{
    beginResetModel();
    maze_ = {};
    endResetModel();
    emit mazeChanged();
}
