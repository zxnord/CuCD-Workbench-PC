#ifndef COMDEFINITIONS_H
#define COMDEFINITIONS_H

#include <QObject>
#include "qextserialport.h"

class COMDefinitions : public QObject
{
	Q_OBJECT

public:
	explicit COMDefinitions(QString portName, BaudRateType baudRate, FlowType flowRate, ParityType parityType,
													DataBitsType dataBits, StopBitsType stopBits, QObject *parent = 0);
	QString getCOMPort(void);
	BaudRateType getBaudRate(void);
	FlowType getFlowControl(void);
	ParityType getParity(void);
	DataBitsType getDataBits(void);
	StopBitsType getStopBits(void);
	bool getLoadedFromFile(void);
	void setCOMPort(QString newName);
	void setBaudRate(BaudRateType newBaudRate);
	void setFlowControl(FlowType newFlowControl);
	void setParity(ParityType newParity);
	void setDataBits(DataBitsType newDataBits);
	void setStopBits(StopBitsType newStopBits);
	void setLoadedFromFile(bool loadedFromFile);

signals:

public slots:

private:
	QString _portName;
	BaudRateType _baudRate;
	FlowType _flowRate;
	ParityType _parityType;
	DataBitsType _dataBits;
	StopBitsType _stopBits;
	bool _fromFile;

};

#endif // COMDEFINITIONS_H
