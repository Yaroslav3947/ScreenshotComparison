#include <QApplication>

#include "mainwindow.h"
#include "imagecomparator.h"
#include "screenshotsnap.h"
#include "databasemanager.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
//    w.show();

//    ScreenshotSnap snap;

//    QObject::connect(&snap, &ScreenshotSnap::newScreenshot, [&](const QImage &screenshot) {
//        static QImage previousScreenshot;
//        if (previousScreenshot.isNull()) {
//            previousScreenshot = screenshot;
//        } else {
//            ImageComparator comparator;
//            double similarityPercentage = comparator.getSimilarityPercentage(previousScreenshot, screenshot);
//            double similarityPercentage2 = comparator.getSimilarityPercentage2(previousScreenshot, screenshot);
//            qDebug() << "Similarity percentage: " << similarityPercentage;
//            qDebug() << "Similarity percentage2: " << similarityPercentage2;
//            qDebug() << comparator.calculateHash(screenshot);
//            qDebug() << comparator.calculateHash(previousScreenshot);
//            previousScreenshot = screenshot;
//        }
//    });

//    snap.startSnap();

    ImageComparator comparator;
    DatabaseManager databaseManager;
    QImage screenshot1("F:/5.jpg");
    QImage screenshot2("F:/6.jpg");
    QByteArray hash1 = comparator.calculateHash(screenshot1);
    QByteArray hash2 = comparator.calculateHash(screenshot2);
    double similarityPercentage = comparator.getSimilarityPercentage(screenshot1, screenshot2);
    databaseManager.initialize();
//    databaseManager.storeComparisonResult(screenshot1, screenshot2, hash1, hash2, similarityPercentage);
    QList<ComparisonResult> comparisonResult = databaseManager.getComparisonResults();
    for(const auto &x: comparisonResult) {
        qDebug() << x.getScreenshot1() << x.getHash1();
    }

    return a.exec();
}
