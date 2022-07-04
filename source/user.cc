#include "user.h"
#include <iostream>

user::user(win32_common *buffer,QWidget *parent)
    : QWidget(parent),buff(buffer)
{
    //获取账号数量
    const auto &pid_thrd = buff->getPid_Thrdcnt();
    for (const auto &p : pid_thrd) {
        std::string username;
        buff->getProcUserName(p.first,username);
        user_pids[username].push_back(p.first);
    }
    for (decltype(user_pids.size()) i = 0; i < user_pids.size(); ++i)
        expanded.push_back(false);

    tree = new QTreeWidget(this);
    tree->setHeaderLabel("用户");

    layout = new QHBoxLayout;
    layout->addWidget(tree);
    setLayout(layout);

    refresh();

    connect(tree,&QTreeWidget::itemExpanded,this,&user::do_expanded);
    connect(tree,&QTreeWidget::itemCollapsed,this,&user::do_collapsed);
}

user::~user()
{
    delete tree;
}

void user::refresh()
{
    user_pids.clear();
    const auto &pid_thrd = buff->getPid_Thrdcnt();
    for (const auto &p : pid_thrd) {
        std::string username;
        buff->getProcUserName(p.first,username);
        user_pids[username].push_back(p.first);
    }

    if (user_pids.size() != expanded.size()) {
        expanded.clear();
        for (decltype(user_pids.size()) i = 0; i < user_pids.size(); ++i)
            expanded.push_back(false);
    }

    tree->clear();
    int i = 0;
    for (const auto &map : user_pids) {
        auto topItem = new QTreeWidgetItem(tree);
        topItem->setText(0,QString::fromStdString(map.first) +
                         QString(" (") +
                         QString::number(map.second.size()) +
                         QString(")"));
        for (const auto &pid : map.second) {
            auto node = new QTreeWidgetItem(topItem);
            std::string procName;
            buff->getProcName(pid,procName);
            node->setText(0,QString::fromStdString(procName));
            //设置图标
            std::string temp;
            buff->getProcPath(pid,temp);
            QIcon icon = getIcon(temp);
            node->setIcon(0,icon);
        }
        //展开/收起
        if (expanded[i])
            tree->expandItem(topItem);
        ++i;
    }
}

void user::do_expanded(QTreeWidgetItem *item)
{
    expanded[tree->indexOfTopLevelItem(item)] = true;
}

void user::do_collapsed(QTreeWidgetItem *item)
{
    expanded[tree->indexOfTopLevelItem(item)] = false;
}
