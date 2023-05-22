#include "imagecomparator.h"


ImageComparator::ImageComparator(QObject *parent)
    : QObject{parent} {
}
QByteArray ImageComparator::calculateHash(const QImage &screenshot) {
    auto byteCount = screenshot.width() * screenshot.height() * screenshot.depth() / 8;
    QByteArray pixelData(reinterpret_cast<const char*>(screenshot.constBits()), byteCount);
    return QCryptographicHash::hash(pixelData, QCryptographicHash::Md5).toHex();
}

double ImageComparator::getSimilarityPercentage(const QImage &image1, const QImage &image2)
{
    // Convert QImage objects to cv::Mat format
    cv::Mat cvImage1(image1.height(),
                     image1.width(),
                     CV_8UC4,
                     const_cast<uchar *>(image1.constBits()),
                     image1.bytesPerLine());
    cv::Mat cvImage2(image2.height(),
                     image2.width(),
                     CV_8UC4,
                     const_cast<uchar *>(image2.constBits()),
                     image2.bytesPerLine());

    // Convert color images to grayscale
    cv::Mat grayImage1, grayImage2;
    cv::cvtColor(cvImage1, grayImage1, cv::COLOR_RGBA2GRAY);
    cv::cvtColor(cvImage2, grayImage2, cv::COLOR_RGBA2GRAY);

    // Compute the absolute difference between the grayscale images
    cv::Mat diffImage;
    cv::absdiff(grayImage1, grayImage2, diffImage);

    // Threshold the difference image to obtain a binary image
    cv::threshold(diffImage, diffImage, 30, 255, cv::THRESH_BINARY);

    // Count the number of non-zero pixels in the difference image
    int nonZeroPixels = cv::countNonZero(diffImage);

    // Calculate the similarity percentage by comparing the differing pixels with the total number of pixels
    double similarityPercentage = 100.0 - (nonZeroPixels * 100.0) / diffImage.total();

    return similarityPercentage;
}
