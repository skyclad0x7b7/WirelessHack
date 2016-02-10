#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    currentChannel = 0;
    QObject::connect(&scannerThread, SIGNAL(started()), &scanner, SLOT(doStart()));
    QObject::connect(&scanner, SIGNAL(captured(Info)), this, SLOT(tempFunc(Info)), Qt::BlockingQueuedConnection);
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

void Widget::tempFunc(Info info){
    QList<QTreeWidgetItem*> listItem = ui->treeWidget->findItems(info.BSSID, Qt::MatchWildcard, SCANNER_COLUMN_BSSID);

    if(listItem.count() == 0) { // New AP

        if(  info.BSSID.contains(QString("01:00:5E"))
                            || info.BSSID.contains(QString("33:33:FF"))
                            || info.BSSID.contains(QString("33:33:00"))
                            || info.BSSID.contains(QString("01:80:C2:00:00:00"))
                            || info.BSSID.contains(QString("00:00:00:00:00:00"))
                            || info.BSSID.contains(QString("FF:FF:FF:FF:FF:FF"))  )
                        return;
        cout << "[*] New Item" << endl;
        cout << qPrintable(info.AP) << " : " << qPrintable(info.BSSID) << " ENC : " << qPrintable(info.ENC) << endl;
        QTreeWidgetItem* newItem = new QTreeWidgetItem(ui->treeWidget);
        newItem->setText(SCANNER_COLUMN_AP, info.AP);
        newItem->setText(SCANNER_COLUMN_BSSID, info.BSSID);
        newItem->setText(SCANNER_COLUMN_ENCRYPT, info.ENC);
        newItem->setText(SCANNER_COLUMN_CHANNEL, QString::number((currentChannel % MAX_CHANNEL) + 1));

    }
    else cout << "Saved one" << endl;
}

void Widget::changeChannel(){
    cout << "[*] Channel Changed : " << (currentChannel % MAX_CHANNEL) + 1 << endl;
    ui->channelLabel->setText(QString("Channel : %1").arg((++currentChannel) % MAX_CHANNEL + 1));
    QString info = QString("iwconfig %1 channel %2").arg(QString("wlan0")).arg(QString::number((currentChannel % MAX_CHANNEL) + 1));
    system(info.toStdString().c_str());
}
