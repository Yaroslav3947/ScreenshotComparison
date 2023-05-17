#include "imagecomparator.h"


ImageComparator::ImageComparator(QObject *parent)
    : QObject{parent} {
}

double ImageComparator::getSimilarityPercentage(const QImage &image1, const QImage &image2) {
    QString hash1 = getDHash(image1);
    QString hash2 = getDHash(image2);

    auto distance = calculateHammingDistance(hash1, hash2);

    return (1.0 - (static_cast<double>(distance) / hash1.length())) * 100;
}

// Difference value hash
QString ImageComparator::getDHash(const QImage &image) {
    // convetring to grayscalePicture. This changes the hash from 72 pixels to a total of 72 colors
    QImage grayscalePicture = image.convertToFormat(QImage::Format_Grayscale8);

    // rescale picture to 9x8 in order to have 72 pixels
    QImage resizedPicture = grayscalePicture.scaled(9, 8, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QString dHash;

    for (auto row = 0; row < resizedPicture.height(); row++) {
        for (auto col = 0; col < resizedPicture.width() - 1; col++) {
            QRgb pixel1 = resizedPicture.pixel(col, row);
            QRgb pixel2 = resizedPicture.pixel(col + 1, row);

            (qRed(pixel1) < qRed(pixel2)) ? dHash.append('1') : dHash.append('0');
        }
    }

    return dHash;
}

int ImageComparator::calculateHammingDistance(const QString &hash1, const QString &hash2) {

    if(hash1.length() != hash2.length()) {
        return 0;
    }

    auto difference = 0;

    for (auto i = 0; i < hash1.length(); i++) {
        difference += (hash1[i].digitValue() ^ hash2[i].digitValue()); // using XOR to determine difference
    }

    return difference;
}


