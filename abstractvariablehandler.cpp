#include "abstractvariablehandler.h"
#include "communicationdatacontainer.h"
#include <QSettings>
#include <QPair>

AbstractVariableHandler* AbstractVariableHandler::Instance(QObject *parent)
{
	static AbstractVariableHandler* singleton = NULL;

	if( singleton == NULL )
	{
		if( parent != NULL )
		{
			singleton = new AbstractVariableHandler(parent);
		}
	}

	return singleton;
}

AbstractVariableHandler::AbstractVariableHandler(QObject *parent) :
	QObject(parent)
{
	_dspToPCContainer = NULL;
	_pcToDSPContainer = NULL;
	_sizeOfDSPToPCPkg = 0;
	_sizeOfPCToDSPPkg = 0;
	_loadedFromFile = false;
	_pcToDSPVarList = QList<QPair<QString, QString> >();
	_dspToPCVarList = QList<QPair<QString, QString> >();
}


bool AbstractVariableHandler::hasInstanceLoadedData() const
{
	return ( (_dspToPCContainer != NULL) &&
					 (_pcToDSPContainer != NULL) &&
					 (_loadedFromFile) );
}


void AbstractVariableHandler::loadVariablesFromFile(QSettings* settings)
{
	int nofVariables = 0;

	if( settings == NULL )
		return;

	_dspToPCVarList.clear();
	_pcToDSPVarList.clear();

	if( _pcToDSPContainer )
		delete _pcToDSPContainer;

	if( _dspToPCContainer )
		delete _dspToPCContainer;

	_pcToDSPContainer = new CommunicationDataContainer(false);
	settings->beginGroup("PCToDSPData");
	nofVariables = settings->value("nofVariables", 0).toInt();
	qDebug() << "Nof Variables from PC to DSP: " << nofVariables;

	for(int i = 1; i < nofVariables + 1; ++i)
	{
		QString varName = settings->value(QString("variable_%1_name").arg(i)).toString();
		QString varType = settings->value(QString("variable_%1_type").arg(i)).toString();

		if( varType == "uint32_t" )
		{
			_pcToDSPContainer->initializeBooleanVariable(varName);
			_sizeOfPCToDSPPkg += 4;
		}
		else if( varType == "int32_t" )
		{
			_pcToDSPContainer->initializeIntegerVariable(varName);
			_sizeOfPCToDSPPkg += 4;
		}
		else if( varType == "double" )
		{
			_pcToDSPContainer->initializeDoubleVariable(varName);
			_sizeOfPCToDSPPkg += 8;
		}
		_pcToDSPVarList << QPair<QString, QString>(varName, varType);
	}
	_sizeOfPCToDSPPkg += 4;
	settings->endGroup();

	nofVariables = 0;
	_dspToPCContainer = new CommunicationDataContainer();
	settings->beginGroup("DSPtoPCData");
	nofVariables = settings->value("nofVariables", 0).toInt();
	qDebug() << "Nof Variables from DSP to PC: " << nofVariables;

	for(int i = 1; i < nofVariables + 1; ++i)
	{
		QString varName = settings->value(QString("variable_%1_name").arg(i)).toString();
		QString varType = settings->value(QString("variable_%1_type").arg(i)).toString();

		if( varType == "uint32_t" )
		{
			_dspToPCContainer->initializeBooleanVariable(varName);
			_sizeOfDSPToPCPkg += 4;
		}
		else if( varType == "int32_t" )
		{
			_dspToPCContainer->initializeIntegerVariable(varName);
			_sizeOfDSPToPCPkg += 4;
		}
		else if( varType == "double" )
		{
			_dspToPCContainer->initializeDoubleVariable(varName);
			_sizeOfDSPToPCPkg += 8;
		}
		_dspToPCVarList << QPair<QString, QString>(varName, varType);
	}
	_sizeOfDSPToPCPkg += 4;
	settings->endGroup();

	_loadedFromFile = true;
	emit dataLoadedSuccessfully();
}


const QList<QPair<QString, QString> >& AbstractVariableHandler::getPCtoDSPVarList() const
{
	return _pcToDSPVarList;
}


const QList<QPair<QString, QString> >& AbstractVariableHandler::getDSPtoPCVarList() const
{
	return _dspToPCVarList;
}


const unsigned int& AbstractVariableHandler::dspToPCPkgSize() const
{
	return _sizeOfDSPToPCPkg;
}


const unsigned int& AbstractVariableHandler::pcToDSPPkgSize() const
{
	return _sizeOfPCToDSPPkg;
}


