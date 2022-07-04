#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QPoint>
#include <QMouseEvent>
#include <QPalette>

#include "cpupage.h"
#include "mempage.h"
#include "thumbnail.h"
#include "performance.h"
#include "win32_common.h"

class performance : public QWidget
{
    Q_OBJECT

public:
    performance(win32_common *buff,QWidget *parent = nullptr);
    ~performance();
    void refresh();

private:
    cpuPage *cpu;
    memPage *mem;
    thumbnail *cputhumbn;
    thumbnail *memthumbn;
    QHBoxLayout *hlayout;
    QVBoxLayout *vlayout;
    QStackedLayout *slayout;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    QPoint mousePos;

public slots:
    void mouseClicked();

};

#endif // PERFORMANCE_H
