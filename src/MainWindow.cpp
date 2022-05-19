#include "ui/MainWindow/ui_MainWindow.h"
#include <include/MainWindow.hpp>
#include <QtCharts>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    QThread* threadSerial = new QThread();
    m_spwWorker = new SerialPortWorker(m_qqSerialBufferQueue);
    m_spwWorker->moveToThread(threadSerial);
    ui->setupUi(this);
    setupGuiComponents();

    connect(m_spwWorker, SIGNAL(workRequested()), threadSerial, SLOT(start()));
    connect(threadSerial, SIGNAL(started()), m_spwWorker, SLOT(doWork()));
    connect(m_spwWorker, SIGNAL(finished()), threadSerial, SLOT(quit()), Qt::ConnectionType::DirectConnection);

    m_spwWorker->abortWork();
    threadSerial->wait();
    m_spwWorker->requestWork();
}

MainWindow::~MainWindow() {
    delete ui;
    m_spwWorker->abortWork();
    m_spwWorker->disconnect();
    delete m_spwWorker;
}

void MainWindow::setupGuiComponents() {
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        ui->cbSerialPorts->addItem(port.portName());
    }

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

void MainWindow::on_pbSerialCD_clicked() {
    if(ui->pbSerialCD->text().contains("Connect")) {
        if(m_spwWorker->connect(ui->cbSerialPorts->currentText()) < 0) {
            QMessageBox mbox;
            mbox.setText("Error opening serial port!");
            ui->lblStatus->setText("Disconnected");
            ui->lblStatus->setStyleSheet("QLabel { color: red; }");
            mbox.exec();
        } else {
            ui->lblStatus->setText("Connected");
            ui->lblStatus->setStyleSheet("QLabel { color: green; }");
            ui->pbSerialCD->setText("Disconnect");
        }
    } else {
        m_spwWorker->abortWork();
        if(m_spwWorker->disconnect() >= 0) {
            ui->lblStatus->setText("Disconnected");
            ui->lblStatus->setStyleSheet("QLabel { color: red; }");
            ui->pbSerialCD->setText("Connect");
        }
    }
}


void MainWindow::on_pbCapture_clicked() {
    try {
        m_spwWorker->doWork();
    } catch(SerialErrorCodes code) {
        QMessageBox mbox;
        switch(code) {
            case SerialErrorCodes::UNDEFINED_SERIAL_PORT:
                mbox.setText("Please connect to a serial port first!");
            break;
        }

        mbox.exec();
    }
}

