#include "imagecomparator.h"


ImageComparator::ImageComparator(QObject *parent)
    : QObject{parent} {
}
QByteArray ImageComparator::calculateHash(const QImage &screenshot) {
    auto byteCount = screenshot.width() * screenshot.height() * screenshot.depth() / 8;
    QByteArray pixelData(reinterpret_cast<const char*>(screenshot.constBits()), byteCount);
    return QCryptographicHash::hash(pixelData, QCryptographicHash::Md5).toHex();
}
