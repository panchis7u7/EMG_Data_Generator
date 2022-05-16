#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupGuiComponents();

private slots:
    void on_pbSerialCD_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
