#include <QCoreApplication>

#include "pilang.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PiLang lang("module");
    lang.runProcessor();

    return a.exec();
}
