#pragma once

#include <QThread>
#include <QMainWindow>


#include "screenshotsnap.h"
#include "imagecomparator.h"
#include "databasemanager.h"
#include "similaritycalculator.h"


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
//    void updateSimilarityPercentage(double similarityPercentage);
    void handleSimilarityCalculationFinished(const ComparisonResult &result, const double &similarityPercentage);

private:
    void displayLastScreenshot();
    void connectSignalsAndSlots();

    std::unique_ptr<ImageComparator> _imageComparator;
    std::unique_ptr<ScreenshotSnap> _screenshotSnap;
    std::unique_ptr<DatabaseManager> _databaseManager;

    Ui::MainWindow *ui;
};
