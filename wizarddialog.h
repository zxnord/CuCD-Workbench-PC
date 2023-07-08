/*
 * wizarddialog.h
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

#ifndef WIZARDDIALOG_H
#define WIZARDDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QList>
#include <QSettings>

namespace Ui {
class WizardDialog;
}

class GenericVariableItem;
class QByteArray;

class WizardDialog : public QDialog
{
  Q_OBJECT

public:
  explicit WizardDialog(QWidget *parent = 0);
  ~WizardDialog();

public slots:
  void onNextButtonPressed();
  void onBackButtonPressed();
  void onLoadSaveFilePressed();
  void onAddPCToDSPVarPressed();
  void onAddDSPToPCVarPressed();
  void onRemoveVariablePressed(QString);

private:
  void       setupConnections();
  void       prepareResumeInformation();
  void       saveCurrentConfiguration();
  void       saveCuCDHeaderFile(const QString&);
  QByteArray loadTemplateHeaderFileToList();
  void       addHeaderConstants(QByteArray&);
  void       addHeaderDefines(QByteArray&);
  void       createSettingsData(const QString&);
  void       setHandlerInstanceData();

  Ui::WizardDialog*           _ui;
  QList<GenericVariableItem*> _pcToDSPVariablesList;
  QList<GenericVariableItem*> _dspToPCVariablesList;
  QVBoxLayout*                _dspToPCListLayout;
  QVBoxLayout*                _pcToDSPListLayout;
  QSettings*                  _resumeData;
};

#endif // WIZARDDIALOG_H
