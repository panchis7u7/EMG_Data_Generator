#include "ui/MainWindow/ui_MainWindow.h"
#include <include/MainWindow.hpp>
#include <QtCharts>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QBarSet* val = new QBarSet("Digital Value.");
    *val << 20 << 255 << 768 << 100 << 998;

    QBarSeries* series = new QBarSeries();
    series->append(val);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Finger read value.");
    chart->setAnimationOptions(QChart::AnimationOption::SeriesAnimations);

    QStringList categories{"Thumb", "Index", "Middle", "Ring", "Little"};
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignmentFlag::AlignBottom);
    series->attachAxis(axisX);
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 1024);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->gvChart->setChart(chart);
    ui->gvChart->setRenderHint(QPainter::Antialiasing);

    QStandardItemModel* model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(categories);

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
