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
    ui->numMedianSegmentation->setValue(Controller::getInstance()->getMedianKernelSizeSegmentation());
    ui->numBilateralSegmentation->setValue(Controller::getInstance()->getBilateralSigmaSegmentation());

    //// SEGMENTATION DEFAULT PARAMETERS ////
    ui->numSegmentationLineProfileColumnSpacing->setValue(
                Controller::getInstance()->getSegmentationLineProfileColumnSpacing());
    ui->numSegmentationLineProfileDerivativeDistance->setValue(
                Controller::getInstance()->getSegmentationLineProfileDerivativeDistance());
    ui->numSegmentationSplinePctSampleSize->setValue(
                Controller::getInstance()->getSegmentationSplinePctSampleSize());
    ui->numSegmentationNecksCurvesStdDevThreshold->setValue(
                Controller::getInstance()->getSegmentationNecksCurvesStdDevThreshold());
    ui->numSegmentationCrownBinarizationNumOfSegments->setValue(
                Controller::getInstance()->getSegmentationCrownBinarizationNumOfSegments());
    ui->numSegmentationCrownBinarizationPctThreshold->setValue(
                Controller::getInstance()->getSegmentationCrownBinarizationPctThreshold());

    /* Code for faster testing.
     * Remove when done testing. */

    QString filename = "/Users/regulrjoe/Documents/CIO/BiometriaDental/imgs/clean/cropped/0000012558_JOSE_ALBETO_SAUCEDO_Panorama_20161205112500.jpg";
    Controller::getInstance()->setInputImage(filename.toUtf8().data());
    ui->imgViewer->showImage(Controller::getInstance()->getFilteredImageSegmentation());
    ui->btnApplyMedianSegmentation->setEnabled(true);
    ui->btnApplyBilateralSegmentation->setEnabled(true);
    ui->btnClearImageSegmentation->setEnabled(true);
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
            ui->imgViewer->showImage(Controller::getInstance()->getFilteredImageSegmentation());
            ui->btnApplyMedianSegmentation->setEnabled(true);
            ui->btnApplyBilateralSegmentation->setEnabled(true);
            ui->btnClearImageSegmentation->setEnabled(true);
            ui->btnApplySegmentation->setEnabled(true);
        } else {
            std::cout << "Image loading failed." << std::endl;
            QMessageBox::information(this, tr("Unable to open image"), tr("Select a valid image file."));
        }
    }
}


void MainWindow::on_numMedianSegmentation_valueChanged(int arg1)
{
    if (!Controller::getInstance()->setMedianKernelSizeSegmentation(arg1)) {
        QMessageBox::information(this, tr("Invalid Median Filter Kernel Size"), tr("Kernel size must be greather or equal to 3, lower or equal to 15, and odd."));
        ui->numMedianSegmentation->setValue(Controller::getInstance()->getMedianKernelSizeSegmentation());
    }
}

void MainWindow::on_numBilateralSegmentation_valueChanged(int arg1)
{
    if (!Controller::getInstance()->setBilateralSigmaSegmentation(arg1)) {
        QMessageBox::information(this, tr("Invalid Bilateral Filter Sigma"), tr("Sigma must be greather than 0 and lower or equal to 30."));
        ui->numBilateralSegmentation->setValue(Controller::getInstance()->getBilateralSigmaSegmentation());
    }
}

void MainWindow::on_btnApplyMedianSegmentation_clicked()
{
    Controller::getInstance()->applyMedianSegmentation();
    ui->imgViewer->showImage(Controller::getInstance()->getFilteredImageSegmentation());
}

void MainWindow::on_btnApplyBilateralSegmentation_clicked()
{
    Controller::getInstance()->applyBilateralSegmentation();
    ui->imgViewer->showImage(Controller::getInstance()->getFilteredImageSegmentation());
}

void MainWindow::on_btnClearImageSegmentation_clicked()
{
    Controller::getInstance()->resetImageSegmentation();
    ui->imgViewer->showImage(Controller::getInstance()->getFilteredImageSegmentation());
}

