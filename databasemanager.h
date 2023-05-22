#pragma once

#include <QObject>
#include <QBuffer>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>

#include "comparisonresult.h"
#include "qapplication.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    void initialize();
    void storeScreenshot(const QImage &screenshot, const QByteArray &hash);
    void storeComparisonResult(const ComparisonResult &comparisonResult, const double &similarity);
    QImage getLastScreenshot();

private:
    QSqlDatabase _database;
};
