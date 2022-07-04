#include "thumbnail.h"

thumbnail::thumbnail(win32_common *buffer,TYPE t,QWidget *parent)
    : QWidget{parent},type(t),buff(buffer)
{
    loadgraph = new load(false,this);

    auto hlayout0 = new QHBoxLayout;
    auto vlayout_0 = new QVBoxLayout;
    hlayout0->addWidget(loadgraph,1);
    hlayout0->addLayout(vlayout_0,3);
    setLayout(hlayout0);
    if (type == CpuPage) {
        auto cpu = new QLabel("CPU",this);
        vlayout_0->addWidget(cpu);
        char temp[20];
        sprintf(temp,"%u%% %.2f GHz",
                (unsigned)buff->getCpuLoad(buff->getLogicCpuCnt()),
                buff->getCpuClock());
        cpulabel = new QLabel(temp,this);
        vlayout_0->addWidget(cpulabel);
    }
    else if (type == MemPage){
        auto mem = new QLabel("内存",this);
        vlayout_0->addWidget(mem);
        char temp[30];
        sprintf(temp,"%.1f/%.1f GB (%lu%%)",
                buff->getPhyUsed()/1024/1024,
                buff->getPhyTot()/1024/1024,
                buff->getMemLoad());
        memlabel = new QLabel(temp,this);
        vlayout_0->addWidget(memlabel);
    }
}

thumbnail::~thumbnail()
{
    if (type == CpuPage)
        delete cpulabel;
    else if (type == MemPage)
        delete memlabel;
    delete loadgraph;
}

void thumbnail::refresh()
{
    if (type == CpuPage) {
        char temp[20];
        sprintf(temp,"%u%% %.2f GHz",
                (unsigned)buff->getCpuLoad(buff->getLogicCpuCnt()),
                buff->getCpuClock());
        cpulabel->setText(temp);
    }
    else if (type == MemPage){
        char temp[30];
        sprintf(temp,"%.1f/%.1f GB (%lu%%)",
                buff->getPhyUsed()/1024/1024,
                buff->getPhyTot()/1024/1024,
                buff->getMemLoad());
        memlabel->setText(temp);
    }

    double val(0);
    if (type == CpuPage) {
        val = buff->getCpuLoad(buff->getLogicCpuCnt());
    }
    else if (type == MemPage)
        val = buff->getMemLoad();
    loadgraph->refresh(val);
}

void thumbnail::setSeriesPen(QPen pen)
{
    loadgraph->setSeriesPen(pen);
}
