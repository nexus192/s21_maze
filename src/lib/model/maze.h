#pragma once

#include <QAbstractTableModel>

struct MazeCell
{
    bool rightWall;
    bool bottomWall;
};

struct MazeData
{
    int rows{0};
    int cols{0};
    bool isGenerated{false};

    std::vector<std::vector<MazeCell>> cells;
};

class MazeModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(int rows READ rows NOTIFY mazeChanged)
    Q_PROPERTY(int cols READ cols NOTIFY mazeChanged)
    Q_PROPERTY(bool isGenerated READ isGenerated NOTIFY mazeChanged)

public:
    enum Roles { RightWallRole = Qt::UserRole + 1, BottomWallRole };

    explicit MazeModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;

    int rows() const;
    int cols() const;
    bool isGenerated() const;

    Q_INVOKABLE void generate(int rows, int cols);
    Q_INVOKABLE void clear();

signals:
    void mazeChanged();

private:
    MazeData maze_;
};
