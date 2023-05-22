#include "screenshotsnap.h"
#include "qimage.h"
#include "qpixmap.h"

ScreenshotSnap::ScreenshotSnap(QObject *parent)
    : QObject(parent)
{
    _timer = std::make_unique<QTimer>(this);
    connect(_timer.get(), &QTimer::timeout, this, &ScreenshotSnap::snapScreen);
}

ScreenshotSnap::~ScreenshotSnap() {
    stopSnap();
}

void ScreenshotSnap::startSnap() {
    const int MINUTE_IN_MILLISECONDS = 60000;

    snapScreen();
    _timer->start(MINUTE_IN_MILLISECONDS);
}

void ScreenshotSnap::stopSnap() {
    _timer->stop();
}

void ScreenshotSnap::snapScreen() {
    QScreen *screen = QGuiApplication::primaryScreen();
    if(screen) {
        QImage screenshot = screen->grabWindow(0).toImage();
        emit newScreenshot(screenshot);
    }
}
