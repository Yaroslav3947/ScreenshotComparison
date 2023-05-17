#pragma once

#include <QImage>

class ComparisonResult {
public:
    ComparisonResult() = default;
    ~ComparisonResult() = default;
    ComparisonResult(const QImage &screenshot1,
                     const QImage &screenshot2,
                     const QString &hash1,
                     const QString &hash2);

    void setScreenshot1(QImage screenshot1);
    void setScreenshot2(QImage screenshot2);
    void setHash1(QString hash1);
    void setHash2(QString hash2);

    QImage getScreenshot1() const;
    QImage getScreenshot2() const;
    QString getHash1() const;
    QString getHash2() const;

private:
    QImage _screenshot1;
    QImage _screenshot2;
    QString _hash1;
    QString _hash2;
};

