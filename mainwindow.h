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
    void on_sliderMedian_sliderMoved(int position);

    void on_sliderBilateral_sliderMoved(int position);

    void on_numMedian_valueChanged(int arg1);

    void on_numBilateral_valueChanged(int arg1);

    void on_btnApplyMedian_clicked();

    void on_btnApplyBilateral_clicked();

    void on_btnClearFilters_clicked();

    void on_spinBox_3_valueChanged(int arg1);

    void on_spinBox_4_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_actionOpen_Image_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
