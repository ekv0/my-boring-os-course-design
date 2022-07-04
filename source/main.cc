#include "taskmgr.h"

#include <QApplication>
#include <iostream>

#include <cstring>
#include "win32_common.h"

int main(int argc, char *argv[])
{
    if (!(argc == 2 && !strcmp(argv[1],"noadmin"))) {
        win32_common::openProc(argv[0],"noadmin",true);
        exit(0);
    }
    QApplication a(argc, argv);
    Taskmgr w;
    w.show();
    return a.exec();
}
