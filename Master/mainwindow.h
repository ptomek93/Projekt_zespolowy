#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "modbus.h"
#include "form.h"
#include "swiatlo.h"
#include "zaluzje.h"
#include "alarm.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbConnect_clicked();

private:
    Ui::MainWindow *ui;
     Modbus     *modbus;  //MARIUSZ
     Zaluzje    *zaluzje; //TOMEK
     Swiatlo    *swiatlo; //DANIEL
     Alarm      *alarm;   //MARIUSZ
     Form *form; //DO WYJEBANIA


};

#endif // MAINWINDOW_H
