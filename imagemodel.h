#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSortFilterProxyModel>

class ImageModel : public QSqlTableModel
{
    Q_OBJECT

    Q_PROPERTY(QSortFilterProxyModel* sortModel READ sortModel CONSTANT)

public:
    enum ImageModelRoles {Id = Qt::UserRole, Image, Diff};
    explicit ImageModel(const QString &tableName, QObject *parent = nullptr);

    QModelIndex indexForRole(const QModelIndex& index, int role) const;


    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QSortFilterProxyModel *sortModel() const;

private:
    QSortFilterProxyModel *_sortModel = new QSortFilterProxyModel(this);
};

#endif // IMAGEMODEL_H
