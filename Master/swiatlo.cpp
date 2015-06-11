#include "swiatlo.h"
#include "ui_swiatlo.h"

Swiatlo::Swiatlo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Swiatlo)
{
    ui->setupUi(this);
}

Swiatlo::~Swiatlo()
{
    delete ui;
}
