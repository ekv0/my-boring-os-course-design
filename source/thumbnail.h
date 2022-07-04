#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QLabel>

#include "win32_common.h"
#include "load.h"

class thumbnail : public QWidget
{
    Q_OBJECT
public:
    enum TYPE {CpuPage,MemPage};

    explicit thumbnail(win32_common *buffer,TYPE t,QWidget *parent = nullptr);
    ~thumbnail();

    void refresh();
    void setSeriesPen(QPen pen);

private:
    TYPE type;
    win32_common *buff;
    load *loadgraph;

    QLabel *cpulabel;
    QLabel *memlabel;
};

#endif // THUMBNAIL_H
