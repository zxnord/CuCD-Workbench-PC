#ifndef COMUNICATIONCONTROLLER_H
#define COMUNICATIONCONTROLLER_H

#include <QObject>
#include <QList>
#include <QPair>
#include "comdefinitions.h"
#include "qextserialport.h"

class QByteArray;

class ComunicationController : public QObject
{
	Q_OBJECT

public:
	enum COMConectionResult {
		COM_ERROR_GLOBAL,
		COM_OPENED_OK,
		COM_CLOSED_OK,
		COM_ERROR_OPENING,
		COM_ERROR_CLOSING
	};

	// Static constructor
	static ComunicationController*                  Instance(QObject* parent = 0);

	COMConectionResult                              configureUSBPort();
	const QList<QPair<unsigned char, QByteArray> >& getRegisteredCommands() const;
	void                                            setRegisteredCommands(const QList<QPair<unsigned char, QByteArray> >&);
	bool                                            isUSBPortConfigured() const;
	bool                                            isUSBPortConnected() const;
	bool                                            connectUSBPort();
	void                                            disconnectUSB();
	void                                            sendHeaders();

signals:
	void applicationReadyToStart(bool);
	void newDataRawReceived(const QByteArray&);
	void newValidDataReceived(QByteArray);
	void commandSent(QByteArray);
	void commandRegistered();

public slots:
	void dataReceived();
	void registerNewCommnand(const QPair<unsigned char, QByteArray>);

private:
	explicit ComunicationController(QObject *parent = 0);

	bool doCheckSumToNewData(QByteArray);
	void sendNewCommand();

	COMDefinitions*                          _comData;
	QextSerialPort*                          _usbPort;
	QList<QPair<unsigned char, QByteArray> > _commandStack;

};

#endif // COMUNICATIONCONTROLLER_H
