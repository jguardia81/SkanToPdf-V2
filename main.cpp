#include "skantopdfdlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SkanToPdfDlg w;
    w.show();

    return a.exec();
}
