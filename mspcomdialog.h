#ifndef MSPCOMDIALOG_H
#define MSPCOMDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "qextserialport.h"

namespace Ui {
class MspCOMDialog;
}

class MspCOMDialog : public QDialog
{
    Q_OBJECT
    
public slots:
    bool findAvailableCOMPorts(void);
    void openCOMPort(void);
    void closeCOMPort(void);

public:
    explicit MspCOMDialog(QWidget *parent = 0);
    explicit MspCOMDialog(bool enableDialog, QString actualCOM, QWidget *parent = 0);
    BaudRateType getBaudRate(void);
    FlowType getFlowControl(void);
    ParityType getParity(void);
    DataBitsType getDataBits(void);
    StopBitsType getStopBits(void);
    QString getCurrentPortName(void);
    ~MspCOMDialog();
    
private:
    Ui::MspCOMDialog *ui;    
    //QStringList availablePorts;
    //QStringList availablePortsClean;

    QextSerialPort *usbPort;
    bool farEnableDialog;
    QString openCOM;
};

#endif // MSPCOMDIALOG_H
