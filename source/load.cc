#include "load.h"
#include "ui_load.h"

load::load(bool hasMesh,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::load)
{
    ui->setupUi(this);

    chart = new QChart;
    series = new QSplineSeries;

    chart->addSeries(series);
    axisX = new QDateTimeAxis;
    axisY = new QValueAxis;

    axisX->setLineVisible(hasMesh);
    axisX->setGridLineVisible(hasMesh);
    axisX->setMinorGridLineVisible(hasMesh);
    axisX->setLabelsVisible(false);
    axisX->setMin(QDateTime::currentDateTime().addSecs(-60));
    axisX->setMax(QDateTime::currentDateTime().addSecs(0));

    axisY->setLineVisible(hasMesh);
    axisY->setGridLineVisible(hasMesh);
    axisY->setMinorGridLineVisible(hasMesh);
    axisY->setLabelsVisible(false);
    axisY->setMin(0);
    axisY->setMax(100);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->setVisible(false);
    //减小空白处尺寸
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setMargins(QMargins(-52, -6, -47, -21));
    chart->setBackgroundRoundness(0);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    ui->graphicsView->setChart(chart);

    hlayout = new QHBoxLayout;
    hlayout->addWidget(ui->graphicsView);
    setLayout(hlayout);

    ui->graphicsView->show();
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

load::~load()
{
    delete series;
    delete chart;

    delete ui;
}

void load::refresh(double load)
{
    auto now = QDateTime::currentDateTime();
    axisX->setMin(now.addSecs(-60));
    axisX->setMax(now.addSecs(0));
    if (series->count() && (series->at(0).x() < now.addSecs(-60).toMSecsSinceEpoch()))
        series->remove(0);
    series->append(now.toMSecsSinceEpoch(),load);
}

void load::setXtick(unsigned cnt)
{
    axisX->setTickCount(cnt);
}

void load::setYtick(unsigned cnt)
{
    axisY->setTickCount(cnt);
}

void load::setSeriesPen(QPen pen)
{
    series->setPen(pen);
}
