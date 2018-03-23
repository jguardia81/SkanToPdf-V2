#include "automaticfilenamefeeder.h"
#include <QDebug>
#include <QDir>
#include <QFileInfoList>

AutomaticFileNameFeeder::AutomaticFileNameFeeder()
{
}

QString AutomaticFileNameFeeder::getFileName(const QString& path)
{
    QDir exploredDir(path, FILENAME_TEMPLATE, QDir::Name | QDir::IgnoreCase, QDir::Files);
    QString searchedFileName = AppContext::getInstance()->getDefaultFilename() + "*." + AppContext::getInstance()->getDefaultExtention();
    QFileInfoList fileList = exploredDir.entryInfoList(QStringList(searchedFileName), QDir::Files);
    qDebug() << fileList;
    QString purposedFileName = "";
    if (fileList.size() == 0) {
        purposedFileName = AppContext::getInstance()->getDefaultFilename() + "." + AppContext::getInstance()->getDefaultExtention();
    } else {
        purposedFileName = AppContext::getInstance()->getDefaultFilename() + "_%1." + AppContext::getInstance()->getDefaultExtention();
        purposedFileName = purposedFileName.arg(fileList.size());
    }
    return purposedFileName;
}
