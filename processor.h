#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QString>
#include <QStringList>
#include <QMap>

class Processor
{
public:
    Processor(QStringList *stringList,QStringList *usingList, QStringList *blockList, QMap<QString,QString> *funcMap,QString content);
    Processor(QMap<QString,QString> *variableMap,QStringList *usingList, QStringList *stringList,QStringList *blockList,QMap<QString,QString> *funcMap, QString content);
    ~Processor();
    QString runProcessor();

private:
    bool mProcessorIsEmpty = false;

    QString mContent;
    QStringList *mStringList;
    QStringList *mBlockList;
    QStringList *mInsertedVariableList;
    QMap<QString,QString> *mFuncMap;

    QStringList *mUsingList;

    QMap<QString,QString> *mVariableMap;

    void init();

    bool isVariable(QString val);

};

#endif // PROCESSOR_H
