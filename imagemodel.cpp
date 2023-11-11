#include "imagemodel.h"

#include <QHash>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

ImageModel::ImageModel(const QString & tableName, QObject *parent) : QSqlTableModel(parent, QSqlDatabase::database("test_yaware_connection"))
{
    setTable(tableName);
    if(!select()){
        qDebug() << "failed to select data from" << tableName << lastError();
    }
    qDebug() << rowCount();
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    _sortModel->setSourceModel(this);
    _sortModel->setDynamicSortFilter(true);
    _sortModel->setSortRole(Id);
    _sortModel->sort(0, Qt::SortOrder::DescendingOrder);

}

QVariant ImageModel::data(const QModelIndex &index, int role) const
{
//    int row = index.row();
//    if(row < 0 || row > )
    if(!checkIndex(index))
        return {};
    if (role < Qt::UserRole)
            return QSqlTableModel::data(index, role);

    const QSqlRecord sqlRecord = record(index.row());
    auto value = sqlRecord.value(role - Qt::UserRole);
    switch (role) {
    case Image:
        return QString("data:image/png;base64,") + QString::fromUtf8(value.toByteArray());
    default:
        return value;
    }

}

QHash<int, QByteArray> ImageModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        {Image, "image"},
        {Diff, "diff"}
    };
    return roles;
}

QModelIndex ImageModel::indexForRole(const QModelIndex& index, int role) const
{
    int row = index.row();
    int column = index.column();
    if (role > Qt::UserRole) {
        column = role - Qt::UserRole - 1;
    }
    return this->index(row, column);
}

QSortFilterProxyModel *ImageModel::sortModel() const
{
    return _sortModel;
}
