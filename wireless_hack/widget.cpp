#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QObject::connect(&scannerThread, SIGNAL(started()), &scanner, SLOT(doStart()));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    DeviceDialog deviceDialog;
    deviceDialog.show();
    deviceDialog.exec();

    scanner.moveToThread(&scannerThread);
    scannerThread.start();
}
