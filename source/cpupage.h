#ifndef CPUPAGE_H
#define CPUPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QString>

#include <cmath>

#include "win32_common.h"
#include "gui_common.h"
#include "load.h"

class cpuPage : public QWidget
{
    Q_OBJECT
public:
    explicit cpuPage(win32_common *buffer,QWidget *parent = nullptr);
    ~cpuPage();

    void refresh();

private:
    win32_common *buff;
    unsigned cnt;
    load **loadgraphs;

    QLabel *lcpu;
    QLabel *rinfo;
    QLabel *load;
    QLabel *spd;
    QLabel *proc;
    QLabel *thrd;
    QLabel *hd;
    QLabel *nrt;

    QLabel *base;
    QLabel *slot;
    QLabel *core;
    QLabel *lgcr;
    QLabel *vr;
    QLabel *l1c;
    QLabel *l2c;
    QLabel *l3c;

    void decomp(unsigned tot,unsigned &row,unsigned &col);
};

#endif // CPUPAGE_H
