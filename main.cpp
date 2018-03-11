#include "skantopdfdlg.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load("SkanToPdfIV2_fr");
    a.installTranslator(&translator);
    SkanToPdfDlg w;
    w.show();

    return a.exec();
}
