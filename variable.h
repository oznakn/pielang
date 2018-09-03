#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>
#include <QMap>
#include <QStringList>

class Variable
{
public:
    static bool isVariable(QString var);
    static QString combineVariables(QMap<QString,QString> *variableMap, QStringList *stringList,QStringList *usingList,QStringList *blockList,QMap<QString,QString> *funcList,QString val);
    static QString applyVariableOption(QMap<QString,QString> *variableMap,QString val);
    static QString applyStringOption(QStringList *stringList, QString val);
    static QString applyFuncOption(QMap<QString,QString> *variableMap, QStringList *stringList,QStringList *usingList,QStringList *blockList,QMap<QString,QString> *funcList,QString val);
};

#endif // VARIABLE_H
