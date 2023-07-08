#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wizarddialog.h"
#include "exportcsvdialog.h"
#include <iostream>
#include <stdlib.h>
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	createActions();
	createToolBars();
	setWindowIcon(QIcon(":/images/escudo_utfsm.gif"));
	setWindowTitle("UTFSM - CuCD Workspace v0.9 by Edson Contreras");

	ui->mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);

	windowCounter = 0;
	referenceWindowCounter = 0;
	enableConnectButton = false;
	enableVariableEditing = false;
	connection = false;
	cucdLibActive = false;

	// Create the controller instance
	ComunicationController::Instance(this);

	// Creater the Abstract Variable Handler instance
	AbstractVariableHandler::Instance(this);

	connect(AbstractVariableHandler::Instance(), SIGNAL(dataLoadedSuccessfully()),
					this, SLOT(onVariablesReady()));

	connect(AbstractVariableHandler::Instance(), SIGNAL(newMsgToDSPReadyToSend(QPair<unsigned char,QByteArray>)),
					ComunicationController::Instance(), SLOT(registerNewCommnand(QPair<unsigned char,QByteArray>)));

	connect(ComunicationController::Instance(), SIGNAL(newValidDataReceived(QByteArray)),
					AbstractVariableHandler::Instance(), SLOT(processIncomingData(QByteArray)));

	connect(ComunicationController::Instance(), SIGNAL(applicationReadyToStart(bool)),
					this, SLOT(onApplicationReadyToStart(bool)));

	WizardDialog* wiz = new WizardDialog(this);
	wiz->setModal(true);
	wiz->show();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createActions()
{
	//Crear I­conos
	//1) Plot
	newAct = new QAction(QIcon(":/images/plot.jpg"), tr("&New plot"), this);
	newAct->setShortcuts(QKeySequence::New);
	newAct->setStatusTip(tr("Create a new plot"));
	newAct->setEnabled(false);
	connect(newAct, SIGNAL(triggered()), this, SLOT(newPlot()));

	//2) Open
	openAct = new QAction(QIcon(":/images/file-open.png"), tr("&Open"), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open existing file"));
	connect(openAct, SIGNAL(triggered()), this->ui->mdiArea, SLOT(tileSubWindows()));

	//3) Save
	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save file"));
	connect(saveAct, SIGNAL(triggered()), this->ui->mdiArea, SLOT(cascadeSubWindows()));

	//4) Connect
	connectAct = new QAction(QIcon(":/images/connection-lost.png"), tr("&Connect"), this);
	//connectAct->setShortcuts(QKeySequence::Save);
	connectAct->setStatusTip(tr("Connect to MSP430"));
	connect(connectAct, SIGNAL(triggered()), this, SLOT(connectButtonAction()));

	//5) Connection settings
	connectSettingAct = new QAction(QIcon(":/images/settings.png"), tr("C&onnection Setting"), this);
	//connectSettingAct->setShortcuts(QKeySequence::Save);
	connectSettingAct->setStatusTip(tr("Edit connection settings"));
	connect(connectSettingAct, SIGNAL(triggered()), this, SLOT(connectOptions()));

	//6) Reference control panel
	refControlPanelAct = new QAction(QIcon(":/images/control-panel2.png"), tr("&Reference Panel"), this);
	//connectSettingAct->setShortcuts(QKeySequence::Save);
	refControlPanelAct->setStatusTip(tr("Add a reference(s) panel"));
	refControlPanelAct->setEnabled(false);
	connect(refControlPanelAct, SIGNAL(triggered()), this, SLOT(addRefControlPanels()));

	//7) Start the magic show xD
	variablesAct = new QAction(QIcon(":/images/play_button.png"), tr("S&tart Communication"), this);
	variablesAct->setStatusTip(tr("Send the first message to MSP and start the communication."));
	variablesAct->setEnabled(false);
//	connect(variablesAct, SIGNAL(triggered()), this, SLOT(variablesConfiguration()));

	connect(this->ui->actionExport, SIGNAL(triggered()), this, SLOT(onExportingVariables()));
}

void MainWindow::createToolBars()
{
	//Anadir Iconos a la barra

	this->ui->toolBar->addAction(newAct);
	this->ui->toolBar->addAction(refControlPanelAct);
	/////////////////////////////////////
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->setAllowedAreas(Qt::LeftToolBarArea);
	fileToolBar->setOrientation(Qt::Vertical);
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);
	/////////////////////////////////////
	insertToolBar(this->ui->toolBar, fileToolBar);
	/////////////////////////////////////
	connectToolBar = addToolBar(tr("Connection"));
	connectToolBar->setAllowedAreas(Qt::LeftToolBarArea);
	connectToolBar->setOrientation(Qt::Vertical);
	connectToolBar->addAction(connectAct);
	connectToolBar->addAction(connectSettingAct);
	connectToolBar->addAction(variablesAct);
	/////////////////////////////////////
	insertToolBar(this->ui->toolBar, connectToolBar);

}

