#ifndef HELPER_H
#define HELPER_H

#include <QString>
#include <QMap>
#include <QStringList>

class Helper
{
public:
    static void print(QMap<QString,QString> var, QString prefix = QString(""));
    static void print(QString var);
    static void scriptPrint(QString var);
    static QMap<QString,QString> combineMaps(QMap<QString,QString> map1, QMap<QString,QString> map2);
};

#endif // HELPER_H
