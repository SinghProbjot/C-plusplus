#include "tablemodel.h"

TableModel::TableModel(const QList<Activity> &activities, QObject *parent)
    : QAbstractTableModel(parent), activities(activities)
{
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Tipo");
        case 1:
            return tr("Titolo");
        case 2:
            return tr("Ora inizio");
        case 3:
            return tr("Ora fine");
        case 4:
            return tr("Si ripete");
        default:
            break;
        }
    }
    return QVariant();
}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


int TableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : activities.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 5;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= activities.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto &activity = activities.at(index.row());

        switch (index.column()) {
        case 0:
            return activity.type;
        case 1:
            return activity.title;
        case 2:
            return activity.startHour.toString("HH:mm");
        case 3:
            return activity.endHour.toString("HH:mm");
        case 4:
            return activity.recursive;
        default:
            break;
        }
    }
    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool TableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

bool TableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
    return true;
}

Activity TableModel::getActivity(int idx)
{
    return activities[idx];
}

