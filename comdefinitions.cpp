#include "comdefinitions.h"

COMDefinitions::COMDefinitions(QString portName, BaudRateType baudRate, FlowType flowRate, ParityType parityType,
															 DataBitsType dataBits, StopBitsType stopBits, QObject *parent) :
	_portName(portName), _baudRate(baudRate), _flowRate(flowRate),
	_parityType(parityType), _dataBits(dataBits), _stopBits(stopBits), QObject(parent)
{
	_fromFile = false;
}

void COMDefinitions::setCOMPort(QString newName)
{
	_portName = newName;
}

void COMDefinitions::setBaudRate(BaudRateType newBaudRate)
{
	_baudRate = newBaudRate;
}

void COMDefinitions::setFlowControl(FlowType newFlowControl)
{
	_flowRate = newFlowControl;
}

void COMDefinitions::setParity(ParityType newParity)
{
	_parityType = newParity;
}

void COMDefinitions::setDataBits(DataBitsType newDataBits)
{
	_dataBits = newDataBits;
}

void COMDefinitions::setStopBits(StopBitsType newStopBits)
{
	_stopBits = newStopBits;
}

QString COMDefinitions::getCOMPort(void)
{
	return _portName;
}

BaudRateType COMDefinitions::getBaudRate(void)
{
	return _baudRate;
}

FlowType COMDefinitions::getFlowControl(void)
{
	return _flowRate;
}

ParityType COMDefinitions::getParity(void)
{
	return _parityType;
}

DataBitsType COMDefinitions::getDataBits(void)
{
	return _dataBits;
}

StopBitsType COMDefinitions::getStopBits(void)
{
	return _stopBits;
}

bool COMDefinitions::getLoadedFromFile(void)
{
	return _fromFile;
}

void COMDefinitions::setLoadedFromFile(bool loadedFromFile)
{
	_fromFile = loadedFromFile;
}
