#include "databasemanager.h"


DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent} {
    // Register ComparisonResult type
    qRegisterMetaType<ComparisonResult>("ComparisonResult");
}

DatabaseManager::~DatabaseManager()
{
    if(_database.isOpen()) {
        _database.close();
    }
}

void DatabaseManager::initialize()
{
    _database = QSqlDatabase::addDatabase("QSQLITE");
    _database.setDatabaseName("snapshots.db");

    if (!_database.open()) {
        QMessageBox::warning(nullptr, "Error", "Problems initializing snapshots.db");
        return;
    }

    QSqlQuery query(_database);

    query.exec("CREATE TABLE IF NOT EXISTS comparison_results "
               "(id INTEGER PRIMARY KEY AUTOINCREMENT, image1ID INTEGER, image2ID INTEGER, "
               "similarity REAL)");

    query.exec("CREATE TABLE IF NOT EXISTS images "
               "(id INTEGER PRIMARY KEY AUTOINCREMENT, image BLOB, hash TEXT)");
}

void DatabaseManager::storeScreenshot(const QImage &screenshot, const QByteArray &hash)
{
    QSqlDatabase database = QSqlDatabase::database();

    if (!database.isValid()) {
        QMessageBox::warning(nullptr, "Error", "Problems opening snapshots.db");
        return;
    }

    QSqlQuery query(database);

    query.prepare("INSERT INTO images (image, hash) VALUES (:image, :hash)");
    QByteArray byteArray1;
    QBuffer buffer1(&byteArray1);
    buffer1.open(QIODevice::WriteOnly);
    screenshot.save(&buffer1, "PNG");
    query.bindValue(":image", byteArray1);
    query.bindValue(":hash", hash);

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Problems storing image and hash");
        return;
    }
}

void DatabaseManager::storeComparisonResult(const ComparisonResult &comparisonResult,
                                            const double &similarityPercentage)
{
    QSqlDatabase database = QSqlDatabase::database();

    if (!database.isValid()) {
        QMessageBox::warning(nullptr, "Error", "Problems opening snapshots.db");
        return;
    }

    QSqlQuery query(database);

    query.prepare("INSERT INTO images (image, hash) VALUES (:image, :hash)");
    QByteArray byteArray1;
    QBuffer buffer1(&byteArray1);
    buffer1.open(QIODevice::WriteOnly);
    comparisonResult.getScreenshot1().save(&buffer1, "PNG");
    query.bindValue(":image", byteArray1);
    query.bindValue(":hash", comparisonResult.getHash1());

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Problems storing image1 and hash1");
        return;
    }

    int image1ID = query.lastInsertId().toInt();

    query.prepare("INSERT INTO images (image, hash) VALUES (:image, :hash)");
    QByteArray byteArray2;
    QBuffer buffer2(&byteArray2);
    buffer2.open(QIODevice::WriteOnly);
    comparisonResult.getScreenshot2().save(&buffer2, "PNG");
    query.bindValue(":image", byteArray2);
    query.bindValue(":hash", comparisonResult.getHash2());

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Problems storing image2 and hash2");
        return;
    }

    int image2ID = query.lastInsertId().toInt();

    query.prepare("INSERT INTO comparison_results (image1ID, image2ID, similarity) "
                  "VALUES (:image1ID, :image2ID, :similarity)");
    query.bindValue(":image1ID", image1ID);
    query.bindValue(":image2ID", image2ID);
    query.bindValue(":similarity", similarityPercentage);

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Problems storing comparison result");
        return;
    }
}

QImage DatabaseManager::getLastScreenshot()
{
    QImage lastScreenshot = QImage();

    QSqlQuery query(_database);
    query.prepare("SELECT i.image "
                  "FROM comparison_results cr "
                  "JOIN images i ON cr.image2ID = i.id "
                  "ORDER BY cr.id DESC "
                  "LIMIT 1");

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Error", "Problems getting data from snapshots.db");
        return lastScreenshot;
    }

    if (query.next()) {
        QByteArray imageData = query.value("image").toByteArray();
        lastScreenshot.loadFromData(imageData);
    }

    return lastScreenshot;
}
