#ifndef ZALUZJE_H
#define ZALUZJE_H

#include <QWidget>

namespace Ui {
class Zaluzje;
}

class Zaluzje : public QWidget
{
    Q_OBJECT

public:
    explicit Zaluzje(QWidget *parent = 0);
    ~Zaluzje();

private:
    Ui::Zaluzje *ui;


signals:
    void WyslijRamke(unsigned char adres, unsigned char funkcja, unsigned short addr_pocz, short value);

};

#endif // ZALUZJE_H
