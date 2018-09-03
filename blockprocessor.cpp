#include "blockprocessor.h"
#include <QString>
#include <QStringList>
#include <QMap>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

#include "processor.h"
#include "variable.h"
#include "helper.h"

BlockProcessor::BlockProcessor(QMap<QString,QString> *variableMap,QStringList *blockList, QStringList *stringList,QStringList *usingList,QMap<QString,QString> *funcMap, QString content)
{
    this->mVariableMap = variableMap;
    this->mBlockList = blockList;
    this->mStringList = stringList;
    this->mFuncMap = funcMap;
    this->mContent = content;
    this->mUsingList = usingList;

    this->mInsertedVariableList = new QStringList;

    this->mStateContent = this->mContent.mid(0,this->mContent.indexOf("{"));
    this->mBodyContent = this->mContent.mid(this->mContent.indexOf("{") + 1 , this->mContent.length() - 2 - this->mContent.indexOf("{"));

    QString nameOfBlock = this->mStateContent.mid(0, this->mStateContent.indexOf("(")).trimmed();

    if(nameOfBlock == "if")
    {
        this->mBlockType = BlockProcessor::IF;
    }

    else if(nameOfBlock == "while")
    {
        this->mBlockType = BlockProcessor::WHILE;
    }
}

BlockProcessor::~BlockProcessor()
{
    for(int i = 0; i < this->mInsertedVariableList->length();i++)
    {
        this->mVariableMap->remove(this->mInsertedVariableList->at(i));
    }

    delete this->mInsertedVariableList;
}

bool BlockProcessor::checkRunnable(QString comparator, QStringList args)
{
    if(this->mBlockType == BlockProcessor::IF || this->mBlockType == BlockProcessor::WHILE)
    {
       args[0] = Variable::combineVariables(this->mVariableMap , this->mStringList , this->mUsingList , this->mBlockList , this->mFuncMap , ((QString)args.at(0)).trimmed());
       args[1] = Variable::combineVariables(this->mVariableMap , this->mStringList , this->mUsingList ,this->mBlockList , this->mFuncMap , ((QString)args.at(1)).trimmed());

       if(comparator == "==")
       {
           return args[0] == args[1];
       }
       else if(comparator == "!=")
       {
           return args[0] != args[1];
       }
       else if(comparator == "<")
       {
           return args[0] < args[1];
       }
       else if(comparator == ">")
       {
           return args[0] > args[1];
       }
    }
    return false;
}

void BlockProcessor::runBlockProcessor()
{
    QString comparator;
    QStringList args;

    if(this->mBlockType == BlockProcessor::IF || this->mBlockType == BlockProcessor::WHILE)
    {
       QString content = this->mStateContent.mid(this->mStateContent.indexOf("(") + 1 , this->mStateContent.lastIndexOf(")") - 1 - this->mStateContent.indexOf("(")).trimmed();
       QRegularExpression regex("(==|!=|<|>)");
       args = content.split(regex);

       QRegularExpressionMatchIterator it = regex.globalMatch(content);
       comparator = it.next().captured();
    }

    if(this->mBlockType == BlockProcessor::IF)
    {
        if(checkRunnable(comparator , args))
        {
            Processor *p = new Processor(this->mVariableMap,this->mStringList, this->mUsingList, this->mBlockList, this->mFuncMap , this->mBodyContent);
            p->runProcessor();
            delete p;
        }
    }
    else if(this->mBlockType == BlockProcessor::WHILE)
    {
        while(checkRunnable(comparator , args))
        {
            Processor *p = new Processor(this->mVariableMap,this->mStringList ,this->mUsingList, this->mBlockList, this->mFuncMap , this->mBodyContent);
            p->runProcessor();
            delete p;
        }
    }

}
