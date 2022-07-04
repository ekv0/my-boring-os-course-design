#include "mempage.h"

memPage::memPage(win32_common *buffer,QWidget *parent)
    : QWidget(parent),buff(buffer)
{
    //upside
    auto hlayout_0 = new QHBoxLayout;
    lmem = new QLabel("<font size=\"7\">内存</font>",this);
    hlayout_0->addWidget(lmem,Qt::AlignLeft);

    char temp[10];
    sprintf(temp,"%.1f GB",buff->getPhyTot()/1024/1024);
    QString qstr("<font size=\"6\">");
    qstr += temp;
    qstr += "</font>";
    tot = new QLabel(qstr,this);
    hlayout_0->addWidget(tot,Qt::AlignRight);

    //middle
    loadgraph = new load(true);
    loadgraph->setXtick(17);
    loadgraph->setYtick(11);
    loadgraph->setToolTip("使用中");
    //设置风格
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(0x9528b4));
    loadgraph->setSeriesPen(pen);

    //downside
    glayout__0 = new QGridLayout;
    auto usedlb = infolabel("使用中",this);
    glayout__0->addWidget(usedlb,0,0,Qt::AlignLeft | Qt::AlignBottom);
    sprintf(temp,"%.1f GB",buff->getPhyUsed()/1024/1024);
    used = vallabel(temp,this);
    glayout__0->addWidget(used,1,0,Qt::AlignLeft | Qt::AlignTop);
    auto availlb = infolabel("可用",this);
    glayout__0->addWidget(availlb,0,1,Qt::AlignLeft | Qt::AlignBottom);
    sprintf(temp,"%.1f GB",buff->getPhyAvail()/1024/1024);
    avail = vallabel(temp,this);
    glayout__0->addWidget(avail,1,1,Qt::AlignLeft | Qt::AlignTop);
    auto commlb = infolabel("已提交",this);
    glayout__0->addWidget(commlb,2,0,Qt::AlignLeft | Qt::AlignBottom);
    sprintf(temp,"%.1f GB",buff->getCommTot()/1024/1024);
    comm = vallabel(temp,this);
    glayout__0->addWidget(comm,3,0,Qt::AlignLeft | Qt::AlignTop);
    auto cachedlb = infolabel("已缓存",this);
    glayout__0->addWidget(cachedlb,2,1,Qt::AlignLeft | Qt::AlignBottom);
    sprintf(temp,"%d MB",(int)buff->getSysCache()/1024);
    cached = vallabel(temp,this);
    glayout__0->addWidget(cached,3,1,Qt::AlignLeft | Qt::AlignTop);
    auto pagedlb = infolabel("分页缓冲池",this);
    glayout__0->addWidget(pagedlb,4,0,Qt::AlignLeft | Qt::AlignBottom);
    sprintf(temp,"%d MB",(int)buff->getPaged()/1024);
    paged = vallabel(temp,this);
    glayout__0->addWidget(paged,5,0,Qt::AlignLeft | Qt::AlignTop);
    auto nonpagedlb = infolabel("非分页缓冲池",this);
    glayout__0->addWidget(nonpagedlb,4,1,Qt::AlignLeft | Qt::AlignBottom);
    sprintf(temp,"%d MB",(int)buff->getNonpaged()/1024);
    nonpaged = vallabel(temp,this);
    glayout__0->addWidget(nonpaged,5,1,Qt::AlignLeft | Qt::AlignTop);

    auto vlayout__0 = new QVBoxLayout;
    auto glayout___0 = new QGridLayout;
    auto spdlb = infolabel("速度：",this);
    glayout___0->addWidget(spdlb,0,0,Qt::AlignLeft | Qt::AlignBaseline);
    sprintf(temp,"%lu MHz",buff->getMemSpeed());
    spd = infolabel(temp,this);
    glayout___0->addWidget(spd,0,1,Qt::AlignLeft | Qt::AlignBaseline);
    auto slotlb = infolabel("已使用的插槽：",this);
    glayout___0->addWidget(slotlb,1,0,Qt::AlignLeft | Qt::AlignBaseline);
    sprintf(temp,"%ud/%ud",buff->getMemUsedSlot(),buff->getMemTotSlot());
    slot = infolabel(temp,this);
    auto shapelb = infolabel("外形规格：",this);
    glayout___0->addWidget(shapelb,2,0,Qt::AlignLeft | Qt::AlignBaseline);
    shape = infolabel(QString::fromStdString(buff->getMemShape()),this);
    glayout___0->addWidget(shape,2,1,Qt::AlignLeft | Qt::AlignBaseline);
    auto reservlb = infolabel("为硬件保留的内存：",this);
    glayout___0->addWidget(reservlb,3,0,Qt::AlignLeft | Qt::AlignBaseline);
    sprintf(temp,"%lu MB",buff->getMemReserved()/1024);
    reserv = infolabel(temp,this);
    glayout___0->addWidget(reserv,3,1,Qt::AlignLeft | Qt::AlignBaseline);
    glayout___0->setVerticalSpacing(-50);
    vlayout__0->addLayout(glayout___0,6);
    vlayout__0->addStretch(4);

    auto hlayout_1 = new QHBoxLayout;
    hlayout_1->addLayout(glayout__0);
    hlayout_1->addLayout(vlayout__0);

    auto vlayout0 = new QVBoxLayout;
    vlayout0->addLayout(hlayout_0,1);
    vlayout0->addWidget(loadgraph,4);
    vlayout0->addLayout(hlayout_1,3);

    setLayout(vlayout0);
}

memPage::~memPage()
{
    delete buff;
}

void memPage::refresh()
{
    char temp[10];
    sprintf(temp,"%.1f GB",buff->getPhyTot()/1024/1024);
    tot->setText(temp);
    sprintf(temp,"%.1f GB",buff->getPhyUsed()/1024/1024);
    used->setText(temp);
    sprintf(temp,"%.1f GB",buff->getPhyAvail()/1024/1024);
    avail->setText(temp);
    sprintf(temp,"%.1f GB",buff->getCommTot()/1024/1024);
    comm->setText(temp);
    sprintf(temp,"%d MB",(int)buff->getSysCache()/1024);
    cached->setText(temp);
    sprintf(temp,"%d MB",(int)buff->getPaged()/1024);
    paged->setText(temp);
    sprintf(temp,"%d MB",(int)buff->getNonpaged()/1024);
    nonpaged->setText(temp);

    sprintf(temp,"%lu MHz",buff->getMemSpeed());
    spd->setText(temp);
    sprintf(temp,"%ud/%ud",buff->getMemUsedSlot(),buff->getMemTotSlot());
    slot->setText(temp);
    shape->setText(QString::fromStdString(buff->getMemShape()));
    sprintf(temp,"%lu MB",buff->getMemReserved()/1024);
    reserv->setText(temp);

    loadgraph->refresh(buff->getMemLoad());
}
