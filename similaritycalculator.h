#pragma once

#include <QImage>
#include <QObject>

class SimilarityCalculator : public QObject {
    Q_OBJECT
public:
    explicit SimilarityCalculator(QObject *parent = nullptr);
    ~SimilarityCalculator() = default;
    double getSimilarityPercentage(const QImage &image1, const QImage &image2);

};
