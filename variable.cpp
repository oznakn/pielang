#include "variable.h"

#include <QString>
#include <QMap>
#include <QStringList>
#include <QDebug>

#include "functionrunner.h"
#include "helper.h"

bool Variable::isVariable(QString var)
{
    bool r = false;
    for(int i = 0; i < var.length(); i++)
    {
        if( !(var.at(i) == QChar('0') ||
               var.at(i) == QChar('1') ||
               var.at(i) == QChar('2') ||
               var.at(i) == QChar('3') ||
               var.at(i) == QChar('4') ||
               var.at(i) == QChar('5') ||
               var.at(i) == QChar('6') ||
               var.at(i) == QChar('7') ||
               var.at(i) == QChar('8') ||
               var.at(i) == QChar('9') ||
               var.at(i) == QChar(',') ||
               var.at(i) == QChar('-') ) )
        {

            r = true;
        }
    }
    return r;
}

QString Variable::applyVariableOption(QMap<QString,QString> *variableMap, QString valTemp)
{

    if(Variable::isVariable(valTemp) && valTemp.indexOf("~") != 0)
    {
        valTemp = variableMap->value(valTemp);
    }
    return valTemp;
}

QString Variable::applyStringOption(QStringList *stringList, QString valTemp)
{
    if(valTemp.contains("~"))
    {
        valTemp = stringList->at(valTemp.mid(1 , valTemp.length() - 2).toInt());
    }
    return valTemp;
}

QString Variable::applyFuncOption(QMap<QString,QString> *variableMap, QStringList *stringList,QStringList *usingList,QStringList *blockList,QMap<QString,QString> *funcMap, QString valTemp)
{
    if(valTemp.contains("(") && valTemp.contains(")"))
    {
        FunctionRunner *r = new FunctionRunner(variableMap, stringList,usingList,blockList,funcMap, valTemp.trimmed());
        valTemp = r->runFunctionRunner();
        delete r;
    }
    return valTemp;
}

QString Variable::combineVariables(QMap<QString,QString> *variableMap, QStringList *stringList,QStringList *usingList,QStringList *blockList,QMap<QString,QString> *funcMap, QString val)
{
    QString valNew = "";

    if(!val.contains(" * ") && !val.contains(" - ") && !val.contains(" / ") && !val.contains(" + ")) {
        QStringList valList = val.split(".");

        for(int j = 0; j < valList.size(); j++)
        {
            QString valTemp = valList.at(j).trimmed();
            valList[j] = Variable::applyFuncOption(variableMap, stringList,usingList,blockList,funcMap, valTemp);
        }

        for(int j = 0; j < valList.size(); j++)
        {
            QString valTemp = valList.at(j).trimmed();

            valList[j] = Variable::applyVariableOption(variableMap, valTemp);
        }

        for(int j = 0; j < valList.size(); j++)
        {
            QString valTemp = valList.at(j);

            valList[j] = Variable::applyStringOption(stringList, valTemp);
            valNew += valList.at(j);
        }
    }
    else if(val.contains(" * "))
    {
        QStringList valList = val.split(" * ");
        valNew = QString::number(applyVariableOption(variableMap, valList.at(0).trimmed()).toDouble() *
                                 applyVariableOption(variableMap, valList.at(1).trimmed()).toDouble());
    }
    else if(val.contains(" / "))
    {
        QStringList valList = val.split(" / ");
        valNew = QString::number(applyVariableOption(variableMap, valList.at(0).trimmed()).toDouble() /
                                 applyVariableOption(variableMap, valList.at(1).trimmed()).toDouble());
    }
    else if(val.contains(" + "))
    {
        QStringList valList = val.split(" + ");
        valNew = QString::number(applyVariableOption(variableMap, valList.at(0).trimmed()).toDouble() +
                                 applyVariableOption(variableMap, valList.at(1).trimmed()).toDouble());
    }
    else if(val.contains(" - "))
    {
        QStringList valList = val.split(" - ");
        valNew = QString::number(applyVariableOption(variableMap, valList.at(0).trimmed()).toDouble() -
                                 applyVariableOption(variableMap, valList.at(1).trimmed()).toDouble());
    }
    return valNew;
}
