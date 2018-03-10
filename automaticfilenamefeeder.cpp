#include "automaticfilenamefeeder.h"
#include <QDir>
#include <QStringList>
AutomaticFileNameFeeder::AutomaticFileNameFeeder()
{

}

QString AutomaticFileNameFeeder::getFileName(const QString &path)
{
   /* QDir exploredDir(path,FILENAME_TEMPLATE,QDir::Files);

    QStringList fileList = exploredDir.entryInfoList(new QStringList(FILENAME_TEMPLATE),QDir::Files);
    qDebug()  << fileList;*/

}
