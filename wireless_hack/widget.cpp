#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->sc = new Scanner("wlan0");
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sc;
}

void MainWindow::startScan(){

}

void MainWindow::on_pushButton_clicked()
{
    MainWindow::sc->startScan();

}

void MainWindow::on_pushButton_2_clicked()
{
    MainWindow::sc->stopScan();
}
