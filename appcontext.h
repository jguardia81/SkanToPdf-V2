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
    QString _currentFolder = "";
    QString _pdfFileName="";
    QString _defaultFilename="";
    QString _defaultExtention="";
    QString assignStoredValue(QString key, QString defaultValue);

public:
    static AppContext* getInstance();
    QSettings *getAppSettings() const;

    QString getCurrentFolder() const;
    void setCurrentFolder(const QString &currentFolder);

    QString getPdfFileName() const;
    void setPdfFileName(const QString &value);
    QString getPdfFileUrl();

    QString getDefaultFilename() const;
    void setDefaultFilename(const QString &defaultFilename);

    QString getDefaultExtention() const;
    void setDefaultExtention(const QString &defaultExtention);

signals:

public slots:
};

#endif // APPCONTEXT_H
