#ifndef ABSTRACTVARIABLEHANDLER_H
#define ABSTRACTVARIABLEHANDLER_H

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDebug>
#include <QPair>

class QSettings;
class CommunicationDataContainer;

class AbstractVariableHandler : public QObject
{
	Q_OBJECT
public:
	static AbstractVariableHandler* Instance(QObject* parent = 0);

	void                updateBooleanVariable(const QString&, const unsigned int&);
	void                updateIntegerVariable(const QString&, const int&);
	void                updateDoubleVariable(const QString&, const double&);
	void                loadVariablesFromFile(QSettings* settings);

	bool                                   hasInstanceLoadedData() const;
	const unsigned int&                    dspToPCPkgSize() const;
	const unsigned int&                    pcToDSPPkgSize() const;
	const QList<QPair<QString, QString> >& getPCtoDSPVarList() const;
	const QList<QPair<QString, QString> >& getDSPtoPCVarList() const;

	CommunicationDataContainer*            getDSPtoPCVarContainer() const;
	
signals:
	void dataLoadedSuccessfully();
	void dataHasBeenUpdated();
	void newMsgToDSPReadyToSend(const QPair<unsigned char, QByteArray>);

public slots:
	void processIncomingData(QByteArray);
	
private:
	explicit AbstractVariableHandler(QObject *parent = 0);

	void     processDataToSend();

	CommunicationDataContainer*     _dspToPCContainer;
	CommunicationDataContainer*     _pcToDSPContainer;
	QList<QPair<QString, QString> > _dspToPCVarList;
	QList<QPair<QString, QString> > _pcToDSPVarList;
	unsigned int                    _sizeOfDSPToPCPkg;
	unsigned int                    _sizeOfPCToDSPPkg;
	bool                            _loadedFromFile;

};

#endif // ABSTRACTVARIABLEHANDLER_H
