#include "detail.h"
#include <iostream>

detail::detail(win32_common *buffer,QWidget *parent)
    : QWidget(parent),buff(buffer)
{
    iconized = false;
    terminateAc = new QAction("结束任务");
    connect(terminateAc,&QAction::triggered,this,&detail::do_terminate);

    vlayout = new QVBoxLayout;
    setLayout(vlayout);

    initListView();

    refresh();
}

detail::~detail()
{
    delete terminateAc;
}

void detail::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(terminateAc);
    menu.exec(event->globalPos());
}

void detail::refresh()
{
    if (!iconized)
        refreshListView();
    else
        refreshIconView();
}

void detail::initListView()
{
    SectionClicked = false;
    for (int i = 0; i < 6; ++i)
        order[i] = true;

    //initialize table
    table = new QTableWidget(this);
    vlayout->addWidget(table);

    table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    table->setColumnCount(6);

    //header设置
    table->verticalHeader()->setVisible(false);
    auto header = table->horizontalHeader();
    header->setDefaultAlignment(Qt::AlignLeft);
    table->setHorizontalHeaderLabels(
                QStringList() << "名称" << "PID" << "线程数" << "用户名"
                << "优先级" << "内存(活动的专用工作集)");
    header->resizeSection(0,180);
    header->resizeSection(1,60);
    header->resizeSection(2,60);
    header->resizeSection(3,80);
    header->resizeSection(4,80);
    header->resizeSection(5,110);

    header->setSectionsClickable(true);
    header->setSortIndicatorShown(true);
    //一次只可选中一整行
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置signal与slot
    connect(table->horizontalHeader(),&QHeaderView::sectionClicked,this,&detail::do_table_sort);

    connect(table,&QTableWidget::cellPressed,this,&detail::do_table_pressed);
}

void detail::initIconView()
{
    iconList = new QListWidget(this);
    vlayout->addWidget(iconList);

    iconList->setViewMode(QListView::IconMode); //图标化
    iconList->setMovement(QListView::Static);    //用户不可移动
    iconList->setSelectionMode(QAbstractItemView::SingleSelection); //单选

    connect(iconList,&QListWidget::itemPressed,this,&detail::do_icon_pressed);
}

void detail::refreshListView()
{
    const auto &pid_thrd = buff->getPid_Thrdcnt();
    auto cnt = pid_thrd.size();

    table->clearContents();
    table->setRowCount(cnt);

    unsigned i(0);
    for (auto beg = pid_thrd.cbegin(); beg != pid_thrd.cend(); ++beg, ++i) {
        std::string temp;
        buff->getProcName(beg->first,temp);
        auto item = new QTableWidgetItem(QString::fromStdString(temp));
        buff->getProcPath(beg->first,temp);
        QIcon icon = getIcon(temp);
        item->setIcon(icon);
        table->setItem(i,0,item);

        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,QVariant((unsigned long long)beg->first));
        table->setItem(i,1,item);

        unsigned long long ulltemp;
        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,QVariant(beg->second));
        table->setItem(i,2,item);

        buff->getProcUserName(beg->first,temp);
        item = new QTableWidgetItem(QString::fromStdString(temp));
        table->setItem(i,3,item);

        buff->getProcCla(beg->first,temp);
        item = new QTableWidgetItem(QString::fromStdString(temp));
        table->setItem(i,4,item);

        buff->getProcMem(beg->first,ulltemp);
        item = new QTableWidgetItem;
        item->setData(Qt::DisplayRole,QVariant(ulltemp));
        table->setItem(i,5,item);

        table->setRowHeight(i,8);
    }

    //若点击过section则按规则重排序
    if (SectionClicked){
        if (order[recentSec])
            table->sortItems(recentSec,Qt::SortOrder::AscendingOrder);
        else
            table->sortItems(recentSec,Qt::SortOrder::DescendingOrder);
    }
}

void detail::refreshIconView()
{
    const auto &pid_thrd = buff->getPid_Thrdcnt();

    iconList->clear();
    ptr_pid.clear();

    for (auto beg = pid_thrd.cbegin(); beg != pid_thrd.cend(); ++beg) {
        std::string temp;
        buff->getProcName(beg->first,temp);
        auto item = new QListWidgetItem(QString::fromStdString(temp));
        buff->getProcPath(beg->first,temp);
        QIcon icon = getIcon(temp);
        item->setIcon(icon);
        iconList->addItem(item);

        //记录对应item的pid
        ptr_pid[iconList->row(item)] = beg->first;
    }
}

void detail::iconize(bool x)
{
    if (!iconized && x) {
        iconized = true;
        vlayout->removeWidget(table);
        delete table;
        initIconView();
        refresh();
    }
    else if (iconized && !x) {
        iconized = false;
        vlayout->removeWidget(iconList);
        delete iconList;
        initListView();
        refresh();
    }
}

void detail::do_table_pressed(int row)
{
    this->cur_row = row;
}

void detail::do_icon_pressed(QListWidgetItem *item)
{
    cur_row = iconList->row(item);
}

void detail::do_table_sort(int index)
{
    SectionClicked = true;  //点击过section
    recentSec = index;
    if (order[index] = !order[index])
        table->sortItems(index,Qt::SortOrder::AscendingOrder);
    else
        table->sortItems(index,Qt::SortOrder::DescendingOrder);
}

void detail::do_terminate()
{
    QString procName;
    if (!iconized)
        procName = table->item(cur_row,0)->text();
    else
        procName = iconList->item(cur_row)->text();

    QMessageBox msgBox;
    msgBox.setWindowTitle("任务管理器");
    msgBox.setText("<font size=\"5\" color=\"#003399\">你希望结束 " + procName + " 吗？</font>");
    msgBox.setInformativeText("如果某个打开的程序与此进程关联，则会关闭此程序并且"
                               "将丢失所有未保存的数据。如果结束某个系统进程，则可能导致"
                               "系统不稳定。你确定要继续吗？");
    auto ack = msgBox.addButton("结束进程", QMessageBox::YesRole);
    msgBox.addButton("取消",QMessageBox::NoRole);
    msgBox.exec();

    if (msgBox.clickedButton() == static_cast<QAbstractButton *>(ack)) {
        unsigned long selectedPid;
        if (!iconized)
            selectedPid = table->item(cur_row,1)->text().toULong();
        else
            selectedPid = ptr_pid[cur_row];
        buff->killProc(selectedPid);
        refresh();
    }
}
