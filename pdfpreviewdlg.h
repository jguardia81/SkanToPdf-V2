#ifndef PDFPREVIEWDLG_H
#define PDFPREVIEWDLG_H

#include <QDialog>
#include <QImage>
#include <QList>

namespace Ui {
class PdfPreviewDlg;
}

class PdfPreviewDlg : public QDialog {
    Q_OBJECT

public:
    explicit PdfPreviewDlg(QList<QImage> images, QWidget* parent = 0);
    ~PdfPreviewDlg();

    QList<QImage> previewImages() const;

    bool modified() const;

private slots:
    void on_btnFirst_clicked();

    void on_btnPrevious_clicked();

    void on_btnNext_clicked();

    void on_btnLast_clicked();

    void on_btnDeletePage_clicked();

private:
    Ui::PdfPreviewDlg* ui;
    QList<QImage> _previewImages;
    int _currentIndex = -1;
    int _imageCount = 0;
    void updateGui();
    void displayImage();
    bool _modified = false;
};

#endif // PDFPREVIEWDLG_H
