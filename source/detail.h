#ifndef DETAIL_H
#define DETAIL_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVariant>
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QVBoxLayout>

#include "win32_common.h"
#include "gui_common.h"

class detail : public QWidget
{
    Q_OBJECT

public:
    detail(win32_common *buffer,QWidget *parent = nullptr);
    ~detail();
    void refresh();
    void iconize(bool x);

private:
    win32_common *buff;

    QVBoxLayout *vlayout;
    QTableWidget *table;
    QListWidget *iconList;
    int cur_row;
    bool SectionClicked;
    bool order[6];
    int recentSec;
    bool iconized;
    std::map<std::string,QIcon> path_icon;
    std::map<int,unsigned long> ptr_pid;

    void contextMenuEvent(QContextMenuEvent *event) override;
    QAction *terminateAc;
    void initListView();
    void initIconView();
    void refreshListView();
    void refreshIconView();
    QIcon getIcon(std::string path);

private slots:
    void do_table_pressed(int row);
    void do_table_sort(int index);
    void do_icon_pressed(QListWidgetItem *item);
    void do_terminate();
};

inline QIcon detail::getIcon(std::string path)
{
    if (path_icon.find(path) == path_icon.end()) {
        QIcon icon = getIconFromPath(path);
        path_icon[path] = icon;
        return icon;
    }
    else
        return path_icon[path];
}

#endif // DETAIL_H
