/*
 * interruptorwidget.cpp
 *
 *  Created on: 22-02-2014
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

#include "interruptorwidget.h"
#include "ui_interruptorwidget.h"

#include "abstractvariablehandler.h"

InterruptorWidget::InterruptorWidget(const QString &varName, QWidget *parent) :
  PCToDSPVariableWidget(varName, parent),
  ui(new Ui::InterruptorWidget)
{
  ui->setupUi(this);
  initUI();
  connect(ui->boolVarCheckBox, SIGNAL(clicked(bool)), this, SLOT(updateValue(bool)));
}

InterruptorWidget::~InterruptorWidget()
{
  delete ui;
}

void InterruptorWidget::initUI()
{
  ui->nameLabel->setText(myRepVariable);
  ui->boolVarCheckBox->setChecked(false);
}

void InterruptorWidget::updateValue(bool value)
{
  AbstractVariableHandler::Instance()->updateBooleanVariable(myRepVariable, value? 0x50505050: 0x00000000);
}
