/*
 * exportcsvdialog.cpp
 *
 *  Created on: 28-12-2014
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

#include "exportcsvdialog.h"
#include "ui_exportcsvdialog.h"

#include "abstractvariablehandler.h"
#include "communicationdatacontainer.h"
#include "csvexporter.h"

#include <QDoubleValidator>
#include <QList>
#include <QPair>

ExportCSVDialog::ExportCSVDialog(QWidget *parent) :
  QDialog(parent),
  _ui(new Ui::ExportCSVDialog)
{
  _ui->setupUi(this);

  QDoubleValidator* validStep = new QDoubleValidator(this);
  validStep->setBottom(0);
  validStep->setDecimals(5);
  _ui->timeStampLE->setValidator(validStep);

  initAvailableVariables();
  _ui->selectedVariables->clear();

  _ui->availableVariables->setSelectionMode(QListWidget::SingleSelection);
  _ui->selectedVariables->setSelectionMode(QListWidget::SingleSelection);

  connect(_ui->addButton, SIGNAL(clicked()),
          this, SLOT(onAddButtonClicked()));

  connect(_ui->removeButton, SIGNAL(clicked()),
          this, SLOT(onRemoveButtonClicked()));

  connect(_ui->enableTimeVarCB, SIGNAL(clicked()),
          this, SLOT(onTimeStampCBClicked()));
}


ExportCSVDialog::~ExportCSVDialog()
{
  delete _ui;
}


void ExportCSVDialog::initAvailableVariables()
{
  const QList<QPair<QString, QString> > comVariables =
      AbstractVariableHandler::Instance()->getDSPtoPCVarList();

  for( int i = 0; i < comVariables.size(); ++i )
  {
    _ui->availableVariables->addItem(comVariables.at(i).first);
  }

  if( comVariables.size() > 0 )
    _ui->addButton->setEnabled(true);
}


void ExportCSVDialog::onAddButtonClicked()
{
  for( int i = _ui->availableVariables->count() - 1; i >= 0 ; --i )
  {
    if( _ui->availableVariables->item(i)->isSelected() )
    {
      QListWidgetItem* selItem = _ui->availableVariables->takeItem(i);
      selItem->setSelected(false);
      _ui->selectedVariables->addItem(selItem);

      _ui->removeButton->setEnabled(true);
    }
  }

  _ui->availableVariables->clearSelection();

  if( _ui->availableVariables->count() == 0 )
    _ui->addButton->setEnabled(false);
}


void ExportCSVDialog::onRemoveButtonClicked()
{
  for( int i = _ui->selectedVariables->count() - 1; i >= 0 ; --i )
  {
    if( _ui->selectedVariables->item(i)->isSelected() )
    {
      QListWidgetItem* selItem = _ui->selectedVariables->takeItem(i);
      selItem->setSelected(false);
      _ui->availableVariables->addItem(selItem);

      _ui->addButton->setEnabled(true);
    }
  }

  _ui->selectedVariables->clearSelection();

  if( _ui->selectedVariables->count() == 0 )
    _ui->removeButton->setEnabled(false);
}


void ExportCSVDialog::accept()
{
  QMap<QString, QList<double> > allVariables =
    AbstractVariableHandler::Instance()->getDSPtoPCVarContainer()->getAllVariables();

  if( _ui->selectedVariables->count() > 0 )
  {
    QStringList mapKeys = allVariables.keys();

    for( int i = 0; i < _ui->selectedVariables->count(); ++i )
    {
      if( mapKeys.contains(_ui->selectedVariables->item(i)->text()) )
      {
        mapKeys.removeOne(_ui->selectedVariables->item(i)->text());
      }
    }


    for( int i = 0; i < mapKeys.count(); ++i )
    {
      allVariables.remove(mapKeys.at(i));
    }

    addTimeVariable(allVariables);

    CSVExporter::ExportVariables(allVariables, _ui->forceExportCB->isChecked());
    this->close();
  }
}


void ExportCSVDialog::addTimeVariable(QMap<QString, QList<double> >& map)
{
  if( _ui->enableTimeVarCB->isChecked() )
  {
    map[QString("time")] = QList<double>();

    for( int i = 0; i < map.begin().value().size(); ++i )
    {
      map[QString("time")].append((i+1)*_ui->timeStampLE->text().toDouble());
    }
  }
}


void ExportCSVDialog::onTimeStampCBClicked()
{
  if( _ui->enableTimeVarCB->isChecked() )
  {
    _ui->timeStampLE->setEnabled(true);
  }
  else
  {
    _ui->timeStampLE->setEnabled(false);
  }
}
