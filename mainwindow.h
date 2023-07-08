#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include "plotmdiwindow.h"
#include "plotdialog.h"
#include "refcontrolpanel.h"
#include "mspcomdialog.h"
#include "qextserialport.h"
#include "referencevariable.h"
#include "changevariablesdialog.h"
#include "refdialog.h"
#include "referencemditemplate.h"
#include <QVector>
#include <QTimer>
#include "comunicationcontroller.h"
#include "comdefinitions.h"
#include "abstractvariablehandler.h"
#include "consolecommandwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public slots:
  void newPlot();
  void openFile();
  void addRefControlPanels();
  void connectOptions();
  void connectButtonAction();
  void variablesConfiguration();
  void onVariablesReady();
  void onApplicationReadyToStart(bool);
  void onPlayPauseUSBPressed();

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void createActions();
  void createToolBars();

private slots:
  void onExportingVariables();

private:
  bool loadVariablesDefFromFile();

  Ui::MainWindow *ui;
  QToolBar *fileToolBar;
  QToolBar *plotToolBar;
  QToolBar *controlToolBar;
  QToolBar *connectToolBar;
  QAction *newAct;
  QAction *openAct;
  QAction *saveAct;
  QAction *connectAct;
  QAction *connectSettingAct;
  QAction *refControlPanelAct;
  QAction *variablesAct;

  PlotMdiWindow *plotMdiWindow;

  int windowCounter;
  int referenceWindowCounter;
  bool enableConnectButton;
  bool connection;
  bool enableVariableEditing;
  bool cucdLibActive;
  QTimer sendDataTimer;
};

#endif // MAINWINDOW_H
