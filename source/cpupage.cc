#include "cpupage.h"

cpuPage::cpuPage(win32_common *buffer,QWidget *parent)
    : QWidget(parent),buff(buffer)
{
    cnt = buff->getLogicCpuCnt();

    //upside
    auto hlayout0 = new QHBoxLayout;
    lcpu = new QLabel("<font size=\"7\">CPU</font>",this);
    hlayout0->addWidget(lcpu,Qt::AlignLeft);

    std::string info = buff->getCpuInfo();
    rinfo = new QLabel(
                QString("<font size=\"7\">") +
                QString::fromStdString(info) +
                QString("</font>"),
                this);
    hlayout0->addWidget(rinfo,Qt::AlignRight);

    //downside left
    auto glayout_0 = new QGridLayout;
    auto loadlb = infolabel("利用率",this);
    glayout_0->addWidget(loadlb,0,0,Qt::AlignLeft | Qt::AlignBottom);
    char temp[10];
    sprintf(temp,"%d%%",(int)buff->getCpuLoad(cnt));
    load = vallabel(temp,this);
    glayout_0->addWidget(load,1,0,Qt::AlignLeft | Qt::AlignTop);
    auto spdlb = infolabel("速度",this);
    glayout_0->addWidget(spdlb,0,1,Qt::AlignLeft | Qt::AlignBottom);
    sprintf(temp,"%.2f GHz",buff->getCpuClock());
    spd = vallabel(temp,this);
    glayout_0->addWidget(spd,1,1,Qt::AlignLeft | Qt::AlignTop);
    auto proclb = infolabel("进程",this);
    glayout_0->addWidget(proclb,2,0,Qt::AlignLeft | Qt::AlignBottom);
    proc = vallabel(QString::number(buff->getProcCnt()),this);
    glayout_0->addWidget(proc,3,0,Qt::AlignLeft | Qt::AlignTop);
    auto thrdlb = infolabel("线程",this);
    glayout_0->addWidget(thrdlb,2,1,Qt::AlignLeft | Qt::AlignBottom);
    thrd = vallabel(QString::number(buff->getThrdCnt()),this);
    glayout_0->addWidget(thrd,3,1,Qt::AlignLeft | Qt::AlignTop);
    auto hdlb = infolabel("句柄",this);
    glayout_0->addWidget(hdlb,2,2,Qt::AlignLeft | Qt::AlignBottom);
    hd = vallabel(QString::number(buff->getHandlCnt()),this);
    glayout_0->addWidget(hd,3,2,Qt::AlignLeft | Qt::AlignTop);
    auto nrtlb = infolabel("正常运行时间",this);
    glayout_0->addWidget(nrtlb,4,0,Qt::AlignLeft | Qt::AlignBottom);
    nrt = vallabel(QString::fromStdString(buff->getTimeSinceStart()),this);
    glayout_0->addWidget(nrt,5,0,Qt::AlignLeft | Qt::AlignTop);

    //middle
    auto glayout0 = new QGridLayout;

    using pload = class load *;
    loadgraphs = new pload[cnt];
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0x117dbb));
    for (unsigned i = 0; i < cnt; ++i) {
        loadgraphs[i] = new class load(true);
        loadgraphs[i]->setXtick(7);
        loadgraphs[i]->setYtick(11);
        loadgraphs[i]->setToolTip("CPU " + QString::number(i));
        loadgraphs[i]->setSeriesPen(pen);   //设置风格
    }
    unsigned row,col;
    decomp(cnt,row,col);
    unsigned i(0);
    for (unsigned r = 0; r < row; ++r) {
        for (unsigned c = 0; c < col; ++c) {
            glayout0->addWidget(loadgraphs[i++],r,c);
        }
    }

    //downside right
    auto glayout_1 = new QGridLayout;
    auto baselb = infolabel("基准速度：",this);
    glayout_1->addWidget(baselb,0,0,Qt::AlignLeft | Qt::AlignBaseline);
    sprintf(temp,"%.2f GHz",buff->getCpuBaseClock());
    base = infolabel(temp,this);
    glayout_1->addWidget(base,0,1,Qt::AlignLeft | Qt::AlignBaseline);
    auto slotlb = infolabel("插槽：",this);
    glayout_1->addWidget(slotlb,1,0,Qt::AlignLeft | Qt::AlignBaseline);
    slot = infolabel(QString::number(buff->getCpuSlot()),this);
    glayout_1->addWidget(slot,1,1,Qt::AlignLeft | Qt::AlignBaseline);
    auto corelb = infolabel("内核：",this);
    glayout_1->addWidget(corelb,2,0,Qt::AlignLeft | Qt::AlignBaseline);
    core = infolabel(QString::number(buff->getCpuCoreCnt()),this);
    glayout_1->addWidget(core,2,1,Qt::AlignLeft | Qt::AlignBaseline);
    auto lgcrlb = infolabel("逻辑处理器：",this);
    glayout_1->addWidget(lgcrlb,3,0,Qt::AlignLeft | Qt::AlignBaseline);
    lgcr = infolabel(QString::number(buff->getLogicCpuCnt()),this);
    glayout_1->addWidget(lgcr,3,1,Qt::AlignLeft | Qt::AlignBaseline);
    auto vrlb = infolabel("虚拟化：",this);
    glayout_1->addWidget(vrlb,4,0,Qt::AlignLeft | Qt::AlignBaseline);
    vr = infolabel(QString::fromStdString(buff->getVl()),this);
    glayout_1->addWidget(vr,4,1,Qt::AlignLeft | Qt::AlignBaseline);
    auto l1clb = infolabel("L1 缓存：",this);
    glayout_1->addWidget(l1clb,5,0,Qt::AlignLeft | Qt::AlignBaseline);
    l1c = infolabel(QString::number(buff->getL1Cache())
                         + " KB",this);
    glayout_1->addWidget(l1c,5,1,Qt::AlignLeft | Qt::AlignBaseline);
    auto l2clb = infolabel("L2 缓存：",this);
    glayout_1->addWidget(l2clb,6,0,Qt::AlignLeft | Qt::AlignBaseline);
    l2c = infolabel(QString::number(buff->getL2Cache()/1024UL)
                         + " MB",this);
    glayout_1->addWidget(l2c,6,1,Qt::AlignLeft | Qt::AlignBaseline);
    auto l3clb = infolabel("L3 缓存：",this);
    glayout_1->addWidget(l3clb,7,0,Qt::AlignLeft | Qt::AlignBaseline);
    l3c = infolabel(QString::number(buff->getL3Cache()/1024UL)
                         + " MB",this);
    glayout_1->addWidget(l3c,7,1,Qt::AlignLeft | Qt::AlignBaseline);

    auto hlayout1 = new QHBoxLayout;
    hlayout1->addLayout(glayout_0,3);
    hlayout1->addLayout(glayout_1,2);
    hlayout1->addStretch(1);

    auto vlayout0 = new QVBoxLayout;
    vlayout0->addLayout(hlayout0,1);
    vlayout0->addLayout(glayout0,4);
    vlayout0->addLayout(hlayout1,3);

    setLayout(vlayout0);
}

