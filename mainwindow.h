#pragma once

#include <QThread>
#include <QMainWindow>
#include <QThreadPool>

#include "screenshotsnap.h"
#include "imagecomparator.h"
#include "databasemanager.h"
#include "SimilarityCalculationTask.h"


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
    void handleSimilarityCalculationFinished(const ComparisonResult &result, const double &similarityPercentage);

private:
    QPixmap getCurrentPixmap();
    void displayLastScreenshot();
    void connectSignalsAndSlots();
    void setLabelImage(QLabel *label, const QImage &image);


    std::unique_ptr<ImageComparator> _imageComparator;
    std::unique_ptr<ScreenshotSnap> _screenshotSnap;
    std::unique_ptr<DatabaseManager> _databaseManager;

    Ui::MainWindow *ui;
};
