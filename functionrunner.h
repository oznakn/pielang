#ifndef FUNCTIONRUNNER_H
#define FUNCTIONRUNNER_H

#include <QString>
#include <QStringList>
#include <QMap>

class FunctionRunner
{
public:
    FunctionRunner(QMap<QString, QString> *variableMap,QStringList *stringList,QStringList *usingList,QStringList *blockList, QMap<QString,QString> *funcMap, QString content);
    QString runFunctionRunner();

private:
    QString mContent;
    QStringList *mStringList;
    QStringList *mBlockList;
    QMap<QString,QString> *mFuncMap;
    QMap<QString, QString> *mVariableMap;

    QStringList *mUsingList;

    bool importOf(QString name);
    bool useOf(QString name,QString under = QString(""));
    bool underOf(QString name);
    QStringList getFunctionArgNames(QString contentOfFuncDef);
    QStringList getFunctionArgValues();
};

#endif // FUNCTIONPROCESSOR_H
