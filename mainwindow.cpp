#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //// PREPROCESSING DEFAULT PARAMETERS ////
    ui->numMedian->setValue(Controller::getInstance()->getMedianKernelSize());
    ui->numBilateral->setValue(Controller::getInstance()->getBilateralSigma());

    //// SEGMENTATION DEFAULT PARAMETERS ////
    ui->numSegmentationLineProfileColumnSpacing->setValue(Controller::getInstance()->getSegmentationLineProfileColumnSpacing());
    ui->numSegmentationLineProfileDerivativeDistance->setValue(Controller::getInstance()->getSegmentationLineProfileDerivativeDistance());
    ui->numSegmentationSplineRelativeSampleSize->setValue(Controller::getInstance()->getSegmentationSplineRelativeSampleSize());

    /* Code for faster testing.
     * Remove when done testing. */

    QString filename = "/Users/regulrjoe/Documents/CIO/BiometriaDental/imgs/clean/cropped/0000012558_JOSE_ALBETO_SAUCEDO_Panorama_20161205112500.jpg";
    Controller::getInstance()->setInputImage(filename.toUtf8().data());
    ui->imgViewer->showImage(Controller::getInstance()->getFilteredImage());
    ui->btnApplyMedian->setEnabled(true);
    ui->btnApplyBilateral->setEnabled(true);
    ui->btnClearFilters->setEnabled(true);
    ui->btnApplySegmentation->setEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this,
                                            tr("Open an image file"),
                                            "~/",
                                            tr("Image Files (*.png *.jpg *.jpeg *.bmp"));

    if (filename != NULL) {
        if (Controller::getInstance()->setInputImage(filename.toUtf8().data())) {
            ui->imgViewer->showImage(Controller::getInstance()->getFilteredImage());
            ui->btnApplyMedian->setEnabled(true);
            ui->btnApplyBilateral->setEnabled(true);
            ui->btnClearFilters->setEnabled(true);
            ui->btnApplySegmentation->setEnabled(true);
        } else {
            std::cout << "Image loading failed." << std::endl;
            QMessageBox::information(this, tr("Unable to open image"), tr("Select a valid image file."));
        }
    }
}

void MainWindow::on_sliderMedian_valueChanged(int value)
{
    if (!Controller::getInstance()->setMedianKernelSize((value*2)+1)) {
        QMessageBox::information(this, tr("Invalid Median Filter Kernel Size"), tr("Kernel size must be greather or equal to 3, lower or equal to 15, and odd."));
        ui->sliderMedian->setValue((Controller::getInstance()->getMedianKernelSize()+1)/2);
    }
    ui->numMedian->setValue(Controller::getInstance()->getMedianKernelSize());
}

void MainWindow::on_sliderBilateral_valueChanged(int value)
{
    if (!Controller::getInstance()->setBilateralSigma(value)) {
        QMessageBox::information(this, tr("Invalid Bilateral Filter Sigma"), tr("Sigma must be greather than 0 and lower or equal to 30."));
        ui->sliderBilateral->setValue(Controller::getInstance()->getBilateralSigma());
    }
    ui->numBilateral->setValue(Controller::getInstance()->getBilateralSigma());
}

void MainWindow::on_numMedian_valueChanged(int arg1)
{
    if (!Controller::getInstance()->setMedianKernelSize(arg1)) {
        QMessageBox::information(this, tr("Invalid Median Filter Kernel Size"), tr("Kernel size must be greather or equal to 3, lower or equal to 15, and odd."));
        ui->numMedian->setValue(Controller::getInstance()->getMedianKernelSize());
    }
    ui->sliderMedian->setValue((Controller::getInstance()->getMedianKernelSize()-1)/2);
}

void MainWindow::on_numBilateral_valueChanged(int arg1)
{
    if (!Controller::getInstance()->setBilateralSigma(arg1)) {
        QMessageBox::information(this, tr("Invalid Bilateral Filter Sigma"), tr("Sigma must be greather than 0 and lower or equal to 30."));
        ui->numBilateral->setValue(Controller::getInstance()->getBilateralSigma());
    }
    ui->sliderBilateral->setValue(Controller::getInstance()->getBilateralSigma());
}

void MainWindow::on_btnApplyMedian_clicked()
{
    Controller::getInstance()->applyMedian();
    ui->imgViewer->showImage(Controller::getInstance()->getFilteredImage());
}

void MainWindow::on_btnApplyBilateral_clicked()
{
    Controller::getInstance()->applyBilateral();
    ui->imgViewer->showImage(Controller::getInstance()->getFilteredImage());
}

void MainWindow::on_btnClearFilters_clicked()
{
    Controller::getInstance()->resetImage();
    ui->imgViewer->showImage(Controller::getInstance()->getFilteredImage());
}

void MainWindow::on_numSegmentationLineProfileColumnSpacing_valueChanged(int arg1)
{
    if (!Controller::getInstance()->setSegmentationLineProfileColumnSpacing(arg1)) {
        QMessageBox::warning(this, tr("Invalid Column Spacing"), tr("Column spacing must be greater than 0 and equal to or lower than 100."));
        ui->numSegmentationLineProfileColumnSpacing->setValue(Controller::getInstance()->getSegmentationLineProfileColumnSpacing());
    }
}

void MainWindow::on_numSegmentationLineProfileDerivativeDistance_valueChanged(int arg1)
{
    if (!Controller::getInstance()->setSegmentationLineProfileDerivativeDistance(arg1)) {
        QMessageBox::warning(this, tr("Invalid Derivative Distance"), tr("Derivative distance must be greater than 0 and equal to or lower than 100."));
        ui->numSegmentationLineProfileDerivativeDistance->setValue(Controller::getInstance()->getSegmentationLineProfileDerivativeDistance());
    }
}

void MainWindow::on_numSegmentationSplineRelativeSampleSize_valueChanged(double arg1)
{
    if (!Controller::getInstance()->setSegmentationSplineRelativeSampleSize((float)arg1)) {
        QMessageBox::warning(this, tr("Invalid Spline Curve Relative Sample Size"), tr("The relative sample size must be greater than 0.00 and equal to or lower than 1.00"));
        ui->numSegmentationSplineRelativeSampleSize->setValue(Controller::getInstance()->getSegmentationSplineRelativeSampleSize());
    }
}

void MainWindow::on_btnApplySegmentation_clicked()
{
    Controller::getInstance()->runSegmentation();
}