cpuPage::~cpuPage()
{
    for (unsigned i = 0; i < cnt; ++i) {
        delete loadgraphs[i];
    }
    delete loadgraphs;
}

void cpuPage::refresh()
{
    rinfo->setText(QString::fromStdString(buff->getCpuInfo()));
    char temp[10];
    sprintf(temp,"%d%%",(int)buff->getCpuLoad(cnt));
    load->setText(temp);
    sprintf(temp,"%.2f GHz",buff->getCpuClock());
    spd->setText(temp);
    proc->setText(QString::number(buff->getProcCnt()));
    thrd->setText(QString::number(buff->getThrdCnt()));
    hd->setText(QString::number(buff->getHandlCnt()));
    nrt->setText(QString::fromStdString(buff->getTimeSinceStart()));

    sprintf(temp,"%.2f GHz",buff->getCpuBaseClock());
    base->setText(temp);
    slot->setText(QString::number(buff->getCpuSlot()));
    core->setText(QString::number(buff->getCpuCoreCnt()));
    lgcr->setText(QString::number(buff->getLogicCpuCnt()));
    vr->setText(QString::fromStdString(buff->getVl()));
    l1c->setText(QString::number(buff->getL1Cache()) + " KB");
    l2c->setText(QString::number(buff->getL2Cache()/1024UL) + " MB");
    l3c->setText(QString::number(buff->getL3Cache()/1024UL) + " MB");

    for (unsigned i = 0; i < cnt; ++i) {
        loadgraphs[i]->refresh(buff->getCpuLoad(i));
    }
}

void cpuPage::decomp(unsigned tot,unsigned &row,unsigned &col)
{
    unsigned x = static_cast<unsigned>(sqrt(static_cast<double>(tot)));
    while (static_cast<double>(tot)/x != tot/x)
        --x;
    row = (x > tot/x) ? tot/x : x;
    col = (x > tot/x) ? x : tot/x;
}
