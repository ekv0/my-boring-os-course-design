#ifndef GUI_COMMON_H
#define GUI_COMMON_H

#include <QLabel>
#include <QFont>
#include <QString>
#include <QFileInfo>
#include <QIcon>
#include <QPixmap>
#include <QFileIconProvider>
#include <QFileSystemModel>

void trim(std::string &str);

//方便的标签设置函数
QLabel *infolabel(const QString &str,QWidget *parent);
QLabel *infolabel(const char *str,QWidget *parent);
QLabel *do_infolabel(QString qstr,QWidget *parent);
QLabel *vallabel(const QString &str,QWidget *parent);
QLabel *vallabel(const char *str,QWidget *parent);
void do_vallabel(QLabel *label);


inline void trim(std::string &str)
{
    str.erase(0,str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
}

//抽取图标
QIcon getIconFromPath(std::string path);

inline QLabel *infolabel(const QString &str,QWidget *parent)
{
    return do_infolabel(str,parent);
}

inline QLabel *infolabel(const char *str,QWidget *parent)
{
    return do_infolabel(QString(str),parent);
}

inline QLabel *do_infolabel(QString qstr,QWidget *parent)
{
    QString str("<font color=\"#707070\">");
    str += qstr;
    str += "</font>";
    auto label = new QLabel(str,parent);

    QFont font;
    font.setPointSize(9);
    label->setFont(font);

    return label;
}

inline QLabel *vallabel(const QString &str,QWidget *parent)
{
    auto label = new QLabel(str,parent);
    do_vallabel(label);
    return label;
}

inline QLabel *vallabel(const char *str,QWidget *parent)
{
    auto label = new QLabel(str,parent);
    do_vallabel(label);
    return label;
}

inline void do_vallabel(QLabel *label)
{
    QFont font;
    font.setPointSize(15);
    label->setFont(font);
}

#endif // GUI_COMMON_H
