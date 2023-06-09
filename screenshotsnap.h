#pragma once

#include <QObject>
#include <QScreen>
#include <QTimer>
#include <memory>

#include "QApplication"


class ScreenshotSnap : public QObject {
    Q_OBJECT

public:
    explicit ScreenshotSnap(QObject *parent = nullptr);
    ~ScreenshotSnap();

private:
    std::unique_ptr<QTimer> _timer;

public slots:
    void startSnap();
    void stopSnap();

private slots:
    void snapScreen();

signals:
    void newScreenshot(const QImage &screenshot);
};
