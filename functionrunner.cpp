#include "functionrunner.h"

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDebug>
#include <math.h>

#include "helper.h"
#include "processor.h"
#include "variable.h"

FunctionRunner::FunctionRunner(QMap<QString,QString> *variableMap, QStringList *stringList,QStringList *usingList,QStringList *blockList,QMap<QString,QString> *funcMap, QString content)
{
    this->mVariableMap = variableMap;
    this->mStringList = stringList;
    this->mBlockList = blockList;
    this->mFuncMap = funcMap;
    this->mContent = content;
    this->mUsingList = usingList;

    Helper::print(QString("Function Processor Started in ") + content);
}

QStringList FunctionRunner::getFunctionArgNames(QString contentOfFuncDef)
{
    QString argsContent = contentOfFuncDef.mid(contentOfFuncDef.indexOf("(") + 1, contentOfFuncDef.indexOf(")") - contentOfFuncDef.indexOf("(") - 1);

    Helper::print(QString("Function Processor Arg Names Content: ") + argsContent + QString(" in ") + this->mContent);

    QStringList list = argsContent.split(",");
    for(int i = 0; i < list.size(); i++)
    {
        list[i] = list.at(i).trimmed();

        Helper::print(QString("Function Processor New Arg Name Found: ") + list.at(i));
    }
    return list;
}

QStringList FunctionRunner::getFunctionArgValues()
{
    QString argsContent = this->mContent.mid(this->mContent.indexOf("(") + 1, this->mContent.indexOf(")") - this->mContent.indexOf("(") - 1);

    Helper::print(QString("Function Processor Arg Values Content: ") + argsContent + QString(" in ") + this->mContent);

    QStringList list = argsContent.split(",");
    for(int i = 0; i < list.size(); i++)
    {
        Helper::print(*(this->mVariableMap),QString("ArgVAlues"));
        list[i] = Variable::combineVariables(this->mVariableMap , this->mStringList,this->mUsingList,this->mBlockList,this->mFuncMap, list.at(i));

        Helper::print(QString("Function Processor New Arg Value Found: ") + list.at(i));
    }

    return list;
}

bool FunctionRunner::importOf(QString name)
{
    return this->mUsingList->contains(name);
}

bool FunctionRunner::useOf(QString name,QString under)
{
    QString val = "";
    if(under != QString(""))
        val = under + QString("#");

    return (this->mContent.trimmed().indexOf(val + name + QString("(")) == 0);
}

bool FunctionRunner::underOf(QString name)
{
    return (this->mContent.trimmed().indexOf(name + QString("#")) == 0) && this->importOf(name);
}

QString FunctionRunner::runFunctionRunner()
{
    if(this->mFuncMap->contains(this->mContent.mid(0, this->mContent.indexOf("("))))
    {
        QString content = this->mFuncMap->value(this->mContent.mid(0, this->mContent.indexOf("(")));

        QMap<QString,QString> variableMapNew = *this->mVariableMap;
        QStringList list1 = this->getFunctionArgNames(content);
        QStringList list2 = this->getFunctionArgValues();
        for(int i = 0; i < list1.length(); i++)
        {
            variableMapNew.insert(list1.at(i) , list2.at(i));
        }

        content = content.mid(content.indexOf("{") + 1 , content.lastIndexOf("}") - content.indexOf("{") - 1);

        Helper::print(variableMapNew,"FunctionRunner");
        Processor *p = new Processor(&variableMapNew,this->mStringList,this->mUsingList,this->mBlockList, this->mFuncMap,content);

        QString val = p->runProcessor();
        delete p;
        return val;
    }
    else {
        if(this->underOf("Math"))
        {
            if(this->useOf(QString("pow"),QString("Math")))
            {
                return QString::number(pow(getFunctionArgValues()[0].toDouble(), getFunctionArgValues()[1].toDouble()));
            }
            else if(this->useOf(QString("abs"),QString("Math")))
            {
                return QString::number(abs(getFunctionArgValues()[0].toDouble()));
            }
            else if(this->useOf(QString("toDegree"),QString("Math")))
            {
                return QString::number((180 * (getFunctionArgValues()[0].replace(",",".").toDouble())) / M_PI).replace("." , ",");
            }
            else if(this->useOf(QString("toRadian"),QString("Math")))
            {
                return QString::number((M_PI * (getFunctionArgValues()[0].replace(",",".").toDouble())) / 180).replace("." , ",");
            }
            else if(this->useOf(QString("sin"),QString("Math")))
            {
                return QString::number(sin(getFunctionArgValues()[0].toDouble())).replace("." , ",");
            }
            else if(this->useOf(QString("cos"),QString("Math")))
            {
                return QString::number(cos(getFunctionArgValues()[0].toDouble())).replace("." , ",");
            }
            else if(this->useOf(QString("tan"),QString("Math")))
            {
                return QString::number(tan(getFunctionArgValues()[0].toDouble())).replace("." , ",");
            }
            else if(this->useOf(QString("cot"),QString("Math")))
            {
                return QString::number(1/(tan(getFunctionArgValues()[0].toDouble()))).replace("." , ",");
            }
            else if(this->useOf(QString("sec"),QString("Math")))
            {
                return QString::number(1/(cos(getFunctionArgValues()[0].toDouble()))).replace("." , ",");
            }
            else if(this->useOf(QString("cosec"),QString("Math")))
            {
                return QString::number(1/(sin(getFunctionArgValues()[0].toDouble()))).replace("." , ",");
            }
        }
        else if(this->importOf("System"))
        {
            if(this->useOf(QString("print")))
            {
                Helper::scriptPrint(getFunctionArgValues()[0]);
            }
        }
    }
    return QString::null;
}
