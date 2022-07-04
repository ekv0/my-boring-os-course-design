#include "taskmgr.h"
#include "ui_taskmgr.h"

Taskmgr::Taskmgr(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Taskmgr)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Taskmgr::do_refresh);
    timer->start(1000);

    onTop = false;
    central = new QWidget;
    setCentralWidget(central);

    layout = new QHBoxLayout;
    central->setLayout(layout);
    layout->setSpacing(5);
    layout->setContentsMargins(5,5,5,5);

    //allocation
    menuBar = new QMenuBar(this);
    fileMenu = new QMenu("文件");
    optionMenu = new QMenu("选项");
    checkMenu = new QMenu("查看");
    shutMenu = new QMenu("关机");
    helpMenu = new QMenu("帮助");
    rateMenu = new QMenu("更新速度");
    viewMenu = new QMenu("视图");

    newProcAc = new QAction("运行新任务",this);
    exitAc = new QAction("退出",this);
    onTopAc = new QAction("置于顶层",this);
    refreshAc = new QAction("立即刷新",this);
    highRateAc = new QAction("高",this);
    normRateAc = new QAction("正常",this);
    lowRateAc = new QAction("低",this);
    pauseRefreshAc = new QAction("已暂停",this);
    iconViewAc = new QAction("小图标",this);
    listViewAc = new QAction("详细列表",this);
    shutAc = new QAction("关机",this);
    logOffAc = new QAction("注销",this);
    infoAc = new QAction("关于",this);
    qtInfoAc = new QAction("关于Qt",this);
    nameAc = new QAction("组长：王凌峰",this);
    rateGrp = new QActionGroup(this);
    viewGrp = new QActionGroup(this);

    //connect signal & slot
    connect(newProcAc,&QAction::triggered,this,&Taskmgr::do_newProc);
    connect(exitAc,&QAction::triggered,this,&Taskmgr::do_Exit);
    connect(onTopAc,&QAction::triggered,this,&Taskmgr::do_onTop);
    connect(refreshAc,&QAction::triggered,this,&Taskmgr::do_refresh);
    connect(highRateAc,&QAction::triggered,this,&Taskmgr::do_highRate);
    connect(normRateAc,&QAction::triggered,this,&Taskmgr::do_normRate);
    connect(lowRateAc,&QAction::triggered,this,&Taskmgr::do_lowRate);
    connect(pauseRefreshAc,&QAction::triggered,this,&Taskmgr::do_pauseRefresh);
    connect(iconViewAc,&QAction::triggered,this,&Taskmgr::do_iconView);
    connect(listViewAc,&QAction::triggered,this,&Taskmgr::do_listView);
    connect(shutAc,&QAction::triggered,this,&Taskmgr::do_shutdown);
    connect(logOffAc,&QAction::triggered,this,&Taskmgr::do_logOff);
    connect(nameAc,&QAction::triggered,this,&Taskmgr::do_info);
    connect(infoAc,&QAction::triggered,this,&Taskmgr::do_info);
    connect(qtInfoAc,&QAction::triggered,this,&Taskmgr::do_qtInfo);

    //set up action group
    rateGrp->addAction(highRateAc);
    rateGrp->addAction(normRateAc);
    rateGrp->addAction(lowRateAc);
    rateGrp->addAction(pauseRefreshAc);
    normRateAc->setChecked(true);

    rateGrp->addAction(iconViewAc);
    rateGrp->addAction(listViewAc);
    listViewAc->setChecked(true);

    //set up menu
    menuBar->addMenu(fileMenu);
    fileMenu->addAction(newProcAc);
    fileMenu->addAction(exitAc);

    menuBar->addMenu(optionMenu);
    optionMenu->addAction(onTopAc);

    menuBar->addMenu(checkMenu);
    checkMenu->addAction(refreshAc);
    checkMenu->addMenu(rateMenu);

    rateMenu->addAction(highRateAc);
    rateMenu->addAction(normRateAc);
    rateMenu->addAction(lowRateAc);
    rateMenu->addAction(pauseRefreshAc);

    checkMenu->addMenu(viewMenu);
    viewMenu->addAction(iconViewAc);
    viewMenu->addAction(listViewAc);

    menuBar->addMenu(shutMenu);
    shutMenu->addAction(shutAc);
    shutMenu->addAction(logOffAc);

    menuBar->addMenu(helpMenu);
    helpMenu->addAction(nameAc);
    helpMenu->addAction(infoAc);
    helpMenu->addAction(qtInfoAc);

    setMenuBar(menuBar);

    buff = new win32_common;    //公共buffer
    tab = new QTabWidget;
    perfTab = new performance(buff);
    detailTab = new detail(buff);
    userTab = new user(buff);

    tab->addTab(perfTab,"性能");
    tab->addTab(userTab,"用户");
    tab->addTab(detailTab,"详细信息");
    layout->addWidget(tab);

    setWindowTitle("任务管理器");
    setMinimumSize(600,600);
    resize(700,700);
}

