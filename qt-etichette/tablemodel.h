#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QLabel>
#include <QAbstractTableModel>

struct Artist
{
    QString name;
    QString url;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel(const QList<Artist> &artists, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<Artist> artists;
};

#endif // TABLEMODEL_H
