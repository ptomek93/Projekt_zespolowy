#include "zaluzje.h"
#include "ui_zaluzje.h"

Zaluzje::Zaluzje(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Zaluzje)
{
    ui->setupUi(this);
}

Zaluzje::~Zaluzje()
{
    delete ui;
}
