#pragma once

#include <QImage>
#include <QObject>
#include <QCryptographicHash>


class ImageComparator : public QObject {
    Q_OBJECT
public:
    explicit ImageComparator(QObject *parent = nullptr);
    ~ImageComparator() = default;

    double getSimilarityPercentage(const QImage &image1, const QImage &image2);
    QByteArray calculateHash(const QImage &screenshot);

private:
    int calculateHammingDistance(const QString &hash1, const QString &hash2);


};

