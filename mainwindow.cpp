#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _imageComparator = std::make_unique<ImageComparator>(this);
    _screenshotSnap = std::make_unique<ScreenshotSnap>(this);
    _databaseManager = std::make_unique<DatabaseManager>(this);

    _databaseManager->initialize();

    connectSignalsAndSlots();

    displayLastScreenshot();
}

void MainWindow::connectSignalsAndSlots() {
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startSnap);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopSnap);
    connect(_screenshotSnap.get(), &ScreenshotSnap::newScreenshot, this, &MainWindow::manageNewScreenshot);
}

void MainWindow::startSnap() {
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    _screenshotSnap->startSnap();
}

void MainWindow::stopSnap() {
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    _screenshotSnap->stopSnap();
}

void MainWindow::manageNewScreenshot(const QImage &newScreenshot) {
    QPixmap currentPixmap = ui->currentScreenshotLabel->pixmap();

    ui->currentScreenshotLabel->setPixmap(QPixmap::fromImage(newScreenshot));
    ui->currentScreenshotLabel->setScaledContents(true);

    ui->previousScreenshotLabel->setPixmap(currentPixmap);
    ui->previousScreenshotLabel->setScaledContents(true);

    QThread* thread = new QThread(this);
    SimilarityCalculator *similarityCalculator = new SimilarityCalculator();

    similarityCalculator->moveToThread(thread);
    connect(thread, &QThread::started, similarityCalculator, [this, newScreenshot, currentPixmap, similarityCalculator]() {
        double similarityPercentage = similarityCalculator->getSimilarityPercentage(newScreenshot, currentPixmap.toImage());
        QByteArray hash1 = _imageComparator->calculateHash(newScreenshot);
        QByteArray hash2 = _imageComparator->calculateHash(currentPixmap.toImage());

        ComparisonResult result;
        result.setScreenshot1(newScreenshot);
        result.setScreenshot2(currentPixmap.toImage());
        result.setHash1(hash1);
        result.setHash2(hash2);

        QMetaObject::invokeMethod(this, "handleSimilarityCalculationFinished",
                                  Qt::QueuedConnection,
                                  Q_ARG(ComparisonResult, result),
                                  Q_ARG(double, similarityPercentage));

        QMetaObject::invokeMethod(similarityCalculator, &SimilarityCalculator::deleteLater, Qt::QueuedConnection);
    });

    connect(thread, &QThread::finished, similarityCalculator, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

void MainWindow::handleSimilarityCalculationFinished(const ComparisonResult &result, const double &similarityPercentage) {
    ui->similarityPercentageLabel->setText(QString("Similarity: %1%").arg(similarityPercentage));

    //hide
    _databaseManager->storeComparisonResult(result.getScreenshot1(), result.getScreenshot2(), result.getHash1(), result.getHash2(), similarityPercentage);
}

void setScreenshotLabelImage(QLabel *label, const QImage &image) {
    QPixmap pixmap = QPixmap::fromImage(image);
    label->setPixmap(pixmap);
    label->setScaledContents(true);
}

void MainWindow::displayLastScreenshot() {
    QList<ComparisonResult> comparisonResult = _databaseManager->getComparisonResults();

    if (!comparisonResult.isEmpty()) {
        const QImage &lastImage = comparisonResult.last().getScreenshot1();
        setScreenshotLabelImage(ui->currentScreenshotLabel, lastImage);
    } else {
        QMessageBox::warning(this, "Error", "No screenshots in the database!");
    }
}

MainWindow::~MainWindow() {
    stopSnap();
    delete ui;
}