Taskmgr::~Taskmgr()
{
    delete timer;
    delete buff;
    delete central;

    delete ui;
}

void Taskmgr::do_newProc()
{
    bool ok;
    QString path = QInputDialog::getText(this,"新建任务",
            "Windows将根据你所键入的名称，为你打开相应的程序、文件夹或文档。"
            "\n打开：",
            QLineEdit::Normal,nullptr,&ok);
    if (ok) {
        if (path.isEmpty()){
            printError("运行");
            return;
        }

        std::string prog,param;
        parsePath(path.toStdString(),prog,param);
        if (!win32_common::openProc(prog.c_str(),param.c_str(),false)) {
            printError("运行");
        }
    }
}

void Taskmgr::do_Exit()
{
    exit(0);
}

void Taskmgr::do_onTop()
{
    if (onTop = !onTop)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    show();
}

void Taskmgr::do_refresh()
{
    buff->refresh();
    perfTab->refresh();
    detailTab->refresh();
    userTab->refresh();
}

void Taskmgr::do_highRate()
{
    if (timer->isActive())
        timer->setInterval(500);
    else
        timer->start(500);
}

void Taskmgr::do_normRate()
{
    if (timer->isActive())
        timer->setInterval(1000);
    else
        timer->start(1000);
}

void Taskmgr::do_lowRate()
{
    if (timer->isActive())
        timer->setInterval(2000);
    else
        timer->start(2000);
}

void Taskmgr::do_pauseRefresh()
{
    timer->stop();
}

void Taskmgr::do_iconView()
{
    detailTab->iconize(true);
}

void Taskmgr::do_listView()
{
    detailTab->iconize(false);
}

void Taskmgr::do_shutdown()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("任务管理器");
    msgBox.setText("<font size=\"5\" color=\"#003399\">你确定要关机吗？</font>");
    msgBox.setInformativeText("如果现在关机，你和其它正在使用这台电脑的人都可能丢失尚未保存的工作。");
    auto conti = msgBox.addButton("关机", QMessageBox::YesRole);
    msgBox.addButton("取消",QMessageBox::NoRole);
    msgBox.exec();

    if (msgBox.clickedButton() == static_cast<QAbstractButton *>(conti))
        if (!win32_common::shutdown())
           printError("关机");
}

void Taskmgr::do_logOff()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("任务管理器");
    msgBox.setText("<font size=\"5\" color=\"#003399\">你确定要注销吗？</font>");
    auto conti = msgBox.addButton("注销", QMessageBox::YesRole);
    msgBox.addButton("取消",QMessageBox::NoRole);
    msgBox.exec();

    if (msgBox.clickedButton() == static_cast<QAbstractButton *>(conti))
        if (!win32_common::logoff())
            printError("注销");
}

void Taskmgr::do_info()
{
    QString title("information");
    QString content("基于Win32 API与Qt 6.2.2\ncomplys to GPLv3\nCUMTB 2021秋OS课设\n联系邮箱 ekv0ogdj@126.com");
    QMessageBox::information(this,title,content,QMessageBox::Ok,QMessageBox::NoButton);
}

void Taskmgr::do_qtInfo()
{
    QMessageBox::aboutQt(this);
}

void Taskmgr::parsePath(std::string path,std::string &prog,std::string &param)
{
    bool inquote = false;
    bool in = false;
    std::string::size_type beg,pos;
    for (pos = 0; pos < path.size(); ++pos) {
        if (!inquote && path[pos] != ' ') {
            if (path[pos] == '\"'){
                beg = pos+1;
                inquote = true;
            }
            else if (!in){
                in = true;
                beg = pos;
            }
        }
        else if (inquote) {
            if (path[pos] == '\"'){
                prog = path.substr(beg,pos-beg);
                break;
            }
        }
        else if (path[pos] == ' ') {
            if (in) {
                prog = path.substr(beg,pos-beg);
                break;
            }
        }
    }
    if (pos == path.size()) {
        prog = path.substr(beg,pos-beg);
    }
    if (inquote)
        param = path.substr(pos+1);
    else
        param = path.substr(pos);
    trim(prog);
    trim(param);
}

void Taskmgr::printError(std::string text)
{
    QMessageBox::information(this,
                            "任务管理器",
                            QString::fromStdString(text) + "失败：错误代码" + QString::number(GetLastError()),
                            QMessageBox::Ok,
                            QMessageBox::NoButton);
}
