#include "ui/MainWindow/ui_MainWindow.h"
#include <include/MainWindow.hpp>
#include <QtCharts>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}
