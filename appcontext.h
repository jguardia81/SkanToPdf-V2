#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <QSettings>

class AppContext : public QObject
{
    Q_OBJECT
private:
    explicit AppContext(QObject *parent = nullptr);
    static AppContext* _uniqueInstance;
    QSettings* _appSettings = nullptr;
public:
    static AppContext* getInstance();
signals:

public slots:
};

#endif // APPCONTEXT_H
