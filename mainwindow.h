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

    void on_sliderMedian_valueChanged(int value);

    void on_sliderBilateral_valueChanged(int value);

    void on_numMedian_valueChanged(int arg1);

    void on_numBilateral_valueChanged(int arg1);

    void on_btnApplyMedian_clicked();

    void on_btnApplyBilateral_clicked();

    void on_btnClearFilters_clicked();

    void on_numSegmentationLineProfileColumnSpacing_valueChanged(int arg1);

    void on_numSegmentationLineProfileDerivativeDistance_valueChanged(int arg1);

    void on_btnApplySegmentation_clicked();

    void on_numSegmentationSplineRelativeSampleSize_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
