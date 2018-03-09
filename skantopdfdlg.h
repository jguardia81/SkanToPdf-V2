#ifndef SKANTOPDFDLG_H
#define SKANTOPDFDLG_H

#include <QDialog>
#include <QByteArray>
#include <QList>
#include <QImage>
#include <QLineEdit>
#include <QPrinter>
#include <QString>

namespace Ui {
class SkanToPdfDlg;
}
/**
 * @brief The SkanToPdfDlg class
 * Main window of the app
 */
class SkanToPdfDlg : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief SkanToPdfDlg
     * Contructor of the class
     * @param parent given parent widget
     */
    explicit SkanToPdfDlg(QWidget *parent = 0);
    /**
     * @brief Destuctor
     */
    ~SkanToPdfDlg();

private slots:
    /**
     * @brief on_closeButton_clicked
     * On clicked used slot
     */
    void on_closeButton_clicked();

    /**
     * @brief onImageReady
     * Slot for receiving scanned images
     * @param data The scanned image as a byte array
     * @param width its width
     * @param height its keight
     * @param bytes_per_line nb of bits per line
     * @param format image format
     */
    void onImageReady(QByteArray &data, int width, int height, int bytes_per_line, int format);
    void on_folderFindBtn_clicked();
    void on_saveBtn_clicked();
    void on_btnPreview_clicked();

    void on_sendMailBtn_clicked();

private:
    Ui::SkanToPdfDlg *ui; /// the ui pointer
    bool _dirty = false; /// indicator of dirty state
    QList <QImage> _images; /// Scanned images list
    /**
     * @brief initialize
     * initialize the window with default values
     */
    void initialize();
    void initDevice(QMap<QString,QString> options, QString selectedDevice);

    QString retrieveText(const QLineEdit * const lineEdit);
    QString createFileUrl(const QString &file, const QString &folder);
    void setupPrinter(QPrinter &printer, const QString &pathToPdfFile);
    void printPages(QPrinter &printer, const QString &file, const QString &folder);
    QString _attachement = "";
    QString _pdfFile = "";
};

#endif // SKANTOPDFDLG_H
