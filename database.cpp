#include "database.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

namespace {
    const QString _connectionName = QStringLiteral("test_yaware_connection");
    const QString _databaseName = QStringLiteral("test_yaware_database.db");
    const QString _tableName = "screenshots";
}

Database::Database(QObject *parent) : QObject(parent)
{
    _database = QSqlDatabase::database(_connectionName);
    if(!_database.isValid())
        _database = QSqlDatabase::addDatabase("QSQLITE", _connectionName);

    _database.setDatabaseName(_databaseName);

    if(!_database.open()) {
        qDebug() << "not connected, error" << _database.lastError() << _database.databaseName();
    }else {
        qDebug() << "connected to" << _database.databaseName();
    }

    if(!_database.tables().contains(_tableName)) {
        createTable(_tableName);
    }

//    clearTable(_tableName);

    _imageModel = new ImageModel(_tableName, this);
}

ImageModel *Database::imageModel() const
{
    return _imageModel;
}

bool Database::createTable(const QString &tableName)
{
    QSqlQuery query(_database);
    if(!query.exec( "CREATE TABLE " + tableName + " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "image BLOB            NOT NULL,"
                                  "diff INTEGER          NOT NULL,"
                                  "hash BLOB     NOT NULL"
                            " )"
                    )){
        qDebug() << "DataBase: error of create " << tableName;
        qDebug() << query.lastError().text() << query.lastQuery();
        return false;
    }
    return true;
}

bool Database::clearTable(const QString &tableName)
{
    QSqlQuery query(_database);
    if(!query.exec( "DELETE FROM " + tableName)){
        qDebug() << "DataBase: error of clear " << tableName;
        qDebug() << query.lastError().text() << query.lastQuery();
        return false;
    }
    return true;
}
