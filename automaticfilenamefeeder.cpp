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

    QFileInfoList fileList = exploredDir.entryInfoList(QStringList(FILENAME_TEMPLATE), QDir::Files);
    qDebug() << fileList;

    return QString();
}
