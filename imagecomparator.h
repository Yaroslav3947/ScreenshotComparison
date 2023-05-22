#pragma once

#include <QImage>
#include <QObject>
#include <QCryptographicHash>

#include <opencv2/opencv.hpp>

class ImageComparator : public QObject
{
    Q_OBJECT
public:
    explicit ImageComparator(QObject *parent = nullptr);
    ~ImageComparator() = default;

    QByteArray calculateHash(const QImage &screenshot);
    double getSimilarityPercentage(const QImage &image1, const QImage &image2);
};
