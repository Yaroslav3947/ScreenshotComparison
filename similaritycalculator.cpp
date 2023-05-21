#include "similaritycalculator.h"

SimilarityCalculator::SimilarityCalculator(QObject *parent)
    : QObject{parent}
{}

double SimilarityCalculator::getSimilarityPercentage(const QImage &image1, const QImage &image2)
{
    // Convert QImage to cv::Mat
    cv::Mat cvImage1(image1.height(),
                     image1.width(),
                     CV_8UC4,
                     const_cast<uchar *>(image1.bits()),
                     image1.bytesPerLine());
    cv::Mat cvImage2(image2.height(),
                     image2.width(),
                     CV_8UC4,
                     const_cast<uchar *>(image2.bits()),
                     image2.bytesPerLine());

    // Convert images to grayscale
    cv::Mat grayImage1, grayImage2;
    cv::cvtColor(cvImage1, grayImage1, cv::COLOR_BGRA2GRAY);
    cv::cvtColor(cvImage2, grayImage2, cv::COLOR_BGRA2GRAY);

    // Perform template matching
    cv::Mat result;
    cv::matchTemplate(grayImage1, grayImage2, result, cv::TM_CCOEFF_NORMED);

    // Get the maximum matching value
    double maxSimilarity;
    cv::minMaxLoc(result, nullptr, &maxSimilarity);

    // Convert similarity to percentage
    double similarityPercentage = maxSimilarity * 100.0;

    return similarityPercentage;
}