void AbstractVariableHandler::processIncomingData(QByteArray data)
{
	if( !hasInstanceLoadedData() ||
			(data.size() != _sizeOfDSPToPCPkg) )
		return;

	unsigned char* rawPtr = reinterpret_cast<unsigned char*>(data.data());

	for(int i = 0; i < _dspToPCVarList.size(); ++i)
	{
		if( _dspToPCVarList.at(i).second == "uint32_t" )
		{
			unsigned int rawValue = *(reinterpret_cast<unsigned int*>(rawPtr));

			if( rawValue != 0 )
			{
				qDebug() << "AVH PROCESS>> Received a true boolean " << rawValue;
				rawValue = 5;
			}
			else
			{
				qDebug() << "AVH PROCESS>> Received a false boolean " << rawValue;
			}

			_dspToPCContainer->setBooleanVariable(_dspToPCVarList.at(i).first,
																						rawValue);
			rawPtr += 4;
		}
		else if( _dspToPCVarList.at(i).second == "int32_t" )
		{
			_dspToPCContainer->setIntegerVariable(_dspToPCVarList.at(i).first,
																						*(reinterpret_cast<int*>(rawPtr)));

			qDebug() << "AVH PROCESS>> Received integer " << *(reinterpret_cast<int*>(rawPtr));

			rawPtr += 4;
		}
		else if( _dspToPCVarList.at(i).second == "double" )
		{
			_dspToPCContainer->setDoubleVariable(_dspToPCVarList.at(i).first,
																					 *(reinterpret_cast<double*>(rawPtr)));

			qDebug() << "AVH PROCESS>> Received double " << *(reinterpret_cast<double*>(rawPtr));

			rawPtr += 8;
		}
	}

	emit dataHasBeenUpdated();
}


void AbstractVariableHandler::processDataToSend()
{
	if( _pcToDSPVarList.isEmpty() )
		return;

	unsigned int checkSum = 0;
	QPair<unsigned char, QByteArray> newCmd = QPair<unsigned char, QByteArray>(0x04, QByteArray());

	for( int i = 0; i < _pcToDSPVarList.size(); ++i )
	{
		if( _pcToDSPVarList.at(i).second == "uint32_t" )
		{
			unsigned int value = _pcToDSPContainer->getBooleanVariable(_pcToDSPVarList.at(i).first);
			checkSum ^= value;
			newCmd.second.append(reinterpret_cast<const char*>(&value), 4);
		}
		else if( _pcToDSPVarList.at(i).second == "int32_t" )
		{
			int value = _pcToDSPContainer->getIntegerVariable(_pcToDSPVarList.at(i).first);
			checkSum ^= *(reinterpret_cast<unsigned int*>(&value));
			newCmd.second.append(reinterpret_cast<const char*>(&value), 4);
		}
		else if( _pcToDSPVarList.at(i).second == "double" )
		{
			double value = _pcToDSPContainer->getDoubleVariable(_pcToDSPVarList.at(i).first);
			unsigned int* localPtr = reinterpret_cast<unsigned int*>(&value);
			checkSum ^= *localPtr;
			localPtr++;
			checkSum ^= *localPtr;
			newCmd.second.append(reinterpret_cast<const char*>(&value), 8);
		}
	}

	newCmd.second.append(reinterpret_cast<const char*>(&checkSum), 4);

	qDebug() << "New command: " << newCmd.first << "Args size: " << newCmd.second.size() <<
							"Args: " << newCmd.second << "CheckSum: " << checkSum;

	emit newMsgToDSPReadyToSend(newCmd);
}


void AbstractVariableHandler::updateBooleanVariable(const QString& varName, const unsigned int& value)
{
	if( _pcToDSPVarList.isEmpty() )
		return;

	if( _pcToDSPContainer->setBooleanVariable(varName, value) )
	{
		processDataToSend();
	}
}


void AbstractVariableHandler::updateIntegerVariable(const QString& varName, const int& value)
{
	if( _pcToDSPVarList.isEmpty() )
		return;

	if( _pcToDSPContainer->setIntegerVariable(varName, value) )
	{
		processDataToSend();
	}
}


void AbstractVariableHandler::updateDoubleVariable(const QString& varName, const double& value)
{
	if( _pcToDSPVarList.isEmpty() )
		return;

	if( _pcToDSPContainer->setDoubleVariable(varName, value) )
	{
		processDataToSend();
	}
}


CommunicationDataContainer* AbstractVariableHandler::getDSPtoPCVarContainer() const
{
	return _dspToPCContainer;
}
