#ifndef MODBUS_H
#define MODBUS_H

#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <windows.h>

#include <settingsdialog.h>
#include <QDebug>

struct Frame{
    char address;
    char function;
    unsigned short start_addr;
    short value;
    QByteArray data;
    unsigned short crc;

    Frame();
    Frame(char adres,char func,short addr_start, short val);
};

namespace Ui {
class Modbus;
}

class Modbus : public QWidget
{
    Q_OBJECT

public:
    explicit Modbus(QWidget *parent = 0);
    WORD CRC16 (const BYTE *nData, WORD wLength);
    unsigned int CRC16_2(unsigned char *buf, int len);
    ~Modbus();

public slots:
    bool openPort();
    bool statePort();
    void closePort();
    void showContent();
    void writeData(const QByteArray &data);
    void readData();
    void sendRequest(unsigned char slave_addres, unsigned char function, unsigned short start_addr, unsigned short value);
    void recive_data_from_widget(unsigned char slave_addres, unsigned char function, unsigned short start_addr, unsigned short value);

private slots:
    void processFrame();
    bool isFrameOk();



signals:
    void frameReady();


public:
    SettingsDialog *settings_dialog;//okienko konfiguracji portu COM
private:
    Ui::Modbus *ui;
    QSerialPort *port;//port COM
    SettingsDialog::Settings *port_settings;//informacje o sesji i porcie COM
    QTimer *timer;

    QByteArray *port_input_bufer;//bufor wejsciowy, do ktorego przychodzą dane z portu szeregowego
    QByteArray *port_output_bufer;//bufor wyjsciowy, z ktorego dane sa wyslane
    Frame frame;

    unsigned short crcChk(unsigned char *data, unsigned char length);
};

#endif // MODBUS_H
