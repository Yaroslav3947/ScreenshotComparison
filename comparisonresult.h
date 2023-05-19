#pragma once

#include <QImage>

class ComparisonResult {
public:
    ComparisonResult() = default;
    ~ComparisonResult() = default;
    ComparisonResult(const QImage &screenshot1,
                     const QImage &screenshot2,
                     const QByteArray &hash1,
                     const QByteArray &hash2);

    void setScreenshot1(QImage screenshot1);
    void setScreenshot2(QImage screenshot2);
    void setHash1(QByteArray hash1);
    void setHash2(QByteArray hash2);

    QImage getScreenshot1() const;
    QImage getScreenshot2() const;
    QByteArray getHash1() const;
    QByteArray getHash2() const;

private:
    QImage _screenshot1;
    QImage _screenshot2;
    QByteArray _hash1;
    QByteArray _hash2;
};
