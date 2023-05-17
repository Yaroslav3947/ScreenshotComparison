#include <QApplication>

#include "mainwindow.h"
#include "imagecomparator.h"
#include "screenshotsnap.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ScreenshotSnap snap;

    QObject::connect(&snap, &ScreenshotSnap::newScreenshot, [&](const QImage &screenshot) {
        static QImage previousScreenshot;
        if (previousScreenshot.isNull()) {
            previousScreenshot = screenshot;
        } else {
            ImageComparator comparator;
            double similarityPercentage = comparator.getSimilarityPercentage(previousScreenshot, screenshot);
            qDebug() << "Similarity percentage: " << similarityPercentage;
            previousScreenshot = screenshot;
        }
    });

    snap.startSnap();

    return a.exec();
}
