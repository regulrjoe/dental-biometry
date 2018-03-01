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

void MainWindow::on_btnApplySegmentation_clicked()
{

}

void MainWindow::on_numSegmentationLineProfileColumnSpacing_valueChanged(int arg1)
{

}

void MainWindow::on_numSegmentationLineProfileDerivativeDistance_valueChanged(int arg1)
{

}
