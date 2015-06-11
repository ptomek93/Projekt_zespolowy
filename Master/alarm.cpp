#include "alarm.h"
#include "ui_alarm.h"

Alarm::Alarm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Alarm)
{
    ui->setupUi(this);
}

Alarm::~Alarm()
{
    delete ui;
}
