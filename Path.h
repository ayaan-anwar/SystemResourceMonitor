#ifndef PATH_H
#define PATH_H

//#include <string>
#include <QString>

const QString base_path = "C:\\dev\\projects\\OS_Project\\SystemResourceMonitor\\";

QString get_path(QString s)
{
    QString path = base_path + s;
    return path;
}

#endif // PATH_H
