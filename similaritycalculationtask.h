#pragma once

#include <QDebug>
#include <QObject>
#include <QPixmap>
#include <QRunnable>

#include "comparisonresult.h"
#include "imagecomparator.h"

class SimilarityCalculationTask : public QObject, public QRunnable
{
    Q_OBJECT

public:
    SimilarityCalculationTask(const QImage &newScreenshot, const QPixmap &currentPixmap)
        : _newScreenshot(newScreenshot)
        , _currentPixmap(currentPixmap)
    {
        _imageComparator = std::make_unique<ImageComparator>();
    }

    void run() override
    {
        double similarityPercentage
            = _imageComparator->getSimilarityPercentage(_newScreenshot, _currentPixmap.toImage());

        QByteArray hash1 = _imageComparator->calculateHash(_newScreenshot);
        QByteArray hash2 = _imageComparator->calculateHash(_currentPixmap.toImage());

        ComparisonResult result;
        result.setScreenshot1(_newScreenshot);
        result.setScreenshot2(_currentPixmap.toImage());
        result.setHash1(hash1);
        result.setHash2(hash2);

        emit similarityCalculationFinished(result, similarityPercentage);
    }

signals:
    void similarityCalculationFinished(const ComparisonResult &result, const double &similarityPercentage);

private:
    QImage _newScreenshot;
    QPixmap _currentPixmap;
    std::unique_ptr<ImageComparator> _imageComparator;
};
