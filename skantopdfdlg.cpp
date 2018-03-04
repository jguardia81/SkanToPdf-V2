#include "skantopdfdlg.h"
#include "ui_skantopdfdlg.h"
#include <QString>
#include <QMap>
#include <QMessageBox>
#include <QFileDialog>
/**
 * @brief SkanToPdfDlg::SkanToPdfDlg
 * @param parent: given parent widget
 */
SkanToPdfDlg::SkanToPdfDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SkanToPdfDlg)
{
    ui->setupUi(this);
    initialize();
}
/**
 * @brief SkanToPdfDlg::~SkanToPdfDlg
 */
SkanToPdfDlg::~SkanToPdfDlg()
{
    delete ui;
}

/**
 * @brief SkanToPdfDlg::on_closeButton_clicked
 */
void SkanToPdfDlg::on_closeButton_clicked()
{
    bool canClose = false;
    if (_dirty) {
        int response = QMessageBox::warning(this,tr("Closing skanToPdf"), tr("Data are not saved. Closing anyway ?"),QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (response == QMessageBox::Yes ) {
            canClose = true;
        }
    } else {
        canClose = true;
    }
    if (canClose)
        QApplication::exit(0);
}

/**
 * @brief SkanToPdfDlg::onImageReady
 * @param data
 * @param width
 * @param height
 * @param bytes_per_line
 * @param format
 */
void SkanToPdfDlg::onImageReady(QByteArray &data, int width, int height, int bytes_per_line, int format)
{
    QImage image = ui->skanWidget->toQImage(data,width,height, bytes_per_line, (KSaneIface::KSaneWidget::ImageFormat) format);
    _images.append(image);
    _dirty = true;
}

/**
 * @brief SkanToPdfDlg::initDevice
 * Initiaiize the skanner
 * @param options: options of the scanner
 * @param selectedDevice selected device string
 */
void SkanToPdfDlg::initDevice(QMap<QString,QString> options, QString selectedDevice)
{
    ui->skanWidget->openDevice(selectedDevice);
    ui->skanWidget->setOptionsCollapsed(true);
    ui->skanWidget->getOptVals(options);
    ui->skanWidget->setOptVal("mode","Color");
    ui->skanWidget->setOptVal("resolution","100 DPI");
    ui->skanWidget->enableAutoSelect(false);
    ui->skanWidget->setPreviewButtonText(tr("Preview"));
    ui->skanWidget->setScanButtonText(tr("Add to document"));
    //connect the slot
    connect (ui->skanWidget, &KSaneIface::KSaneWidget::imageReady, this,  &SkanToPdfDlg::onImageReady );
}

/**
 * @brief SkanToPdfDlg::initialize
 */
void SkanToPdfDlg::initialize() {
    QString selectedDevice = "";
    QMap<QString,QString> options;
    ui->tabWidget->setCurrentIndex(0);
    bool getOut = false;
    bool deviceInitialized = false;
    while (!getOut) {
         selectedDevice = ui->skanWidget->selectDevice(this).trimmed();
        // Initialize the scanner
        if (!selectedDevice.isEmpty()) {
            initDevice(options, selectedDevice);
            getOut = true;
            deviceInitialized = true;
        } else {
            int response = QMessageBox::critical(this, tr("Scanner problem"),
                                                  tr("No scanner has been selected or found.")
                                                  , QMessageBox::Retry, QMessageBox::Abort);
            if (response == QMessageBox::Abort) {
                getOut = true;
                deviceInitialized = false;
            }
        }

    }


    if (!deviceInitialized) {
        this->close();
        QApplication::exit(-1);
    }
}

void SkanToPdfDlg::on_folderFindBtn_clicked()
{
    QFileDialog dirDialog(this);

    dirDialog.setFileMode(QFileDialog::Directory);
    int result = dirDialog.exec();

    if (result == QDialog::Accepted) {
        ui->folderLineEdit->setText(dirDialog.selectedFiles().at(0));
    }
}

void SkanToPdfDlg::on_saveBtn_clicked()
{
    QString folderName = "";
    QString pdfName = "";

}
