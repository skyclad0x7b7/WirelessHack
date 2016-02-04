#ifndef WIDGET_H
#define WIDGET_H

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

    Scanner scanner;
    QThread scannerThread;




};

#endif // WIDGET_H
