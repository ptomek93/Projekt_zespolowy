#ifndef SWIATLO_H
#define SWIATLO_H

#include <QWidget>

namespace Ui {
class Swiatlo;
}

class Swiatlo : public QWidget
{
    Q_OBJECT

public:
    explicit Swiatlo(QWidget *parent = 0);
    ~Swiatlo();

private:
    Ui::Swiatlo *ui;

signals:
    void WyslijRamke(unsigned char adres, unsigned char funkcja, unsigned short addr_pocz, short value);
};

#endif // SWIATLO_H
