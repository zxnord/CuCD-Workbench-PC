/*
 * wizarddialog.cpp
 *
 *  Created on: 15-08-2014
 *      Author: Edson Contreras
 *
 * <<Copyright 2013,2014 Edson Contreras>>
 *
 * This file is part of Contreras USB Communication Device (CUCD) library.
 *
 * CUCD library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CUCD library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CUCD library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "wizarddialog.h"
#include "ui_wizarddialog.h"
#include "genericvariableitem.h"
#include "abstractvariablehandler.h"

#include <QFile>
#include <QByteArray>
#include <QFileDialog>
#include <QDebug>

WizardDialog::WizardDialog(QWidget *parent) :
  QDialog(parent),
  _ui(new Ui::WizardDialog)
{
  _ui->setupUi(this);
  setWindowTitle("CuCD Variable Creation Wizard");

  setupConnections();

  _resumeData = NULL;

  _ui->_backButton->setVisible(false);

  _dspToPCListLayout = new QVBoxLayout();
  _dspToPCListLayout->setContentsMargins(0,0,0,0);
  _ui->_dspToPCListWidget->setLayout(_dspToPCListLayout);
  _pcToDSPListLayout = new QVBoxLayout();
  _pcToDSPListLayout->setContentsMargins(0,0,0,0);
  _ui->_pcToDSPListWidget->setLayout(_pcToDSPListLayout);
}

WizardDialog::~WizardDialog()
{
  delete _ui;

  while( _dspToPCVariablesList.size() > 0 )
  {
    GenericVariableItem* item = _dspToPCVariablesList.takeFirst();

    if( item )
      delete item;
  }

  while( _pcToDSPVariablesList.size() > 0 )
  {
    GenericVariableItem* item = _pcToDSPVariablesList.takeFirst();

    if( item )
      delete item;
  }

  if( _dspToPCListLayout )
    delete _dspToPCListLayout;

  if( _pcToDSPListLayout )
    delete _pcToDSPListLayout;

  if( _resumeData )
    delete _resumeData;
}


void WizardDialog::setupConnections()
{
  connect(_ui->_backButton, SIGNAL(pressed()),
          this, SLOT(onBackButtonPressed()));
  connect(_ui->_nextButton, SIGNAL(pressed()),
          this, SLOT(onNextButtonPressed()));
  connect(_ui->_loadSaveButton, SIGNAL(pressed()),
          this, SLOT(onLoadSaveFilePressed()));

  connect(_ui->_addDSPToPCButton, SIGNAL(pressed()),
          this, SLOT(onAddDSPToPCVarPressed()));
  connect(_ui->_addPCToDSPButton, SIGNAL(pressed()),
          this, SLOT(onAddPCToDSPVarPressed()));
}


void WizardDialog::onAddDSPToPCVarPressed()
{
  GenericVariableItem* newItem = new GenericVariableItem();
  _dspToPCVariablesList << newItem;

  connect(newItem, SIGNAL(removeVariableItem(QString)),
          this, SLOT(onRemoveVariablePressed(QString)));

  _dspToPCListLayout->addWidget(newItem);
}


void WizardDialog::onAddPCToDSPVarPressed()
{
  GenericVariableItem* newItem = new GenericVariableItem();
  _pcToDSPVariablesList << newItem;

  connect(newItem, SIGNAL(removeVariableItem(QString)),
          this, SLOT(onRemoveVariablePressed(QString)));

  _pcToDSPListLayout->addWidget(newItem);
}


void WizardDialog::onRemoveVariablePressed(QString varID)
{
  int removeIndex = -1;
  int tempIndex = 0;

  foreach( GenericVariableItem* item, _dspToPCVariablesList )
  {
    if( item->getMyID() == varID )
    {
      removeIndex = tempIndex;
      break;
    }
    ++tempIndex;
  }

  if( removeIndex >= 0 )
  {
    GenericVariableItem* itemToRemove = _dspToPCVariablesList.takeAt(removeIndex);
    _dspToPCListLayout->removeWidget(itemToRemove);

    if( itemToRemove )
      delete itemToRemove;
  }
  else
  {
    tempIndex = 0;
    foreach( GenericVariableItem* item, _pcToDSPVariablesList )
    {
      if( item->getMyID() == varID )
      {
        removeIndex = tempIndex;
        break;
      }
      ++tempIndex;
    }
    if( removeIndex >= 0 )
    {
      GenericVariableItem* itemToRemove = _pcToDSPVariablesList.takeAt(removeIndex);
      _pcToDSPListLayout->removeWidget(itemToRemove);

      if( itemToRemove )
        delete itemToRemove;
    }
  }
}


void WizardDialog::onNextButtonPressed()
{
  switch( _ui->_wizardStackedWidget->currentIndex() )
  {
  case 0:
    _ui->_backButton->setVisible(true);
    _ui->_loadSaveButton->setVisible(false);
    _ui->_wizardStackedWidget->setCurrentIndex(1);
    break;
  case 1:
    _ui->_wizardStackedWidget->setCurrentIndex(2);
    break;
  case 2:
    _ui->_nextButton->setText("Finish");
    prepareResumeInformation();
    _ui->_wizardStackedWidget->setCurrentIndex(3);
    break;
  case 3:
    saveCurrentConfiguration();
    setHandlerInstanceData();
    break;
  default:
    //Should never get here!!
    break;
  }
}


void WizardDialog::onBackButtonPressed()
{
  switch( _ui->_wizardStackedWidget->currentIndex() )
  {
  case 0:
    //Should never get here!
    break;
  case 1:
    _ui->_loadSaveButton->setVisible(true);
    _ui->_backButton->setVisible(false);
    _ui->_wizardStackedWidget->setCurrentIndex(0);
    break;
  case 2:
    _ui->_wizardStackedWidget->setCurrentIndex(1);
    break;
  case 3:
    _ui->_nextButton->setText("Next");
    _ui->_wizardStackedWidget->setCurrentIndex(2);
    break;
  default:
    //Should never get here!
    break;
  }
}


void WizardDialog::onLoadSaveFilePressed()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  "Load Variable Configuration File",
                                                  "cucd_workspace",
                                                  "CuCD Workspace Configuration File (*.cwcf)");
  if( !fileName.isEmpty() )
  {
    qDebug() << "Configuration file name: " << fileName;
    if( _resumeData )
      delete _resumeData;

    _resumeData = new QSettings(fileName, QSettings::IniFormat, this);
    setHandlerInstanceData();
  }
}


void WizardDialog::prepareResumeInformation()
{
  _ui->_resumeTextEdit->clear();

  QString resumeMsg("Displaying resulting information:\n\n");

  QString dspToPCHeader("DSP to PC variables:\n");
  foreach( GenericVariableItem* item, _dspToPCVariablesList )
  {
    dspToPCHeader += item->getResumeVariableInfo();
  }

  QString pcToDSPHeader("\nPC to DSP variables:\n");
  foreach( GenericVariableItem* item, _pcToDSPVariablesList )
  {
    pcToDSPHeader += item->getResumeVariableInfo();
  }

  resumeMsg += dspToPCHeader + pcToDSPHeader;
  _ui->_resumeTextEdit->setText(resumeMsg);
}


void WizardDialog::saveCurrentConfiguration()
{
  QString fileName = QFileDialog::getSaveFileName(this,
                                                  "Save Variable Header File",
                                                  "cucd_workspace",
                                                  "CuCD C Header Description (*.h)");

  if( !fileName.isEmpty() )
  {
    qDebug() << "Workspace file name: " << fileName;
    saveCuCDHeaderFile(fileName);

    fileName = QFileDialog::getSaveFileName(this,
                                            "Save Variable Configuration File",
                                            "cucd_workspace",
                                            "CuCD Workspace Configuration File (*.cwcf)");
    if( !fileName.isEmpty() )
    {
      qDebug() << "Configuration file name: " << fileName;
      createSettingsData(fileName);
    }
  }
}


void WizardDialog::saveCuCDHeaderFile(const QString& fileName)
{
  QFile cucdHeaderFile(fileName);
  QByteArray headerFileRaw = loadTemplateHeaderFileToList();

  if( cucdHeaderFile.open(QFile::WriteOnly | QFile::Text) )
  {
    cucdHeaderFile.write(headerFileRaw);
    cucdHeaderFile.close();
  }
}


QByteArray WizardDialog::loadTemplateHeaderFileToList()
{
  const int numberOfLinesInHeader   = 48;
  const int definesLine             = 6;
  const int pcToDSPDefinitionsLine  = 12;
  const int dspToPCDefinitionsLine  = 8;
  const int constantsDefinitionLine = 36;

  QFile templateHeaderFile(":/images/cucd_workspace.template");
  QByteArray fileAsByteArray;

  if( templateHeaderFile.open(QFile::ReadOnly | QFile::Text) )
  {
    for( int i = 0; i < numberOfLinesInHeader; ++i )
    {
      fileAsByteArray.append(templateHeaderFile.readLine());

      if( i == pcToDSPDefinitionsLine )
      {
        foreach( GenericVariableItem* item, _pcToDSPVariablesList )
        {
          fileAsByteArray.append(QString("\t%1 %2\n")
                                 .arg(item->getVariableInfo().at(0), item->getVariableInfo().at(1)).toUtf8());
        }
        fileAsByteArray.append('\t');
      }
      else if( i == dspToPCDefinitionsLine )
      {
        foreach( GenericVariableItem* item, _dspToPCVariablesList )
        {
          fileAsByteArray.append(QString("\t%1 %2\n")
                                 .arg(item->getVariableInfo().at(0), item->getVariableInfo().at(1)).toUtf8());
        }
        fileAsByteArray.append('\t');
      }
      else if( i == constantsDefinitionLine )
      {
        addHeaderConstants(fileAsByteArray);
      }
      else if( i == definesLine )
      {
        addHeaderDefines(fileAsByteArray);
      }
    }

    templateHeaderFile.close();
  }

  return fileAsByteArray;
}


void WizardDialog::addHeaderDefines(QByteArray& headerFileRaw)
{
  int variableSum = 0;
  foreach( GenericVariableItem* item, _pcToDSPVariablesList )
  {
    if( item->getVariableInfo().at(0) == "uint32_t" )
    {
      variableSum += 4;
    }
    else if( item->getVariableInfo().at(0) == "int32_t" )
    {
      variableSum += 4;
    }
    else if( item->getVariableInfo().at(0) == "double" )
    {
      variableSum += 8;
    }
  }
  // Include the checksum!
  variableSum += 4;

  QString tempStr = QString("#define  CUCD_IN_VARIABLES_SIZE   %1\n").arg(variableSum);
  headerFileRaw.append(tempStr.toUtf8());
  variableSum = 0;

  foreach( GenericVariableItem* item, _dspToPCVariablesList )
  {
    if( item->getVariableInfo().at(0) == "uint32_t" )
    {
      variableSum += 4;
    }
    else if( item->getVariableInfo().at(0) == "int32_t" )
    {
      variableSum += 4;
    }
    else if( item->getVariableInfo().at(0) == "double" )
    {
      variableSum += 8;
    }
  }
  // Include the checksum!
  variableSum += 4;

  tempStr = QString("#define  CUCD_OUT_VARIABLES_SIZE  %1\n").arg(variableSum);
  headerFileRaw.append(tempStr.toUtf8());
}


void WizardDialog::addHeaderConstants(QByteArray& headerFileRaw)
{
  QString tempStr1 = QString("const uint8_t   CUCD_IN_VARIABLES_NUM            = %1;\n")
      .arg(_pcToDSPVariablesList.size());
  QString tempStr2 = QString("const uint8_t   CUCD_OUT_VARIABLES_NUM           = %1;\n")
      .arg(_dspToPCVariablesList.size());

  headerFileRaw.append(tempStr1.toUtf8());
  headerFileRaw.append(tempStr2.toUtf8());

  tempStr1 = QString("{");
  tempStr2.clear();

  foreach( GenericVariableItem* item, _pcToDSPVariablesList )
  {
    if( item->getVariableInfo().at(0) == "uint32_t" )
    {
      tempStr1.append("UINT_SIZE, ");
    }
    else if( item->getVariableInfo().at(0) == "int32_t" )
    {
      tempStr1.append("INT_SIZE, ");
    }
    else if( item->getVariableInfo().at(0) == "double" )
    {
      tempStr1.append("DOUBLE_SIZE, ");
    }
  }

  // CheckSum must be added too!
  tempStr1.append("UINT_SIZE};\n");

  tempStr2 = QString("{");

  foreach( GenericVariableItem* item, _dspToPCVariablesList )
  {
    if( item->getVariableInfo().at(0) == "uint32_t" )
    {
      tempStr2.append("UINT_SIZE, ");
    }
    else if( item->getVariableInfo().at(0) == "int32_t" )
    {
      tempStr2.append("INT_SIZE, ");
    }
    else if( item->getVariableInfo().at(0) == "double" )
    {
      tempStr2.append("DOUBLE_SIZE, ");
    }
  }

  // CheckSum must be added too!
  tempStr2.append("UINT_SIZE};\n");

  headerFileRaw.append(QString("const DataSizes CUCD_INSIZEARRAY[]               = ").append(tempStr1).toUtf8());
  headerFileRaw.append(QString("const DataSizes CUCD_OUTSIZEARRAY[]              = ").append(tempStr2).toUtf8());
}


void WizardDialog::createSettingsData(const QString& fileName)
{
  int iter = 1;
  _resumeData = new QSettings(fileName, QSettings::IniFormat, this);
  _resumeData->clear();

  _resumeData->beginGroup("PCToDSPData");
  _resumeData->setValue("nofVariables", _pcToDSPVariablesList.size());
  foreach( GenericVariableItem* item, _pcToDSPVariablesList )
  {
    _resumeData->setValue(QString("variable_%1_type").arg(iter), item->getVariableInfo().at(0));
    _resumeData->setValue(QString("variable_%1_name").arg(iter), item->getVariableInfo().at(1));
    ++iter;
  }
  _resumeData->endGroup();
  iter = 1;

  _resumeData->beginGroup("DSPToPCData");
  _resumeData->setValue("nofVariables", _dspToPCVariablesList.size());
  foreach( GenericVariableItem* item, _dspToPCVariablesList )
  {
    _resumeData->setValue(QString("variable_%1_type").arg(iter), item->getVariableInfo().at(0));
    _resumeData->setValue(QString("variable_%1_name").arg(iter), item->getVariableInfo().at(1));
    ++iter;
  }
  _resumeData->endGroup();
}


void WizardDialog::setHandlerInstanceData()
{
  if( _resumeData == NULL )
  {
    qDebug() << "Error creating QSettings data. Restart application";
    exit(-1);
  }

  AbstractVariableHandler::Instance()->loadVariablesFromFile(_resumeData);

  if( AbstractVariableHandler::Instance()->hasInstanceLoadedData() )
    this->close();
}
