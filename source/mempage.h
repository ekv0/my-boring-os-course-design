#ifndef MEMPAGE_H
#define MEMPAGE_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "win32_common.h"
#include "gui_common.h"
#include "load.h"

class memPage : public QWidget
{
    Q_OBJECT
public:
    explicit memPage(win32_common *buffer,QWidget *parent = nullptr);
    ~memPage();

    void refresh();

private:
    win32_common *buff;
    load *loadgraph;

    QLabel *lmem;
    QLabel *tot;
    QLabel *used;
    QLabel *avail;
    QLabel *comm;
    QLabel *cached;
    QLabel *paged;
    QLabel *nonpaged;
    QLabel *spd;
    QLabel *slot;
    QLabel *shape;
    QLabel *reserv;

    //test layout pointer
    QGridLayout *glayout__0;
};

#endif // MEMPAGE_H
