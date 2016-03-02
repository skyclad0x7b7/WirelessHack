#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    currentChannel = 0;
    QObject::connect(&scannerThread, SIGNAL(started()), &scanner, SLOT(doStart()));
    QObject::connect(&scanner, SIGNAL(captured(Info)), this, SLOT(setItem(Info)), Qt::BlockingQueuedConnection);
    QObject::connect(&timerThread, SIGNAL(started()), &timer, SLOT(start()));
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(changeChannel()));
    QObject::connect(&timerThread, SIGNAL(finished()), &timer, SLOT(stop()));
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

    timer.setInterval(2000);
    timer.moveToThread(&timerThread);
    timerThread.start();
}

void Widget::setItem(Info info){
    QList<QTreeWidgetItem*> listItem = ui->treeWidget->findItems(info.BSSID, Qt::MatchWildcard, SCANNER_COLUMN_BSSID);

    if(listItem.count() == 0) { // New AP
        cout << "[*] New Item" << endl;
        cout << qPrintable(info.AP) << " : " << qPrintable(info.BSSID) << " ENC : " << qPrintable(info.ENC) << endl;
        QTreeWidgetItem* newItem = new QTreeWidgetItem(ui->treeWidget);
        newItem->setText(SCANNER_COLUMN_AP, info.AP);
        newItem->setText(SCANNER_COLUMN_BSSID, info.BSSID);
        newItem->setText(SCANNER_COLUMN_ENCRYPT, info.ENC);
        newItem->setText(SCANNER_COLUMN_CHANNEL, QString::number((currentChannel % MAX_CHANNEL) + 1));

    }
    else if (listItem.count() == 1){
        cout << "[*] Add Item" << endl;
        QTreeWidgetItem* currentItem = listItem[0];
        //currentItem->setText(SCANNER_COLUMN_SIGNAL, QString::number(info.Signal));  // signal update

        QTreeWidgetItem *subItem;
        listItem.clear();
        QList<QTreeWidgetItem*> listItem = ui->treeWidget->findItems(info.BSSID, Qt::MatchExactly | Qt::MatchRecursive, SCANNER_COLUMN_BSSID);
        if (listItem.count() == 0) {
            subItem = new QTreeWidgetItem();
            subItem->setText(SCANNER_COLUMN_AP, "station");
            subItem->setText(SCANNER_COLUMN_BSSID, info.BSSID);
            currentItem->setText(SCANNER_COLUMN_STA_COUNT, QString::number(currentItem->childCount()));
        }
        else if(listItem.count() == 1){
            subItem = listItem[0];
            int datas = subItem->text(SCANNER_COLUMN_DATAS).toInt();
            subItem->setText(SCANNER_COLUMN_DATAS, QString::number(1));
            subItem->setText(SCANNER_COLUMN_DATAS, QString::number(++datas));
        }
        int totalDatas = currentItem->text(SCANNER_COLUMN_DATAS).toInt();
        currentItem->setText(SCANNER_COLUMN_DATAS, QString::number(++totalDatas));

    }
    else cout << "Saved one" << endl;
}

void Widget::changeChannel(){
    cout << "[*] Channel Changed : " << (currentChannel % MAX_CHANNEL) + 1 << endl;
    ui->channelLabel->setText(QString("Channel : %1").arg((++currentChannel) % MAX_CHANNEL + 1));
    QString info = QString("iwconfig %1 channel %2").arg(QString("wlan1")).arg(QString::number((currentChannel % MAX_CHANNEL) + 1));
    system(info.toStdString().c_str());
}
