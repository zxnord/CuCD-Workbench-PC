#include "mspcomdialog.h"
#include "ui_mspcomdialog.h"
#include <QDebug>

MspCOMDialog::MspCOMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MspCOMDialog)
{
    ui->setupUi(this);
    setWindowTitle("USB COM Configuration");

    //Connections
    connect(this->ui->findButton, SIGNAL(pressed()), this, SLOT(findAvailableCOMPorts()));
    connect(this->ui->openButton, SIGNAL(pressed()), this, SLOT(openCOMPort()));
}

MspCOMDialog::MspCOMDialog(bool enableDialog, QString actualCOM, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MspCOMDialog)
{
    ui->setupUi(this);
    setWindowTitle("USB COM Configuration");

    farEnableDialog = enableDialog;
    openCOM = actualCOM;

    if(farEnableDialog){
        ui->portsComboBox->setEnabled(false);
        ui->findButton->setEnabled(false);
        ui->openButton->setEnabled(true);
        ui->openButton->setText("Close");
        ui->portsComboBox->addItem(openCOM);

    }

    //Connections
    connect(this->ui->findButton, SIGNAL(pressed()), this, SLOT(findAvailableCOMPorts()));
    connect(this->ui->openButton, SIGNAL(pressed()), this, SLOT(closeCOMPort()));
}

MspCOMDialog::~MspCOMDialog()
{
    delete ui;
}

bool MspCOMDialog::findAvailableCOMPorts(void){
    int MAX_COM_PORT = 20;    // The maximum number of COM ports that will be searched for.
    QString testPortName;
    QStringList availablePorts, availablePortsClean;

    //availablePorts.clear();         // Clears the ports found in the previous search.
    //availablePortsClean.clear();
    ui->portsComboBox->clear();
    QextSerialPort testPort;
    testPort.setBaudRate(BAUD115200);
    testPort.setFlowControl(FLOW_OFF);
    testPort.setParity(PAR_NONE);
    testPort.setDataBits(DATA_8);
    testPort.setStopBits(STOP_1);

    for (int i = 1; i < MAX_COM_PORT; i++)
    {
        testPortName = QString("\\\\.\\COM%1").arg(i);  // I TRIED TO PREFIX "\\.\" IN HERE..
        testPort.setPortName(testPortName);
        if (testPort.open(QIODevice::ReadWrite))
        {
            availablePorts.append(testPortName);
            testPort.close();
        }
        qDebug() << "try "  << i;
    }
    if (availablePorts.isEmpty())
    {
        QMessageBox::information(this,tr("Information Message"),tr("No available COM ports found") );
        ui->portsComboBox->clear();
        return false;
    }
    else
    {
        qDebug() << "availablePorts:" << availablePorts;
        ui->portsComboBox->clear();

        for(int i=0; i<availablePorts.length(); i++)
        {
            availablePortsClean.append(availablePorts.at(i).split("\\").at(3));
            qDebug() << "second try "  << i;
        }

        ui->portsComboBox->addItems(availablePortsClean);
        ui->openButton->setEnabled(true);
        ui->findButton->setEnabled(false);
        ui->settingsGroupBox->setEnabled(true);
        return true;
    }
}

void MspCOMDialog::openCOMPort(void){
//    usbPort = new QextSerialPort(QextSerialPort::EventDriven, this->parent());
//    usbPort->setPortName(ui->portsComboBox->currentText());
//    setBauds();
//    setFlow();
//    setParity();
//    setDataBits();
//    setStopBits();

//    if(usbPort->open(QIODevice::ReadWrite))
//    {
        ui->openButton->setEnabled(false);
//        QMessageBox::information(this,tr("Information Message"),tr("Port opened :-).") );
        //usbPort->write("hello 1");
        this->accept();
//    }
//    else
//        QMessageBox::information(this,tr("Information Message"),tr("Couldn't open port.") );
}

void MspCOMDialog::closeCOMPort(){
    this->accept();
}

QString MspCOMDialog::getCurrentPortName(){
    return ui->portsComboBox->currentText();
}

BaudRateType MspCOMDialog::getBaudRate()
{
  switch( ui->baudComboBox->currentIndex() )
  {
  case 0:
    return BAUD2400;
  case 1:
    return BAUD4800;
  case 2:
    return BAUD9600;
  case 3:
    return BAUD19200;
  case 4:
    return BAUD38400;
  case 5:
    return BAUD57600;
  case 6:
    return BAUD115200;
  default:
    // Should never get here!
    return BAUD115200;
  }
}

FlowType MspCOMDialog::getFlowControl()
{
  switch( ui->flowComboBox->currentIndex() )
  {
  case 0:
    return FLOW_OFF;
  case 1:
    return FLOW_HARDWARE;
  case 2:
    return FLOW_XONXOFF;
  default:
    //Should never get here!
    return FLOW_OFF;
  }
}

DataBitsType MspCOMDialog::getDataBits()
{
  switch( ui->bitsComboBox->currentIndex() )
  {
  case 0:
    return DATA_5;
  case 1:
    return DATA_6;
  case 2:
    return DATA_7;
  case 3:
    return DATA_8;
  default:
    //Should never get here!
    return DATA_8;
  }
}

StopBitsType MspCOMDialog::getStopBits()
{
  switch( ui->stopComboBox->currentIndex() )
  {
  case 0:
    return STOP_1;
  case 1:
    return STOP_2;
  default:
    //Should never get here!
    return STOP_1;
  }
}

ParityType MspCOMDialog::getParity()
{
  switch( ui->parComboBox->currentIndex() )
  {
  case 0:
    return PAR_NONE;
  case 1:
    return PAR_ODD;
  case 2:
    return PAR_EVEN;
  case 3:
    return PAR_SPACE;
  default:
    //Should never get here!
    return PAR_NONE;
  }
}
