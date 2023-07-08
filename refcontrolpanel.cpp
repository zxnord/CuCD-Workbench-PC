/*
 * refcontrolpanel.cpp
 *
 *  Created on: 21-02-2014
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

#include "refcontrolpanel.h"
#include "ui_refcontrolpanel.h"

#include "interruptorwidget.h"
#include "integerwidget.h"
#include "doublewidget.h"

#include <QHBoxLayout>

RefControlPanel::RefControlPanel(const QString& varName, RefVariableType varType, QWidget *parent) :
  QWidget(parent),
  _varType(varType),
  _ui(new Ui::RefControlPanel)
{
  _ui->setupUi(this);

  _refVarWidget = NULL;
  initUI(varName);
}

RefControlPanel::~RefControlPanel()
{
  delete _ui;
}

void RefControlPanel::initUI(const QString& varName)
{
  _layout = new QHBoxLayout(_ui->frame);
  _layout->addStretch();

  switch( _varType )
  {
  case REF_VAR_ONOFF:
   _refVarWidget = new InterruptorWidget(varName, _ui->frame);
    break;
  case REF_VAR_INT:
    _refVarWidget = new IntegerWidget(varName, _ui->frame);
    break;
  case REF_VAR_DOUBLE:
    _refVarWidget = new DoubleWidget(varName, _ui->frame);
    break;
  default:
    return;
  }

  _layout->addWidget(_refVarWidget);
  _layout->addStretch();
}


const QString& RefControlPanel::getRefVariableName() const
{
  return _refVarWidget->getRepVariableName();
}
