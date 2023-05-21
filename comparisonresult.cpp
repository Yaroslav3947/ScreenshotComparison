#include "comparisonresult.h"

ComparisonResult::ComparisonResult(const QImage &screenshot1, const QImage &screenshot2, const QByteArray &hash1, const QByteArray &hash2)
    : _screenshot1(screenshot1), _screenshot2(screenshot2), _hash1(hash1), _hash2(hash2) {
}

void ComparisonResult::setScreenshot1(const QImage &screenshot1)
{
    _screenshot1 = screenshot1;
}

void ComparisonResult::setScreenshot2(const QImage &screenshot2)
{
    _screenshot2 = screenshot2;
}

void ComparisonResult::setHash1(const QByteArray &hash1)
{
    _hash1 = hash1;
}

void ComparisonResult::setHash2(const QByteArray &hash2)
{
    _hash2 = hash2;
}

QImage ComparisonResult::getScreenshot1() const {
    return _screenshot1;
}

QImage ComparisonResult::getScreenshot2() const {
    return _screenshot2;
}

QByteArray ComparisonResult::getHash1() const {
    return _hash1;
}

QByteArray ComparisonResult::getHash2() const {
    return _hash2;
}