void MainWindow::on_numSegmentationLineProfileColumnSpacing_valueChanged(int arg1)
{
    if (!Controller::getInstance()->setSegmentationLineProfileColumnSpacing(arg1)) {
        QMessageBox::warning(this,
                             tr("Invalid Column Spacing"),
                             tr("Column spacing must be greater than 0 and equal to or lower than 100."));
        ui->numSegmentationLineProfileColumnSpacing->setValue(
                    Controller::getInstance()->getSegmentationLineProfileColumnSpacing());
    }
}

void MainWindow::on_numSegmentationLineProfileDerivativeDistance_valueChanged(int arg1)
{
    if (!Controller::getInstance()->setSegmentationLineProfileDerivativeDistance(arg1)) {
        QMessageBox::warning(this,
                             tr("Invalid Derivative Distance"),
                             tr("Derivative distance must be greater than 0 and equal to or lower than 100."));
        ui->numSegmentationLineProfileDerivativeDistance->setValue(
                    Controller::getInstance()->getSegmentationLineProfileDerivativeDistance());
    }
}

void MainWindow::on_numSegmentationSplinePctSampleSize_valueChanged(double arg1)
{
    if (!Controller::getInstance()->setSegmentationSplinePctSampleSize((float)arg1)) {
        QMessageBox::warning(this,
                             tr("Invalid Spline Curve Percentage Sample Size"),
                             tr("The percentage sample size must be greater than 0.00 and equal to or lower than 1.00"));
        ui->numSegmentationSplinePctSampleSize->setValue(
                    Controller::getInstance()->getSegmentationSplinePctSampleSize());
    }
}

void MainWindow::on_numSegmentationNecksCurvesStdDevThreshold_valueChanged(double arg1)
{
    if (!Controller::getInstance()->setSegmentationNecksCurvesStdDevThreshold((float)arg1)) {
        QMessageBox::warning(this,
                             tr("Invalid Standard Deviation Threshold"),
                             tr("The standard deviation threhsold must be greather than 0.00 and lower than 1.00"));
        ui->numSegmentationNecksCurvesStdDevThreshold->setValue(
                    Controller::getInstance()->getSegmentationNecksCurvesStdDevThreshold());
    }
}

void MainWindow::on_numSegmentationCrownBinarizationNumOfSegments_valueChanged(int arg1)
{
    if (!Controller::getInstance()->setSegmentationCrownBinarizationNumOfSegments(arg1)) {
        QMessageBox::warning(this,
                             tr("Invalid Number of Segments"),
                             tr("The number of segments must be greater than 0 and lower than 100."));
        ui->numSegmentationCrownBinarizationNumOfSegments->setValue(
                    Controller::getInstance()->getSegmentationCrownBinarizationNumOfSegments());
    }
}

void MainWindow::on_numSegmentationCrownBinarizationPctThreshold_valueChanged(double arg1)
{
    if (!Controller::getInstance()->setSegmentationCrownBinarizationPctThreshold((float)arg1)) {
        QMessageBox::warning(this,
                             tr("Invalid Percentage Threhsold"),
                             tr("The percentage threshold must be greater than 0 and lower than 1."));
        ui->numSegmentationCrownBinarizationPctThreshold->setValue(
                    Controller::getInstance()->getSegmentationCrownBinarizationPctThreshold());
    }
}

void MainWindow::on_btnApplySegmentation_clicked()
{
    Controller::getInstance()->runSegmentation();
    ui->imgViewer->showImage(Controller::getInstance()->getFilteredImageSegmentation());
}

void MainWindow::on_numMedianTracing_valueChanged(int arg1)
{

}

void MainWindow::on_numBilateralTracing_valueChanged(int arg1)
{

}

void MainWindow::on_numSobelTracing_valueChanged(int arg1)
{

}

void MainWindow::on_cmbBilateralTracing_currentIndexChanged(int index)
{

}

void MainWindow::on_btnApplyMedianTracing_clicked()
{

}

void MainWindow::on_btnApplyBilateralTracing_clicked()
{

}

void MainWindow::on_btnApplySobelTracing_clicked()
{

}

void MainWindow::on_btnClearImageTracing_clicked()
{

}
