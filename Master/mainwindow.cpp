#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    modbus = new Modbus;
    swiatlo= new Swiatlo;
    zaluzje= new Zaluzje;
    alarm=   new Alarm;

    form = new Form;

    form->show();

    connect(form,SIGNAL(akcja()),modbus,SLOT(openPort()));
    connect(alarm,SIGNAL(WyślijRamke(unsigned char,unsigned char,unsigned short,short)),
            modbus,SLOT(recive_data_from_widget(unsigned char,unsigned char,unsigned short,unsigned short)));
    connect(zaluzje,SIGNAL(WyślijRamke(unsigned char,unsigned char,unsigned short,short)),
            modbus,SLOT(recive_data_from_widget(unsigned char,unsigned char,unsigned short,unsigned short)));
    connect(swiatlo,SIGNAL(WyślijRamke(unsigned char,unsigned char,unsigned short,short)),
            modbus,SLOT(recive_data_from_widget(unsigned char,unsigned char,unsigned short,unsigned short)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modbus;
    delete swiatlo;
    delete zaluzje;
    delete alarm;
}

void MainWindow::on_pbConnect_clicked()
{
   modbus->openPort();

}

