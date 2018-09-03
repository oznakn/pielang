#include "pilang.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QDebug>

#include "processor.h"

PiLang::PiLang(QString fileName)
{
    this->mFileName = fileName;

    this->mStringList = new QStringList;
    this->mBlockList = new QStringList;
    this->mFuncList = new QStringList;
    this->mFuncMap = new QMap<QString,QString>;
    this->mUsingList = new QStringList;
}

PiLang::~PiLang()
{
    this->dispose();
}

void PiLang::dispose()
{
    delete this->mStringList;
    delete this->mBlockList;
    delete this->mFuncList;
    delete this->mFuncMap;
    delete this->mUsingList;
}

void PiLang::runProcessor()
{
    this->mFileContent = this->readFile();

    this->mFileContent = this->mFileContent.replace(QString("\n"),QString(""));
    this->mFileContent = this->mFileContent.replace(QString("\r"),QString(""));
    this->mFileContent = this->mFileContent.replace(QString("\t"),QString(""));
    this->mFileContent = this->mFileContent.replace(QString("\""),QString("'"));

    this->mUnchangedFileContent = this->mFileContent;

    this->runComments();
    this->runStrings();
    this->runFunctions();
    this->runFunctionConverter();
    this->runBlocks();

    Processor *p = new Processor(this->mStringList,this->mUsingList, this->mBlockList,this->mFuncMap, this->mFileContent);
    p->runProcessor();
    delete p;
}

void PiLang::runComments()
{
    QRegularExpression exp("\\/\\*(.*?)\\*\\/");

    QRegularExpressionMatchIterator it = exp.globalMatch(this->mFileContent);

    while(it.hasNext())
    {
        QRegularExpressionMatch match = it.next();

        this->mFileContent = this->mFileContent.mid(0,match.capturedStart()) + this->mFileContent.mid(match.capturedEnd() ,
                                 this->mFileContent.length() - match.capturedEnd());

        it = exp.globalMatch(this->mFileContent);
    }
}

void PiLang::runStrings()
{

    QRegularExpression exp("'(.*?)'");

    QRegularExpressionMatchIterator it = exp.globalMatch(this->mFileContent);

    while(it.hasNext())
    {
        QRegularExpressionMatch match = it.next();

        QString founded = this->mFileContent.mid(match.capturedStart() + 1 , match.capturedEnd() - match.capturedStart() - 2);

        this->mFileContent = this->mFileContent.mid(0,
                        match.capturedStart()) + QString("~") + QString::number(this->mStringList->size()) + QString("~") +
                            this->mFileContent.mid(match.capturedEnd() ,
                                 this->mFileContent.length() - match.capturedEnd());

        this->mStringList->push_back(founded);

        it = exp.globalMatch(this->mFileContent);
    }
}

void PiLang::runFunctions()
{
    QRegularExpression exp("function(.*?){(.*?)}");

    QRegularExpressionMatchIterator it = exp.globalMatch(this->mFileContent);

    while(it.hasNext())
    {
        QRegularExpressionMatch match = it.next();

        QString founded = this->mFileContent.mid(
                    this->mFileContent.mid(0, match.capturedStart()).lastIndexOf(";") + 1,
                    match.capturedEnd() - this->mFileContent.mid(0, match.capturedStart()).lastIndexOf(";") -1);

        this->mFileContent = this->mFileContent.mid(0 , this->mFileContent.mid(0, match.capturedStart()).lastIndexOf(";") + 1) +
                QString("$") + QString::number(this->mFuncList->size()) + QString("$;") +
                this->mFileContent.mid(match.capturedEnd() , this->mFileContent.length() - match.capturedEnd());

        this->mFuncList->push_back(founded);

        it = exp.globalMatch(this->mFileContent);
    }
}

void PiLang::runFunctionConverter()
{
    for(int i = 0; i < this->mFuncList->size(); i++)
    {
        QString nameOfFunction = this->mFuncList->at(i).mid(0, this->mFuncList->at(i).indexOf("("));
        nameOfFunction = nameOfFunction.split(" ")[1].trimmed();

        this->mFuncMap->insert(nameOfFunction , this->mFuncList->at(i));
    }

    delete this->mFuncList;
}

void PiLang::runBlocks()
{
    QRegularExpression exp("{(.*?)}");

    QRegularExpressionMatchIterator it = exp.globalMatch(this->mFileContent);

    QString beforeFounded = "";

    while(it.hasNext())
    {
        QRegularExpressionMatch match = it.next();

        QString founded = this->mFileContent.mid(
                    this->mFileContent.mid(0, match.capturedStart()).lastIndexOf(";") + 1,
                    match.capturedEnd() - this->mFileContent.mid(0, match.capturedStart()).lastIndexOf(";") -1);

        this->mFileContent = this->mFileContent.mid(0 , this->mFileContent.mid(0, match.capturedStart()).lastIndexOf(";") + 1) +
                QString(":") + QString::number(this->mBlockList->size()) + QString(":;") +
                this->mFileContent.mid(match.capturedEnd() , this->mFileContent.length() - match.capturedEnd());

        if(founded.indexOf(QString("else if")) == 0)
        {
            beforeFounded += founded;
        }
        else if(founded.indexOf(QString("else")) == 0)
        {
            beforeFounded += founded;
            this->mBlockList->push_back(beforeFounded);
            beforeFounded = "";
        }
        else
        {
            this->mBlockList->push_back(founded);
        }

        it = exp.globalMatch(this->mFileContent);
    }
}

QString PiLang::getFilePath()
{
    return QString("E:/") + this->mFileName + QString(".pi");
}

void PiLang::writeToFile(QString data)
{
    QFile file(this->getFilePath());
    file.open(QFile::WriteOnly);

    QTextStream in(&file);

    in << data;
}


QString PiLang::readFile()
{
    QString result = "";

    QFile file(this->getFilePath());
    file.open(QFile::ReadOnly);

    QTextStream out(&file);
    while(!out.atEnd())
    {
        result += out.readLine();
    }

    return result;
}

QString PiLang::getSeperatRegex()
{
    return QString("%1(?=([^']*'[^']*')*[^']*$)");
}
