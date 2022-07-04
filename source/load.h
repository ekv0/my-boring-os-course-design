#ifndef LOAD_H
#define LOAD_H

#include <QWidget>
#include <QHBoxLayout>
#include <QChartView>
#include <QChart>
#include <QtCharts/QtCharts>
#include <QSplineSeries>
#include <QVector>
#include <QValueAxis>
#include <QPointF>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QGraphicsView>
#include <QRect>
#include <QRectF>
#include <QPainter>
#include <QPen>

namespace Ui {
class load;
}

class load : public QWidget
{
    Q_OBJECT

public:
    explicit load(bool hasMesh,QWidget *parent = nullptr);
    ~load();

    void refresh(double load);
    void setXtick(unsigned cnt);
    void setYtick(unsigned cnt);
    void setSeriesPen(QPen pen);

private:
    Ui::load *ui;

    QChart *chart;
    QSplineSeries *series;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    QHBoxLayout *hlayout;
};

#endif // LOAD_H
