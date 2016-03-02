#ifndef WIDGET_H
#define WIDGET_H

#define MAX_CHANNEL 13

#include <QWidget>
#include <QTreeWidgetItem>
#include <QThread>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QFileDialog>

#include "scanner.h"
#include "devicedialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();
private:
    Ui::Widget *ui;
    int currentChannel;
    Scanner scanner;
    QThread scannerThread;
    QTimer timer;
    QThread timerThread;

public slots:
    void setItem(Info);
    void changeChannel();
};

#endif // WIDGET_H
