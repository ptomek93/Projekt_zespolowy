#include "modbus.h"
#include "ui_modbus.h"
#include <windows.h>

Frame cyclic_bufor []=
{
    Frame(1,1,0,1),Frame(2,1,0,1)

};


Frame::Frame(){
    address=0;
    function=0;
    start_addr=0;
    value=0;
}

Frame::Frame(char adres,char func,short addr_start, short val){
    address=adres;
    function=func;
    start_addr=addr_start;
    value=val;
}

void Modbus::processFrame(){
    static int state=0;
    static unsigned short data_length=0;
    static unsigned char array[2]={0,0};
    timer->stop();
    qDebug() << port_input_bufer->size();
    while(!port_input_bufer->isEmpty()){
        switch(state){
        case 0: //address
            frame.address = port_input_bufer->at(0);
            port_input_bufer->remove(0,1);
            state++;
            break;
        case 1://function
            frame.function = port_input_bufer->at(0);
            port_input_bufer->remove(0,1);
            state++;
            break;
        case 2://start address hi
            array[0]=port_input_bufer->at(0);
            port_input_bufer->remove(0,1);
            state++;
            break;
        case 3://start address lo
            array[1]=port_input_bufer->at(0);
            frame.start_addr = array[0]*256 + array[1];
            port_input_bufer->remove(0,1);
            state++;
            break;
        case 4://value address hi
            array[0]=port_input_bufer->at(0)-'0';
            port_input_bufer->remove(0,1);
            state++;
            break;
        case 5://value address lo
            array[1]=port_input_bufer->at(0);
            frame.value = array[0]*256 + array[1];
            port_input_bufer->remove(0,1);
            //jesli to funkcje czytajace to value okresla ile bajtow przesyla slave
            if(frame.function != 0x05 || frame.function != 0x06){
                data_length=frame.value;
            }
            //inaczej sa to funkcje zapisujace ktore odsylaja echo
            else{
                data_length=4;
            }
            state++;
            break;
        case 6://data
            if(data_length-- > 0){
                frame.data.append(port_input_bufer->at(0));
                port_input_bufer->remove(0,1);
            }
            else{
                state++;
            }
            break;
        case 7://value address hi
            array[0]=port_input_bufer->at(0);
            port_input_bufer->remove(0,1);
            state++;
            break;
        case 8://value address lo
            array[1]=port_input_bufer->at(0);
            frame.crc = array[0]*256 + array[1];
            port_input_bufer->remove(0,1);
            state++;
        case 9:
            qDebug() << frame.address << frame.function << frame.start_addr << frame.value << frame.data.toShort()<<frame.crc;
            emit frameReady();
            state=0;
            break;
        }
    }
}

Modbus::Modbus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Modbus)
{
    ui->setupUi(this);

    port_settings = new SettingsDialog::Settings;
    settings_dialog = new SettingsDialog;
    port = new QSerialPort(this);
    timer = new QTimer(this);

    port_input_bufer = new QByteArray;
    port_output_bufer = new QByteArray;

    connect(port, SIGNAL(readyRead()),this,SLOT(readData()));
    connect(timer,SIGNAL(timeout()),this,SLOT(processFrame()));
    connect(this,SIGNAL(frameReady()),this,SLOT(isFrameOk()));
}

Modbus::~Modbus()
{
    closePort();
    delete port;
    delete settings_dialog;
    delete port_settings;
    delete ui;
    delete timer;

    delete port_input_bufer;
    delete port_output_bufer;


}

bool Modbus::openPort(){
    *port_settings = settings_dialog->settings();
    port->setBaudRate(port_settings->baudRate);
    port->setDataBits(port_settings->dataBits);
    port->setStopBits(port_settings->stopBits);
    port->setPortName(port_settings->name);
    port->setParity(port_settings->parity);
    port->setFlowControl(port_settings->flowControl);
    if(port->open(QIODevice::ReadWrite)){
        qDebug() << "open ok";
       return TRUE;
    }
    else qDebug() << "open ok";
    return FALSE;
}

void Modbus::closePort(){
    if(port->isOpen()){
        port->close();
        qDebug() << "close ok";
    }
}

void Modbus::writeData(const QByteArray &data){
    port->write(data);
}

bool Modbus::statePort()
{
    if(port->open(QIODevice::ReadWrite))
        return TRUE;
    else
        return FALSE;
}

void Modbus::readData(){
    timer->stop();
    port_input_bufer->append(port->readAll());
    timer->start(3);
}

void Modbus::showContent(){
    qDebug() << "bufor: " << port_input_bufer->constData();
}

bool Modbus::isFrameOk(){

}

void Modbus::sendRequest(unsigned char slave,unsigned char function,
                         unsigned short start_addr, unsigned short value){

    QByteArray frame;
    QDataStream ds(&frame, QIODevice::WriteOnly);
    ds.setByteOrder(QDataStream::BigEndian);
    ds << quint8(slave) << quint8(function) << quint16(start_addr) << quint16(value);
    //ds.writeRawData(data.constData(), data.size());
    int const crc = crcChk((BYTE*)frame.constData(),frame.size());
    ds.setByteOrder(QDataStream::LittleEndian);
    ds << quint16(crc);
    qDebug() << "crc: " << crc;
    qDebug() << "len: " << frame.size();
    writeData(frame);

}

unsigned short Modbus::crcChk(unsigned char *data, unsigned char length)
{
        int i;
        unsigned int reg_crc=0xFFFF;

        while(length--) {
                reg_crc ^= *data++;
                for(i=0;i<8;i++) {
                        if( reg_crc & 0x01 ) { /*LSB(bit 0) = 1 */
                                reg_crc = (reg_crc >> 1)^0xA001;
                        } else {
                                reg_crc = (reg_crc>>1);
                        }
                }
        }
        return reg_crc;
}

unsigned int CRC16_2(unsigned char *buf, int len)
{
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
  crc ^= (unsigned int)buf[pos];    // XOR byte into least sig. byte of crc

  for (int i = 8; i != 0; i--) {    // Loop over each bit
    if ((crc & 0x0001) != 0) {      // If the LSB is set
      crc >>= 1;                    // Shift right and XOR 0xA001
      crc ^= 0xA001;
    }
    else                            // Else LSB is not set
      crc >>= 1;                    // Just shift right
    }
  }

  return crc;
}


void Modbus::recive_data_from_widget(unsigned char slave_addres, unsigned char function, unsigned short start_addr, unsigned short value){


};
