#include "appcontext.h"
#include <QCoreApplication>
#include <QDebug>
#include <QStandardPaths>

AppContext* AppContext::_uniqueInstance = nullptr;

AppContext::AppContext(QObject* parent)
    : QObject(parent)
{
    QCoreApplication::setOrganizationName("JGuardia");
    QCoreApplication::setOrganizationDomain("Hobby development");
    QCoreApplication::setApplicationName("SkanToPdf V2.0");

    _appSettings = new QSettings(this);
    QStringList documentFolders = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    qDebug() << documentFolders;

    _currentFolder = assignStoredValue(QString("pdfstuff/lastfolder"), documentFolders.at(0));

    _defaultFilename = _appSettings->value("pdfstuff/defaultFileName").toString();
    if (_defaultFilename.isEmpty()) {
        _defaultFilename = "producedPdf";
        _appSettings->setValue("pdfstuff/defaultFileName", _defaultFilename);
    }
}

QString AppContext::assignStoredValue(QString key, QString defaultValue)
{
    QString storedValue = _appSettings->value(key).toString();
    if (storedValue.isEmpty()) {
        storedValue = defaultValue;
        _appSettings->setValue(key, storedValue);
    }
    return storedValue;
}

QString AppContext::getDefaultExtention() const
{
    return _defaultExtention;
}

void AppContext::setDefaultExtention(const QString& defaultExtention)
{
    _defaultExtention = defaultExtention;
    if (_appSettings != nullptr) {
        _appSettings->setValue("pdfstuff/defaultExtName", defaultExtention);
    }
}

QString AppContext::getDefaultFilename() const
{
    return _defaultFilename;
}

void AppContext::setDefaultFilename(const QString& defaultFilename)
{
    _defaultFilename = defaultFilename;
    if (_appSettings != nullptr) {
        _appSettings->setValue("pdfstuff/defaultfilename", defaultFilename);
    }
}

QString AppContext::getPdfFileName() const
{
    return _pdfFileName;
}

void AppContext::setPdfFileName(const QString& value) { _pdfFileName = value; }

QString AppContext::getPdfFileUrl()
{
    QString url = _currentFolder;
    if (!_currentFolder.endsWith("/")) {
        url += "/";
    }
    url += _pdfFileName;
    return url;
}

QString AppContext::getCurrentFolder() const
{
    return _currentFolder;
}

void AppContext::setCurrentFolder(const QString& currentFolder)
{
    _currentFolder = currentFolder;
    if (_appSettings != nullptr) {
        _appSettings->setValue("pdfstuff/lastFolder", currentFolder);
    }
}

QSettings* AppContext::getAppSettings() const
{
    return _appSettings;
}

AppContext* AppContext::getInstance()
{
    if (_uniqueInstance == nullptr) {
        _uniqueInstance = new AppContext();
    }
    return _uniqueInstance;
}
