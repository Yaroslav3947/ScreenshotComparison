#include "similaritycalculator.h"

SimilarityCalculator::SimilarityCalculator(QObject *parent)
    : QObject{parent} {
}

double SimilarityCalculator::getSimilarityPercentage(const QImage &image1, const QImage &image2) {
    if (image1.size() != image2.size()) {
        return -1.0;
    }

    auto count = 0;
    for(auto x = 0; x < image1.width(); x++) {
        for(auto y = 0; y < image1.height(); y++) {
            QColor currentPixel = image1.pixel(x, y);
            QColor previousPixel = image2.pixel(x, y);
            if(currentPixel == previousPixel) {
                count++;
            }
        }
    }

    double similarityPercentage = static_cast<double>(count) / static_cast<double>(image1.width() * image1.height()) * 100.0;

    emit similarityPercentageCalculated(similarityPercentage);
    emit calculationFinished();

    return similarityPercentage;
}

