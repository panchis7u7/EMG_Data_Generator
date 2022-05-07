#include "ui/MainWindow/ui_MainWindow.h"
#include <include/MainWindow.hpp>
#include <QtCharts>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //define the upper and lower boundary of the area.
    QLineSeries* series0 = new QLineSeries();
    QLineSeries* series1 = new QLineSeries();

    *series0 << QPointF(1, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7) << QPointF(12, 6)
                 << QPointF(16, 7) << QPointF(18, 5);
    *series1 << QPointF(1, 3) << QPointF(3, 4) << QPointF(7, 3) << QPointF(8, 2) << QPointF(12, 3)
                 << QPointF(16, 4) << QPointF(18, 3);

    QAreaSeries* series = new QAreaSeries(series0, series1);
    series->setName("Prueba");
    QPen pen(0x059605);
    pen.setWidth(3);
    series->setPen(pen);

    QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
    gradient.setColorAt(0.0, 0x3cc63c);
    gradient.setColorAt(1.0, 0x26f626);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    series->setBrush(gradient);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple areachart example");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 20);
    chart->axes(Qt::Vertical).first()->setRange(0, 10);

    ui->gvChart->setChart(chart);
    ui->gvChart->setRenderHint(QPainter::Antialiasing);

    QStandardItemModel* model = new QStandardItemModel();
    QStringList horizontalHeader {"Pulgar", "Indice", "Medio", "Anular", "Menique"};

    model->setHorizontalHeaderLabels(horizontalHeader);


    ui->tvData->setModel(model);
    ui->tvData->verticalHeader()->setVisible(false);
    ui->tvData->verticalHeader()->setDefaultSectionSize(10);
    ui->tvData->verticalHeader()->setDefaultAlignment(Qt::AlignVCenter);
    ui->tvData->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->tvData->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->tvData->setShowGrid(false);

    QList<QStandardItem*> list;
    for(int i = 0; i < 5; ++i) {
        QStandardItem* item = new QStandardItem("0.45");
        item->setTextAlignment(Qt::AlignmentFlag::AlignCenter);
        list << item;
    }

    model->appendRow(list);
}

MainWindow::~MainWindow() {
    delete ui;
}
