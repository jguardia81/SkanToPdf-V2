#include "pdfpreviewdlg.h"
#include "ui_pdfpreviewdlg.h"

PdfPreviewDlg::PdfPreviewDlg(QList<QImage> images, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::PdfPreviewDlg)
    , _previewImages(images)
{
    ui->setupUi(this);
    if (_previewImages.size() > 0) {
        _currentIndex = 0;
        _imageCount = _previewImages.size();
        displayImage();
    } else {
        _currentIndex = -1;
        _imageCount = -1;
    }
}

PdfPreviewDlg::~PdfPreviewDlg()
{
    delete ui;
}

QList<QImage> PdfPreviewDlg::previewImages() const
{
    return _previewImages;
}

/**
 * @brief PdfPreviewDlg::updateGui
 */
void PdfPreviewDlg::updateGui()
{
    if (_currentIndex == -1) {
        ui->btnFirst->setEnabled(false);
        ui->btnLast->setEnabled(false);
        ui->btnNext->setEnabled(false);
        ui->btnPrevious->setEnabled(false);
    } else if (_currentIndex == 0 && _imageCount - 1 > 0) {
        ui->btnFirst->setEnabled(false);
        ui->btnLast->setEnabled(true);
        ui->btnNext->setEnabled(true);
        ui->btnPrevious->setEnabled(false);
    } else if (_currentIndex == _imageCount - 1 && _imageCount > 1) {
        ui->btnFirst->setEnabled(true);
        ui->btnLast->setEnabled(false);
        ui->btnNext->setEnabled(false);
        ui->btnPrevious->setEnabled(true);
    } else if (_currentIndex == 0 && _imageCount == 1) {
        ui->btnFirst->setEnabled(false);
        ui->btnLast->setEnabled(false);
        ui->btnNext->setEnabled(false);
        ui->btnPrevious->setEnabled(false);
    } else {
        ui->btnFirst->setEnabled(true);
        ui->btnLast->setEnabled(true);
        ui->btnNext->setEnabled(true);
        ui->btnPrevious->setEnabled(true);
    }
}

void PdfPreviewDlg::displayImage()
{
    QPixmap imageToDisplay;
    if (_currentIndex >= 0 && _currentIndex < _imageCount) {
        imageToDisplay = QPixmap::fromImage(_previewImages.at(_currentIndex));
    }
    QPixmap finalImage = imageToDisplay.scaledToHeight(700);
    ui->lblShowImage->setPixmap(finalImage);
    updateGui();
}

bool PdfPreviewDlg::modified() const
{
    return _modified;
}
void PdfPreviewDlg::on_btnFirst_clicked()
{
    if (_currentIndex > 0) {
        _currentIndex = 0;
        displayImage();
    }
}

void PdfPreviewDlg::on_btnPrevious_clicked()
{
    if (_currentIndex > 0) {
        _currentIndex--;
        displayImage();
    }
}

void PdfPreviewDlg::on_btnNext_clicked()
{
    if (_currentIndex < _imageCount - 1) {
        _currentIndex++;
        displayImage();
    }
}

void PdfPreviewDlg::on_btnLast_clicked()
{
    if (_currentIndex < _imageCount - 1) {
        _currentIndex = _imageCount - 1;
        displayImage();
    }
}

void PdfPreviewDlg::on_btnDeletePage_clicked()
{
    if (_currentIndex >= 0 && _currentIndex < _imageCount) {
        _previewImages.removeAt(_currentIndex);
        _modified = true;
        _imageCount--;
        if (_imageCount <= 0) {
            _currentIndex = -1;
        } else if (_currentIndex >= _imageCount) {
            _currentIndex = _imageCount - 1;
        }
        displayImage();
    }
}
