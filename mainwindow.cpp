#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _imageComparator = std::make_unique<ImageComparator>(this);
    _screenshotSnap = std::make_unique<ScreenshotSnap>(this);

    connectSignalsAndSlots();

}


void MainWindow::connectSignalsAndSlots() {
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startSnap);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopSnap);

    connect(_screenshotSnap.get(), &ScreenshotSnap::newScreenshot, this, &MainWindow::manageNewScreenshot);

}

void MainWindow::startSnap() {
    _screenshotSnap->startSnap();
}

void MainWindow::stopSnap() {
    _screenshotSnap->stopSnap();
}

void MainWindow::manageNewScreenshot(const QImage& screenshot) {
    QPixmap currentPixmap = ui->currentScreenshotLabel->pixmap();

    ui->currentScreenshotLabel->setPixmap(QPixmap::fromImage(screenshot));
    ui->currentScreenshotLabel->setScaledContents(true);

    ui->previousScreenshotLabel->setPixmap(currentPixmap);
    ui->previousScreenshotLabel->setScaledContents(true);


    double similarityPercentage = _imageComparator->getSimilarityPercentage(screenshot, currentPixmap.toImage());

    ui->similarityPercentageLabel->setText(QString("Similarity: %1%").arg(similarityPercentage));
}


MainWindow::~MainWindow() {
    stopSnap();
    delete ui;
}

