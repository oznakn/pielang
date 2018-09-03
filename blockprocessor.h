#ifndef BLOCKPROCESSOR_H
#define BLOCKPROCESSOR_H

#include <QString>
#include <QMap>
#include <QStringList>

class BlockProcessor
{
public:
    BlockProcessor(QMap<QString,QString> *variableMap, QStringList *blockList, QStringList *stringList,QStringList *usingList, QMap<QString , QString> *funcMap, QString content);
    ~BlockProcessor();
    void runBlockProcessor();

private:
    const static int IF = 0;
    const static int WHILE = 1;

    int mBlockType;
    QString mContent;
    QString mStateContent;
    QString mBodyContent;
    QStringList *mBlockList;
    QStringList *mStringList;
    QMap<QString,QString> *mFuncMap;

    QStringList *mUsingList;

    QStringList *mInsertedVariableList;

    QMap<QString,QString> *mVariableMap;

    bool checkRunnable(QString comparator , QStringList args);
};

#endif // BLOCKPROCESSOR_H
