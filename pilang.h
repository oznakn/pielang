#ifndef PILANG_H
#define PILANG_H

#include <QString>
#include <QStringList>
#include <QMap>

class PiLang
{

public:
    PiLang(QString fileName);
    ~PiLang();
    void runProcessor();
    void dispose();

private:
    QString mFileName;
    QString mUnchangedFileContent;
    QString mFileContent;

    QStringList *mStringList;
    QStringList *mFuncList;
    QMap<QString,QString> *mFuncMap;
    QStringList *mBlockList;

    QStringList *mUsingList;

    void runComments();
    void runStrings();
    void runFunctions();
    void runFunctionConverter();
    void runBlocks();

    QString getSeperatRegex();
    QString getFilePath();
    QString readFile();
    void writeToFile(QString data);
};

#endif // PILANG_H
