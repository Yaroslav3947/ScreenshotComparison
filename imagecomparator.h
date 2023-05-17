#pragma once

#include <QImage>
#include <QObject>


class ImageComparator : public QObject {
    Q_OBJECT
public:
    explicit ImageComparator(QObject *parent = nullptr);
    ~ImageComparator() = default;

    QString getDHash(const QImage &image);
    double getSimilarityPercentage(const QImage &image1, const QImage &image2);

private:
    int calculateHammingDistance(const QString &hash1, const QString &hash2);

};

