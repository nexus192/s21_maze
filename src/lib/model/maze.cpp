#include "maze.h"
#include "src/lib/service/generator/generator.h"

MazeModel::MazeModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

int MazeModel::rowCount(const QModelIndex &) const
{
    return maze_.rows;
}

int MazeModel::columnCount(const QModelIndex &) const
{
    return maze_.cols;
}

QVariant MazeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !maze_.isGenerated)
        return {};

    const auto &cell = maze_.cells[index.row()][index.column()];

    switch (role) {
    case RightWallRole:
        return cell.rightWall;
    case BottomWallRole:
        return cell.bottomWall;
    }
    return {};
}

bool MazeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    auto &cell = maze_.cells[index.row()][index.column()];

    switch (role) {
    case RightWallRole:
        cell.rightWall = value.toBool();
        break;
    case BottomWallRole:
        cell.bottomWall = value.toBool();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags MazeModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEnabled;
}

QHash<int, QByteArray> MazeModel::roleNames() const
{
    return {{RightWallRole, "rightWall"}, {BottomWallRole, "bottomWall"}};
}

int MazeModel::rows() const
{
    return maze_.rows;
}
int MazeModel::cols() const
{
    return maze_.cols;
}
bool MazeModel::isGenerated() const
{
    return maze_.isGenerated;
}

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
