#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sliderMedian_sliderMoved(int position)
{

}

void MainWindow::on_sliderBilateral_sliderMoved(int position)
{

}

void MainWindow::on_numMedian_valueChanged(int arg1)
{

}

void MainWindow::on_numBilateral_valueChanged(int arg1)
{

}

void MainWindow::on_btnApplyMedian_clicked()
{

}

void MainWindow::on_btnApplyBilateral_clicked()
{

}

void MainWindow::on_btnClearFilters_clicked()
{

}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{

}

void MainWindow::on_spinBox_4_valueChanged(int arg1)
{

}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_actionOpen_Image_triggered()
{

}
