#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QAction>
#include <QMenu>
#include <QHBoxLayout>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QPushButton>

#include "win32_common.h"
#include "gui_common.h"

class user : public QWidget
{
    Q_OBJECT

public:
    user(win32_common *buffer,QWidget *parent = nullptr);
    ~user();

    void refresh();

private:
    win32_common *buff;
    std::map<std::string,std::vector<unsigned long>> user_pids;
    QTreeWidget *tree;
    QHBoxLayout *layout;

    std::vector<bool> expanded;
    std::map<std::string,QIcon> path_icon;

    QIcon getIcon(std::string path);

private slots:
    void do_expanded(QTreeWidgetItem *item);
    void do_collapsed(QTreeWidgetItem *item);
};

inline QIcon user::getIcon(std::string path)
{
    if (path_icon.find(path) == path_icon.end()) {
        QIcon icon = getIconFromPath(path);
        path_icon[path] = icon;
        return icon;
    }
    else
        return path_icon[path];
}

#endif // USER_H
