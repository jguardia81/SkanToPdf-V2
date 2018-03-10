#include "appcontext.h"
#include <QCoreApplication>

AppContext* AppContext::_uniqueInstance = nullptr;


AppContext::AppContext(QObject *parent) : QObject(parent)
{
    QCoreApplication::setOrganizationName("JGuardia");
    QCoreApplication::setOrganizationDomain("Hobby develepment");
    QCoreApplication::setApplicationName("SkanToPdf V2.0");

    _appSettings = new QSettings(this);
}

AppContext *AppContext::getInstance()
{
    if (_uniqueInstance == nullptr) {
        _uniqueInstance = new AppContext();
    }
    return _uniqueInstance;
}


