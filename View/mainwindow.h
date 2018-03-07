#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_Image_triggered();

    void on_numMedianSegmentation_valueChanged(int arg1);

    void on_numBilateralSegmentation_valueChanged(int arg1);

    void on_btnApplyMedianSegmentation_clicked();

    void on_btnApplyBilateralSegmentation_clicked();

    void on_btnClearImageSegmentation_clicked();

    void on_numSegmentationLineProfileColumnSpacing_valueChanged(int arg1);

    void on_numSegmentationLineProfileDerivativeDistance_valueChanged(int arg1);

    void on_numSegmentationSplinePctSampleSize_valueChanged(double arg1);

    void on_numSegmentationNecksCurvesStdDevThreshold_valueChanged(double arg1);

    void on_numSegmentationCrownBinarizationNumOfSegments_valueChanged(int arg1);

    void on_numSegmentationCrownBinarizationPctThreshold_valueChanged(double arg1);

    void on_btnApplySegmentation_clicked();

    void on_numMedianTracing_valueChanged(int arg1);

    void on_numBilateralTracing_valueChanged(int arg1);

    void on_numSobelTracing_valueChanged(int arg1);

    void on_cmbBilateralTracing_currentIndexChanged(int index);

    void on_btnApplyMedianTracing_clicked();

    void on_btnApplyBilateralTracing_clicked();

    void on_btnApplySobelTracing_clicked();

    void on_btnClearImageTracing_clicked();

    void on_numTracingSlopeAndAngleDistance_valueChanged(int arg1);

    void on_numTracingFirstPixelIntensityThreshold_valueChanged(int arg1);

    void on_numTracingFirstPixelInnerMargin_valueChanged(int arg1);

    void on_numTracingCrownTraceMaxPctHeight_valueChanged(double arg1);

    void on_numTracingCrownTraceExtrapolationDistance_valueChanged(int arg1);

    void on_numTracingCrownTraceExtrapolationMaskSize_valueChanged(int arg1);

    void on_btnApplyTracing_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
