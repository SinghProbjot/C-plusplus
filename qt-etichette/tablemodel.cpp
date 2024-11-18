#include "tablemodel.h"

TableModel::TableModel(const QList<Artist> &artists, QObject *parent)
    : QAbstractTableModel(parent), artists(artists)
{
}
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return tr("Name");
                case 1:
                    return tr("Wiki");
                default:
                    break;
            }
        }
        return QVariant();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
   return parent.isValid() ? 0 : artists.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 2;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= artists.size() || index.row() < 0)
            return QVariant();

        if (role == Qt::DisplayRole) {
            const auto &artist = artists.at(index.row());

            switch (index.column()) {
                case 0:
                    return artist.name;
                case 1:
//                    QLabel urlLabel;
//                    urlLabel.setText("<a href=\""+ artist.url +"\">text</a>");
//                    urlLabel.setTextInteractionFlags(Qt::TextBrowserInteraction);
//                    return urlLabel;
                    return artist.url;
                default:
                    break;
            }
        }
    return QVariant();
}