void MainWindow::newPlot()
{
		PlotDialog dialog(this);
		dialog.setWindowTitle(QString("Add new plot"));
		if(dialog.exec() == QDialog::Accepted)
		{
				QString title = dialog.getPlotName();
				plotMdiWindow = new PlotMdiWindow(dialog.getPlotVariables(),
																					dialog.getTimeStamp(),
																					title,
																					this);
				ui->mdiArea->addSubWindow(plotMdiWindow);
				plotMdiWindow->show();
				plotMdiWindow->activateWindow();
		}
}

void MainWindow::openFile()
{
    connectAct->setIcon(QIcon(":/images/connection-established.png"));
    statusBar()->showMessage("Connection Established", 5000);
}


void MainWindow::onExportingVariables()
{
  ExportCSVDialog dialog;

  if( dialog.exec() == QDialog::Accepted )
  {
    statusBar()->showMessage("Export Successful", 5000);
  }
}


void MainWindow::addRefControlPanels()
{
  const QList<QPair<QString, QString> > variables = AbstractVariableHandler::Instance()->getPCtoDSPVarList();

  if( variables.isEmpty() )
    return;

  for( int i = 0; i < variables.size(); ++i )
  {
    RefControlPanel::RefVariableType varType = RefControlPanel::REF_VAR_NONE;

    if( variables.at(i).second == "uint32_t" )
    {
      varType = RefControlPanel::REF_VAR_ONOFF;
    }
    else if( variables.at(i).second == "int32_t" )
    {
      varType = RefControlPanel::REF_VAR_INT;
    }
    else if( variables.at(i).second == "double" )
    {
      varType = RefControlPanel::REF_VAR_DOUBLE;
    }
    else
      continue;

    QMdiSubWindow* refMdiWindow = new QMdiSubWindow(ui->mdiArea);
    RefControlPanel* refWidget = new RefControlPanel(variables.at(i).first, varType, refMdiWindow);

    refMdiWindow->setWindowTitle(variables.at(i).first + " Control Panel");
    refMdiWindow->setWidget(refWidget);
    refMdiWindow->setWindowIcon(QIcon(":/images/icon-future.gif"));
    ui->mdiArea->addSubWindow(refMdiWindow);
    refMdiWindow->show();
    refMdiWindow->activateWindow();
  }

  QMdiSubWindow* refMdiWindow = new QMdiSubWindow(ui->mdiArea);
  ConsoleCommandWidget* widget = new ConsoleCommandWidget(refMdiWindow);
  refMdiWindow->setWindowTitle("Console and Commands Panel");
  refMdiWindow->setWidget(widget);
  refMdiWindow->setWindowIcon(QIcon(":/images/icon-future.gif"));
  ui->mdiArea->addSubWindow(refMdiWindow);
  refMdiWindow->show();
  refMdiWindow->activateWindow();
}


void MainWindow::connectButtonAction()
{
  if( ComunicationController::Instance()->isUSBPortConnected() )
  {
    ComunicationController::Instance()->disconnectUSB();

    connectAct->setIcon(QIcon(":/images/connection-lost.png"));
    variablesAct->setEnabled(false);
    refControlPanelAct->setEnabled(false);
    QMessageBox::information(this,tr("Information Message"),tr("Port safely closed =).") );
  }
  else
  {
    if( ComunicationController::Instance()->isUSBPortConfigured() )
    {
      ComunicationController::Instance()->connectUSBPort();

      connectAct->setIcon(QIcon(":/images/connection-established.png"));
      variablesAct->setEnabled(true);
      refControlPanelAct->setEnabled(true);
      QMessageBox::information(this,tr("Information Message"),tr("Port opened =).") );
    }
    else
    {
      connectOptions();
    }
  }
}

