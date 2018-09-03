#include "processor.h"

#include <QString>
#include <QChar>
#include <QDebug>
#include <QMap>

#include "functionrunner.h"
#include "helper.h"
#include "variable.h"
#include "blockprocessor.h"

Processor::Processor(QStringList *stringList, QStringList *usingList,QStringList *blockList, QMap<QString,QString> *funcMap, QString content)
{
    this->mStringList = stringList;
    this->mBlockList = blockList;
    this->mFuncMap = funcMap;

    this->mUsingList = usingList;

    this->mInsertedVariableList = new QStringList;

    this->mContent = content;

    this->mVariableMap = new QMap<QString , QString>;
    this->init();
}

Processor::Processor(QMap<QString,QString> *variableMap, QStringList *stringList,QStringList *usingList,QStringList *blockList,QMap<QString,QString> *funcMap, QString content)
{
    this->mStringList = stringList;
    this->mBlockList = blockList;
    this->mFuncMap = funcMap;

    this->mUsingList = usingList;

    this->mContent = content;

    this->mInsertedVariableList = new QStringList;

    QMap<QString,QString> variableMapNew = (*variableMap);
    this->mVariableMap = &variableMapNew;

    Helper::print(*(this->mVariableMap),"Processor");

    this->init();
}

void Processor::init()
{
    Helper::print(QString("Processor Started in ") + this->mContent);

    if(this->mContent.size() == 0 || this->mContent == QString("") || this->mContent == QString(" "))
    {
        Helper::print(QString("Processor is Empty in " + this->mContent));
        this->mProcessorIsEmpty = true;
    }
}

Processor::~Processor()
{
    //delete this->mVariableMap;

    for(int i = 0; i < this->mInsertedVariableList->size(); i++)
    {
        this->mVariableMap->remove(this->mInsertedVariableList->at(i));
    }
    delete this->mInsertedVariableList;
}

QString Processor::runProcessor()
{
    if(!this->mProcessorIsEmpty)
    {
        Helper::print(QString("Code Processor Started") + QString(" in ") + this->mContent);
        QStringList processList = this->mContent.split(";");

        for(int i = 0; i < processList.size(); i++)
        {
            if(processList.at(i).contains("="))
            {
                QString name = processList.at(i).split("=")[0].trimmed();
                QString val = processList.at(i).split("=")[1].trimmed();
                QString valNew = "";

                valNew = Variable::combineVariables(this->mVariableMap , this->mStringList,this->mUsingList,this->mBlockList,this->mFuncMap, val);

                if(!this->mVariableMap->contains(name))
                    this->mInsertedVariableList->push_back(name);

                this->mVariableMap->insert(name, valNew);

                Helper::print(QString("New Variable Found: ") + name + QString(" Val: ") + valNew + QString(" in ") + processList.at(i));
            }
            else if(processList.at(i).contains(":"))
            {
                QString val = processList.at(i).mid(1, processList.at(i).length() - 2);

                BlockProcessor *p = new BlockProcessor(this->mVariableMap,this->mBlockList, this->mStringList,this->mUsingList, this->mFuncMap, this->mBlockList->at(val.toInt()));
                p->runBlockProcessor();
                delete p;
            }
            else if(processList.at(i).indexOf("return") == 0)
            {
                return this->mVariableMap->value(processList.at(i).split(" ")[1]);
            }           
            else if(processList.at(i).indexOf("using") == 0)
            {
                QString val = processList.at(i).split(" ")[1].trimmed();
                this->mUsingList->push_back(val);
            }
            else if(!processList.at(i).contains(QString("$")))
            {
                FunctionRunner *p = new FunctionRunner(this->mVariableMap, this->mStringList,this->mUsingList, this->mBlockList,this->mFuncMap, processList.at(i));
                p->runFunctionRunner();
                delete p;
            }
        }
    }
    return QString::null;
}


