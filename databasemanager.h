#pragma once

#include <QObject>
#include <QBuffer>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>

#include "comparisonresult.h"


class DatabaseManager : public QObject {
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    void initialize();
    void storeComparisonResult(const QImage &screenshot1, const QImage &screenshot2,
                               const QByteArray &hash1, const QByteArray &hash2, const double &similarity);
    QList<ComparisonResult> getComparisonResults();

private:
    QSqlDatabase _database;

};
