#ifndef TASKMGR_H
#define TASKMGR_H

#include "performance.h"
#include "detail.h"
#include "user.h"
#include "win32_common.h"

#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QMessageBox>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QTimer>
#include <QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Taskmgr; }
QT_END_NAMESPACE

class Taskmgr : public QMainWindow
{
    Q_OBJECT

public:
    Taskmgr(QWidget *parent = nullptr);
    ~Taskmgr();

private:
    Ui::Taskmgr *ui;

    void parsePath(std::string path,const char **prog,const char **param);
    void printError(std::string text);
    void parsePath(std::string path,std::string &prog,std::string &param);
    bool onTop;

    QWidget *central;
    QHBoxLayout *layout;

    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *checkMenu;
    QMenu *shutMenu;
    QMenu *helpMenu;
    QMenu *rateMenu;
    QMenu *viewMenu;

    QAction *newProcAc;
    QAction *exitAc;
    QAction *onTopAc;
    QAction *refreshAc;
    QAction *highRateAc;
    QAction *normRateAc;
    QAction *lowRateAc;
    QAction *pauseRefreshAc;
    QAction *iconViewAc;
    QAction *listViewAc;
    QAction *shutAc;
    QAction *logOffAc;
    QAction *infoAc;
    QAction *qtInfoAc;
    QAction *nameAc;
    QActionGroup *rateGrp;
    QActionGroup *viewGrp;

    QTabWidget *tab;
    performance *perfTab;
    detail *detailTab;
    user *userTab;

    QTimer *timer;

    win32_common *buff;

private slots:
    void do_newProc();
    void do_Exit();
    void do_onTop();
    void do_refresh();
    void do_highRate();
    void do_normRate();
    void do_lowRate();
    void do_pauseRefresh();
    void do_iconView();
    void do_listView();
    void do_shutdown();
    void do_logOff();
    void do_info();
    void do_qtInfo();
};

#endif // TASKMGR_H
