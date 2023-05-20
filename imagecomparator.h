#pragma once

#include <QImage>
#include <QObject>
#include <QCryptographicHash>


class ImageComparator : public QObject {
    Q_OBJECT
public:
    explicit ImageComparator(QObject *parent = nullptr);
    ~ImageComparator() = default;

    QByteArray calculateHash(const QImage &screenshot);
};
