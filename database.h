#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include "imagemodel.h"


class Database : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ImageModel* imageModel READ imageModel CONSTANT)

public:
    explicit Database(QObject *parent = nullptr);

    ImageModel *imageModel() const;

private:
    ImageModel * _imageModel = nullptr;
    QSqlDatabase _database;

private:

    bool createTable(const QString & tableName);
    bool clearTable(const QString & tableName);
};

#endif // DATABASE_H
