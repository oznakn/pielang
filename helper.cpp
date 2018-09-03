#include "helper.h"

#include <QString>
#include <QMap>
#include <QStringList>
#include <QDebug>

void Helper::print(QMap<QString, QString> var,QString prefix)
{
    QString temp = "{";
    for(int i = 0; i < var.keys().length(); i++)
    {
        temp += QString("'") + var.keys().at(i) + QString("' : '") + var.value(var.keys().at(i)) + QString("'");
        if(i < var.keys().length() - 1)
            temp += QString(" , ");
    }
    temp += QString("}");
    qDebug().noquote() << "DEBUG MAP " << prefix << temp;
}

void Helper::print(QString var)
{
    qDebug().noquote() << "DEBUG" << var;
}

void Helper::scriptPrint(QString var)
{
    qDebug().noquote() << var;
}

QMap<QString,QString> Helper::combineMaps(QMap<QString,QString> map1, QMap<QString,QString> map2)
{
    for(int i = 0; i < map2.keys().length(); i++)
    {
        map1.insert(map2.keys().at(i) , map2.value(map2.keys().at(i)));
    }
    return map1;
}
