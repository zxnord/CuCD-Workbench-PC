/*
 * genericvariableitem.cpp
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

#include "genericvariableitem.h"
#include "ui_genericvariableitem.h"

static int DEFAULT_NAME_COUNTER = 1;
static int KEY_NAME_COUNTER = 0;

GenericVariableItem::GenericVariableItem(QWidget *parent) :
  QWidget(parent),
  _ui(new Ui::GenericVariableItem)
{
  _ui->setupUi(this);

  _myID = QString("VarID_%1").arg(KEY_NAME_COUNTER++);
  _defaultName = QString("variable_%1").arg(DEFAULT_NAME_COUNTER++);

  _ui->_nameLineEdit->setText(_defaultName);

  connect(_ui->_removeButton, SIGNAL(pressed()),
          this, SLOT(onRemoveButtonPressed()));
}


GenericVariableItem::~GenericVariableItem()
{
  delete _ui;
}


QStringList GenericVariableItem::getVariableInfo()
{
  QStringList variableInfoList;

  switch( _ui->_typeComboBox->currentIndex() )
  {
  case 0:
    variableInfoList << QString("uint32_t");
    break;
  case 1:
    variableInfoList << QString("int32_t");
    break;
  case 2:
    variableInfoList << QString("double");
    break;
  default:
    // Should never get here!!
    break;
  }

  if( _ui->_nameLineEdit->text().isEmpty() )
  {
    variableInfoList << _defaultName;
  }
  else
  {
    variableInfoList << _ui->_nameLineEdit->text();
  }

  return variableInfoList;
}

QString GenericVariableItem::getResumeVariableInfo()
{
  QStringList infoList = getVariableInfo();

  QString typeDefinition;

  if( infoList.at(0) == "uint32_t" )
  {
    typeDefinition = "On-Off";
  }
  else if( infoList.at(0) == "int32_t" )
  {
    typeDefinition = "Integer";
  }
  else
  {
    typeDefinition = "Double";
  }

  return QString("\t\t%1  %2\n").arg(typeDefinition, infoList.at(1));
}


void GenericVariableItem::onRemoveButtonPressed()
{
  emit removeVariableItem(_myID);
}

const QString& GenericVariableItem::getMyID()
{
  return _myID;
}
