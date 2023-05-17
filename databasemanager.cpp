#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent} {
}

DatabaseManager::~DatabaseManager() {
    if(_database.isOpen()) {
        _database.close();
    }
}

void DatabaseManager::initialize() {
    _database = QSqlDatabase::addDatabase("QSQLITE");

    ////TODO: hardcode, find out the solution
    _database.setDatabaseName("C:/Users/Yaroslav/Documents/ScreenshotComparison/snapshots.db");

    if (!_database.open()) {
        QMessageBox::warning(nullptr, "Error", "Problems opening snapshots.db");
        return;
    }
}

void DatabaseManager::storeComparisonResult(const QImage &screenshot1, const QImage &screenshot2,
                                            const QString &hash1, const QString &hash2, const double &similarity) {
    QSqlDatabase database = QSqlDatabase::database();

    if (!database.isValid()) {
        QMessageBox::warning(nullptr, "Error", "Problems opening snapshots.db");
        return;
    }

    QSqlQuery query(database);
    query.prepare("INSERT INTO snapshots (image1, image2, hash1, hash2, similarity) "
                  "VALUES (:image1, :image2, :hash1, :hash2, :similarity)");

    QByteArray byteArray1;
    QBuffer buffer1(&byteArray1);
    buffer1.open(QIODevice::WriteOnly);
    screenshot1.save(&buffer1, "PNG");

    QByteArray byteArray2;
    QBuffer buffer2(&byteArray2);
    buffer2.open(QIODevice::WriteOnly);
    screenshot2.save(&buffer2, "PNG");

    query.bindValue(":image1", byteArray1);
    query.bindValue(":image2", byteArray2);
    query.bindValue(":hash1", hash1);
    query.bindValue(":hash2", hash2);
    query.bindValue(":similarity", similarity);

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Problems executing query");
        return;
    }
}

QList<ComparisonResult> DatabaseManager::getComparisonResults() {
    QList<ComparisonResult> results;

    QSqlQuery query(_database);
    query.prepare("SELECT image1, image2, hash1, hash2, similarity FROM snapshots");

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Problems getting data from snapshots.db");
        return results;
    }

    while (query.next()) {
        ComparisonResult result;

        QByteArray image1Data = query.value("image1").toByteArray();
        QByteArray image2Data = query.value("image2").toByteArray();

        QImage image1;
        image1.loadFromData(image1Data);

        QImage image2;
        image2.loadFromData(image2Data);

        QString hash1 = query.value("hash1").toString();
        QString hash2 = query.value("hash2").toString();

        result.setScreenshot1(image1);
        result.setScreenshot2(image2);
        result.setHash1(hash1);
        result.setHash2(hash2);

        results.append(result);
    }

    return results;
}