void MainWindow::connectOptions()
{
	if( !AbstractVariableHandler::Instance()->hasInstanceLoadedData() )
	{
		int fromFile = QMessageBox::question(this,
																				 "Question Message",
																				 "Variables definition not found!\nDo you want to load it from file? Or launch wizard?",
																				 "File",
																				 "Wizard",
																				 "Cancel",
																				 2,
																				 2);
		switch( fromFile )
		{
		case 0: //Load definitions from file
			if( !loadVariablesDefFromFile() )
			{
				QMessageBox::information(this,tr("Information Message"),tr("Port can not be opened until you load your 'Variables Definition' file.") );
				return;
			}
			break;
		case 1: //Launch variables wizard
		{
			WizardDialog dialog(this);
			if( dialog.exec() == QDialog::Rejected )
			{
				QMessageBox::information(this,tr("Information Message"),tr("Port can not be opened until you load your 'Variables Definition' file.") );
				return;
			}
			break;
		}
		default:
			QMessageBox::information(this,tr("Information Message"),tr("Port can not be opened until you load your 'Variables Definition' file.") );
			return;
			break;
		}
	}

	// When variables definitions are loaded, proceed to configure the USB port.
	switch( ComunicationController::Instance()->configureUSBPort() )
	{
	case ComunicationController::COM_OPENED_OK:
		QMessageBox::information(this,tr("Information Message"),tr("Port opened =).") );
		connectAct->setIcon(QIcon(":/images/connection-established.png"));
		statusBar()->showMessage("Connection Established", 5000);
		enableConnectButton = true;
		connection = true;
		break;
	case ComunicationController::COM_ERROR_OPENING:
		QMessageBox::information(this,tr("Information Message"),tr("Port could not be opened.") );
		statusBar()->showMessage("Connection can not be completed", 5000);
		break;
	case ComunicationController::COM_CLOSED_OK:
		connection = false;
		variablesAct->setEnabled(false);
		refControlPanelAct->setEnabled(false);
		QMessageBox::information(this,tr("Information Message"),tr("Port safely closed =).") );
		connectAct->setIcon(QIcon(":/images/connection-lost.png"));
		statusBar()->showMessage("Connection can not be completed", 5000);
		break;
	case ComunicationController::COM_ERROR_GLOBAL:
		QMessageBox::critical(this,tr("Information Message"),tr("Communication error! Please reconnect the USB cable, or restart the program.") );
		break;
	default:
		break;
	}
}

void MainWindow::variablesConfiguration()
{
	//FIXME
//    ChangeVariablesDialog dialog(outVariablesVector, refVariablesVector, this);
//    if(dialog.exec() == QDialog::Accepted){
//        // TODO
//    }
}

void MainWindow::onVariablesReady()
{
	enableVariableEditing = true;
	variablesAct->setEnabled(true);
	refControlPanelAct->setEnabled(true);
	newAct->setEnabled(true);

	QString fullMsg("Variables from DSP to PC loaded: ");
	const QList<QPair<QString, QString> > dspToPCVariables = AbstractVariableHandler::Instance()->getDSPtoPCVarList();
	const QList<QPair<QString, QString> > pcToDSPVariables = AbstractVariableHandler::Instance()->getPCtoDSPVarList();

	for(int i = 0; i < dspToPCVariables.size(); ++i)
	{
		fullMsg.append(dspToPCVariables.at(i).first).append(" type ").append(dspToPCVariables.at(i).second).append(";  ");
	}

	fullMsg.append("-- Variables from PC to DSP loaded: ");

	for(int i = 0; i < pcToDSPVariables.size(); ++i)
	{
		fullMsg.append(pcToDSPVariables.at(i).first).append(" type ").append(pcToDSPVariables.at(i).second).append(";  ");
	}

	statusBar()->showMessage(fullMsg);
}


bool MainWindow::loadVariablesDefFromFile()
{
	bool ok = false;
	QString fileName = QFileDialog::getOpenFileName(this,
																									"Open Configuration File",
																									QDir::currentPath(),
																									"CuCD Workspace Config File (*.cwcf)");
	if( !fileName.isEmpty() )
	{
		QSettings* settings = new QSettings(fileName, QSettings::IniFormat, this);
		AbstractVariableHandler::Instance()->loadVariablesFromFile(settings);

		ok = AbstractVariableHandler::Instance()->hasInstanceLoadedData();

		qDebug() << QString("File '%1' Loaded, check values.").arg(fileName);
	}
	return ok;
}


void MainWindow::onApplicationReadyToStart(bool ready)
{
	if( ready )
	{
		variablesAct->setEnabled(true);
		connect(variablesAct, SIGNAL(triggered()),
						this, SLOT(onPlayPauseUSBPressed()));
	}
	else
	{
		variablesAct->setEnabled(false);
		variablesAct->setIcon(QIcon(":/images/play_button.png"));
		disconnect(variablesAct, SIGNAL(triggered()),
							 this, SLOT(onPlayPauseUSBPressed()));
	}
	qDebug() << "Todo listo!... Buena suerte Edson. =)";
}


void MainWindow::onPlayPauseUSBPressed()
{
	if( cucdLibActive )
	{

	}
	else
	{
		cucdLibActive = true;
		//variablesAct->setIcon(QIcon(":/images/pause_button.png"));
		ComunicationController::Instance()->sendHeaders();
	}
}
