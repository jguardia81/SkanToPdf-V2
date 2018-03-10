#ifndef AUTOMATICFILENAMEFEEDER_H
#define AUTOMATICFILENAMEFEEDER_H
#include "appcontext.h"
#include <QString>
/**
 * @brief The AutomaticFileNameFeeder class
 * This class allows to generate an automatic filename
 * according tp the folder given in param
 */
class AutomaticFileNameFeeder
{
public:
    AutomaticFileNameFeeder();
    QString getFileName(const QString& path );
private:
    const QString FILENAME_TEMPLATE = "scanned_file*.pdf";

};

#endif // AUTOMATICFILENAMEFEEDER_H
