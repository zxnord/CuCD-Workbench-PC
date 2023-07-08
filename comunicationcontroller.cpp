#include "comunicationcontroller.h"
#include "abstractvariablehandler.h"
#include "mspcomdialog.h"
#include "cucdutil.h"

#include <QDebug>
#include <QByteArray>
#include <iostream>
#include <QThread>

ComunicationController* ComunicationController::Instance(QObject* parent)
{
	static ComunicationController* mainController = NULL;

	if( mainController == NULL )
	{
		if( parent != NULL )
		{
			mainController = new ComunicationController(parent);
		}
	}

	return mainController;
}


ComunicationController::ComunicationController(QObject* parent) :
	QObject(parent)
{
	_comData = NULL;
	_commandStack << QPair<unsigned char, QByteArray>(0x01, QByteArray());
	_usbPort = new QextSerialPort(QextSerialPort::EventDriven, this);
}


ComunicationController::COMConectionResult ComunicationController::configureUSBPort()
{
	COMConectionResult result = COM_ERROR_GLOBAL;

	if( _usbPort == NULL )
	{
		qDebug() << "Error!! QextSerialPort was not created!";
		return result;
	}

	if( !_usbPort->isOpen() )
	{
		MspCOMDialog dialog(qobject_cast<QWidget*>(this));

		if(dialog.exec() == QDialog::Accepted)
		{
			qDebug() << "nombre del puerto " << dialog.getCurrentPortName();

			if( _comData != NULL )
				delete _comData;

			_comData = new COMDefinitions(dialog.getCurrentPortName(), dialog.getBaudRate(),
																		dialog.getFlowControl(), dialog.getParity(),
																		dialog.getDataBits(), dialog.getStopBits(),
																		this);

			if( connectUSBPort() )
			{
				result = COM_OPENED_OK;
			}
			else
				result = COM_ERROR_OPENING;
		}
	}
	else
	{
		// Port already opened
		MspCOMDialog dialog1(true, _comData->getCOMPort(), qobject_cast<QWidget*>(this));

		if(dialog1.exec() == QDialog::Accepted)
		{
				disconnectUSB();
				delete _comData;
				result = COM_CLOSED_OK;
		}
	}

	return result;
}


bool ComunicationController::connectUSBPort()
{
	if( _comData != NULL )
	{
		_usbPort->setPortName(_comData->getCOMPort());
		_usbPort->setBaudRate(_comData->getBaudRate());
		_usbPort->setFlowControl(_comData->getFlowControl());
		_usbPort->setDataBits(_comData->getDataBits());
		_usbPort->setStopBits(_comData->getStopBits());
		_usbPort->setParity(_comData->getParity());

		connect(_usbPort, SIGNAL(readyRead()), this, SLOT(dataReceived()));

		if( _usbPort->open(QextSerialPort::ReadWrite) )
		{
			emit applicationReadyToStart(true);
			return true;
		}
	}

	return false;
}

void ComunicationController::dataReceived()
{
	QByteArray msg;
	// Receive all msg
	msg = _usbPort->readAll();
	qDebug() << "Raw msg: " << QString(msg);
	emit newDataRawReceived(msg);

	if( QString(msg).contains("SEND!") )
	{
		// Send new command.
		_usbPort->flush();
		sendNewCommand();
		qDebug() << "Sending new command.";
		return;
	}

	if( msg.size() != AbstractVariableHandler::Instance()->dspToPCPkgSize() )
	{
		qDebug() << "Dropping unvalid pkg!";
		return;
	}

	if( doCheckSumToNewData(msg) )
	{
		qDebug() << "Received valid data!";
		emit newValidDataReceived(msg);
	}
	else
		qDebug() << "Dropping bad checksum package =(";

}


void ComunicationController::disconnectUSB()
{
	disconnect(_usbPort, SIGNAL(readyRead()), this, SLOT(dataReceived()));
	_usbPort->close();

	emit applicationReadyToStart(false);
}


bool ComunicationController::isUSBPortConnected() const
{
	return _usbPort->isOpen();
}


bool ComunicationController::isUSBPortConfigured() const
{
	return (_comData != NULL);
}


bool ComunicationController::doCheckSumToNewData(QByteArray msg)
{
	unsigned int sizeWithoutCheckSum = AbstractVariableHandler::Instance()->dspToPCPkgSize() - 4;
	unsigned int iterator = 0;
	unsigned char* rawPtr = reinterpret_cast<unsigned char*>(msg.data());

	unsigned int currentCheckSum = 0;

	while( iterator < sizeWithoutCheckSum )
	{
		currentCheckSum ^= *(reinterpret_cast<unsigned int*>(rawPtr + iterator));

		iterator += 4;
	}

	return ( currentCheckSum == *(reinterpret_cast<unsigned int*>(rawPtr + iterator)) );
}


void ComunicationController::sendNewCommand()
{
	if( !_usbPort->isOpen() )
		return;

	QPair<unsigned char, QByteArray> cmd = _commandStack.takeLast();

	QByteArray fullMsg = QByteArray(1, '@');

	fullMsg.append(cmd.first + '0');

	if( !cmd.second.isEmpty() )
		fullMsg.append(cmd.second);

	for( QByteArray::Iterator it = fullMsg.begin();
			 it != fullMsg.end();
			 ++it)
	{
		CuCDUtilThread::msleep(20);
		_usbPort->putChar(*it);
	}

	if( _commandStack.isEmpty() )
		_commandStack << QPair<unsigned char, QByteArray>(0x01, QByteArray());

	emit commandSent(fullMsg);
}


void ComunicationController::registerNewCommnand(const QPair<unsigned char, QByteArray> cmd)
{
	if( (cmd.first > 0x00) && (cmd.first < 0x06) )
	{
		_commandStack << cmd;

		emit commandRegistered();
	}
}


const QList<QPair<unsigned char, QByteArray> >& ComunicationController::getRegisteredCommands() const
{
	return _commandStack;
}


void ComunicationController::setRegisteredCommands(const QList<QPair<unsigned char, QByteArray> >& cmds)
{
	_commandStack = cmds;

	emit commandRegistered();
}


void ComunicationController::sendHeaders()
{
	QString mspHeader("#I");
	QByteArray sizeByte = CuCDUtil::convertShortToHexStr(AbstractVariableHandler::Instance()->pcToDSPPkgSize());
	mspHeader.append(sizeByte).append("O");
	sizeByte = CuCDUtil::convertShortToHexStr(AbstractVariableHandler::Instance()->dspToPCPkgSize());
	mspHeader.append(sizeByte);

	QByteArray mspHeaderRaw = mspHeader.toUtf8();
	qDebug() << "Header msg to be sent:" << QString(mspHeaderRaw);

	if( _usbPort->isOpen() )
	{
		for( QByteArray::Iterator it = mspHeaderRaw.begin();
				 it != mspHeaderRaw.end();
				 ++it )
		{
			CuCDUtilThread::msleep(20);
			_usbPort->putChar(*it);
		}

		emit commandSent(mspHeaderRaw);
	}
}
