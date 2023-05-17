#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>


#include "screenshotsnap.h"
#include "imagecomparator.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startSnap();
    void stopSnap();
    void manageNewScreenshot(const QImage &screenshot);

private:
    void connectSignalsAndSlots();

    std::unique_ptr<ImageComparator> _imageComparator;
    std::unique_ptr<ScreenshotSnap> _screenshotSnap;

    Ui::MainWindow *ui;
};
