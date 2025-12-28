#pragma once

#include <QAbstractListModel>

struct MazeCell {
  bool rightWall;
  bool bottomWall;
};

struct MazeData {
  int rows{0};
  int cols{0};
  bool isGenerated{false};

  std::vector<std::vector<MazeCell>> cells;
};

class MazeModel : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(int rows READ rows NOTIFY mazeChanged)
  Q_PROPERTY(int cols READ cols NOTIFY mazeChanged)
  Q_PROPERTY(bool isGenerated READ isGenerated NOTIFY mazeChanged)

 public:
  enum Roles { RightWallRole = Qt::UserRole + 1, BottomWallRole };

  explicit MazeModel(QObject* parent = nullptr);
  const MazeData& mazeData() const { return maze_; }
  int rowCount(const QModelIndex& = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  int rows() const;
  int cols() const;
  bool isGenerated() const;

  void setMazeData(MazeData&& data);
  Q_INVOKABLE void generate(int rows, int cols);
  Q_INVOKABLE void clear();

 signals:
  void mazeChanged();

 private:
  MazeData maze_;
};
