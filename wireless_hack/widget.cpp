#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QObject::connect(&scannerThread, SIGNAL(started()), &scanner, SLOT(doStart()));
    QObject::connect(&scanner, SIGNAL(captured(Info)), this, SLOT(tempFunc(Info)), Qt::BlockingQueuedConnection);
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

void Widget::tempFunc(Info info){
    QList<QTreeWidgetItem*> listItem = ui->treeWidget->findItems(info.BSSID, Qt::MatchWildcard, SCANNER_COLUMN_BSSID);

    if(listItem.count() == 0) { // New AP
        cout << "New Item" << endl;
        QTreeWidgetItem* newItem = new QTreeWidgetItem(ui->treeWidget);
        newItem->setText(SCANNER_COLUMN_AP, info.AP);
        newItem->setText(SCANNER_COLUMN_BSSID, info.BSSID);
        newItem->setText(SCANNER_COLUMN_ENCRYPT, info.ENC);
    }


    else cout << "Saved one" << endl;
}
