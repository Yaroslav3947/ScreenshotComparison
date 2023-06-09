#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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

void MainWindow::setLabelImage(QLabel *label, const QImage &image) {
    QPixmap pixmap = QPixmap::fromImage(image);
    label->setPixmap(pixmap);
    label->setScaledContents(true);
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

QPixmap MainWindow::getCurrentPixmap() {
    return *ui->currentScreenshotLabel->pixmap();
}

void MainWindow::manageNewScreenshot(const QImage &newScreenshot) {
    QPixmap currentPixmap = getCurrentPixmap();

    setLabelImage(ui->currentScreenshotLabel, newScreenshot);
    setLabelImage(ui->previousScreenshotLabel, currentPixmap.toImage());

    SimilarityCalculationTask *task = new SimilarityCalculationTask(newScreenshot, currentPixmap);

    connect(task, &SimilarityCalculationTask::similarityCalculationFinished, this, &MainWindow::handleSimilarityCalculationFinished, Qt::QueuedConnection);
    task->setAutoDelete(false);
    QThreadPool::globalInstance()->start(task);
}

void MainWindow::handleSimilarityCalculationFinished(const ComparisonResult &result, const double &similarityPercentage) {
    ui->similarityPercentageLabel->setText(QString("Similarity: %1%").arg(similarityPercentage));

    _databaseManager->storeComparisonResult(result, similarityPercentage);

    SimilarityCalculationTask *task = qobject_cast<SimilarityCalculationTask*>(sender());
    if (task) {
        task->deleteLater();
    }
}

void setScreenshotLabelImage(QLabel *label, const QImage &image) {
    QPixmap pixmap = QPixmap::fromImage(image);
    label->setPixmap(pixmap);
    label->setScaledContents(true);
}

void MainWindow::displayLastScreenshot()
{
    QImage lastScreenshot = _databaseManager->getLastScreenshot();

    if (lastScreenshot != QImage()) {
        setScreenshotLabelImage(ui->currentScreenshotLabel, lastScreenshot);
    } else {
        QMessageBox::warning(this, "Error", "No screenshots in the database, creating new one");

        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
            QImage newScreenshot = screen->grabWindow(0).toImage();
            _databaseManager->storeScreenshot(newScreenshot,
                                              _imageComparator->calculateHash(newScreenshot));
            setScreenshotLabelImage(ui->currentScreenshotLabel, newScreenshot);
        }
    }
}

MainWindow::~MainWindow() {
    stopSnap();
    delete ui;
}
