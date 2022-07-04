#include "performance.h"
#include <iostream>

performance::performance(win32_common *buff,QWidget *parent)
    : QWidget(parent)
{
    cpu = new cpuPage(buff);
    mem = new memPage(buff);
    cputhumbn = new thumbnail(buff,thumbnail::CpuPage);
    cputhumbn->setToolTip("CPU 活动");
    memthumbn = new thumbnail(buff,thumbnail::MemPage);
    memthumbn->setToolTip("使用中");
    //设置风格
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0x117dbb));
    cputhumbn->setSeriesPen(pen);
    pen.setColor(QColor(0x9528b4));
    memthumbn->setSeriesPen(pen);

    hlayout = new QHBoxLayout;
    vlayout = new QVBoxLayout;
    slayout = new QStackedLayout;

    vlayout->addWidget(cputhumbn,1);
    vlayout->addWidget(memthumbn,1);
    vlayout->addStretch(7);

    slayout->addWidget(cpu);
    slayout->addWidget(mem);

    hlayout->addLayout(vlayout,1);
    hlayout->addLayout(slayout,2);

    setLayout(hlayout);

    connect(this, SIGNAL(clicked()), this, SLOT(mouseClicked()));
    refresh();
}

performance::~performance()
{
}

void performance::mouseClicked()
{
    if (cputhumbn->geometry().contains(mousePos))
        slayout->setCurrentIndex(0);
    else if (memthumbn->geometry().contains(mousePos))
        slayout->setCurrentIndex(1);
}

void performance::mousePressEvent(QMouseEvent *ev)
{
    mousePos = QPoint(ev->x(), ev->y());
}

void performance::mouseReleaseEvent(QMouseEvent *ev)
{
    if(mousePos == QPoint(ev->x(), ev->y())) emit clicked();
}

void performance::refresh()
{
    cpu->refresh();
    mem->refresh();
    cputhumbn->refresh();
    memthumbn->refresh();
}
