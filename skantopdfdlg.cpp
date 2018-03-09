#include "skantopdfdlg.h"
#include "ui_skantopdfdlg.h"
#include <QString>
#include <QMap>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>
#include <QUrl>
#include <QDesktopServices>
#include <QDebug>
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

/**
 * @brief SkanToPdfDlg::on_folderFindBtn_clicked
 * slot to select folder
 */
void SkanToPdfDlg::on_folderFindBtn_clicked()
{
    QFileDialog dirDialog(this);

    dirDialog.setFileMode(QFileDialog::Directory);
    int result = dirDialog.exec();

    if (result == QDialog::Accepted) {
        ui->folderLineEdit->setText(dirDialog.selectedFiles().at(0));
    }
}

/**
 * @brief SkanToPdfDlg::retrieveText Retrieve trimmed
 * teext from line edit box
 * @param lineEdit the given line edit
 * @return the trimmed text of the lienedit
 */
QString SkanToPdfDlg::retrieveText(const QLineEdit* const lineEdit) {
    QString retrievedText = "";
    if (lineEdit != nullptr) {
        retrievedText = lineEdit->text().trimmed();
    }
    return retrievedText;
}

/**
 * @brief SkanToPdfDlg::on_saveBtn_clicked
 * Save action slot
 */
void SkanToPdfDlg::on_saveBtn_clicked()
{
    QString folderName = "";
    QString pdfName = "";

    // Retrieve data from boxes
    folderName = retrieveText( ui->folderLineEdit);
    pdfName = retrieveText(ui->pdfNameLineEdit);

    // Check values and out if invalid
    if (folderName.isEmpty()) {
        QMessageBox::critical(this, tr("Folder name"), tr("A folder name must be supplied.") );
        ui->folderLineEdit->setFocus();
        return;
    }

    if (pdfName.isEmpty()) {
        QMessageBox::critical(this,tr("Pdf name"), tr("A pdf filename must be supplied."));
        ui->pdfNameLineEdit->setFocus();
        return;
    }
    _pdfFile = pdfName;
    QString pathToPdfFile = createFileUrl(pdfName, folderName);

    // Check generated file nalme existence
    if (QFile::exists(pathToPdfFile)) {
        int result = QMessageBox::warning(this,tr("File exists"),
                                          QString(tr("Warning %1 already exists inf folder %2. Overwrite it ?"))
                                          .arg(pdfName).arg(folderName),QMessageBox::Yes, QMessageBox::No);
        if (result == QMessageBox::No) {
            QMessageBox::warning(this,"Warning", tr("Change file name or folder."));
            ui->pdfNameLineEdit->setFocus();
            return;
        }
    }

    QPrinter printer;
    // Prepare pdf printer
    setupPrinter(printer,pathToPdfFile );

    // Print pages
    printPages(printer, pdfName, folderName);
    _attachement = pathToPdfFile;
}
/**
 * @brief SkanToPdfDlg::createFileUrl
 * Create file url using its name and its folder
 * @param file the file name (.pdf will be added if missing)
 * @param folder the given folder name
 * @return  the correct url of the file
 */
QString SkanToPdfDlg::createFileUrl(const QString& file,  const QString& folder)
{
    QString pathToPdfFile = folder;
    if (!folder.endsWith("/")) {
        pathToPdfFile = pathToPdfFile.append("/");
    }
    pathToPdfFile.append(file);

    if (!pathToPdfFile.endsWith(".pdf")) {
        pathToPdfFile = pathToPdfFile.append(".pdf");
    }

    return pathToPdfFile;
}


void SkanToPdfDlg::on_btnPreview_clicked()
{

}

/**
 * @brief SkanToPdfDlg::setupPrinter
 * Setup the pdf printer
 * @param printer the printer
 * @param pathToPdfFile the file wher to store
 */
void SkanToPdfDlg::setupPrinter(QPrinter& printer, const QString& pathToPdfFile )
{
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(pathToPdfFile);

    QPrinter::Margins margins;
    margins.bottom = 0.0;
    margins.top =0.0;
    margins.left = 0.0;
    margins.right =0.0;
    printer.setMargins(margins);
    printer.setFullPage(true);
    QPageLayout layout = printer.pageLayout();
    layout.setMode(QPageLayout::FullPageMode);
    printer.setPageLayout(layout);
}

/**
 * @brief SkanToPdfDlg::printPages
 * Print the page on the printer
 * @param printer the given printer
 * @param file the given filename
 * @param folder the given folder
 */
void SkanToPdfDlg::printPages(QPrinter& printer, const QString &file, const QString &folder)
{
    QPainter painter;
    QRectF pageRect;
    painter.begin(&printer);

    printer.setPageSize(QPagedPaintDevice::A4);
    for (int index = 0; index < _images.count(); ++index) {
        if (index != 0) {
            printer.newPage();
        }
        painter.drawImage(printer.pageRect(),_images.at(index));
    }
    painter.end();
    QMessageBox::information(this,QString(tr("Saving file")),QString("File %1 has been saved in folder %2").arg(file).arg(folder));
    _dirty = false;
}

void SkanToPdfDlg::on_sendMailBtn_clicked()
{
    QString filename = _attachement;

    if (filename.isEmpty() || _pdfFile.isEmpty()) {
        return;
    }

    if (!QFile::exists(filename)) {
        QString errorMessage = tr("%1 does not exist.");
        QMessageBox::critical(this,tr("Error"), errorMessage);
        return;
    }

    QString subject = tr("Sending %1").arg(_pdfFile);
    QString urlToOpen = QString("mailto:?subject=%1&attach=%2").arg(subject).arg(filename);
    qDebug() << urlToOpen;
    QDesktopServices::openUrl(QUrl(urlToOpen));
}
