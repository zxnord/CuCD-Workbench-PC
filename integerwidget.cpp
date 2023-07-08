/*
 * integerwidget.cpp
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

#include "integerwidget.h"
#include "ui_integerwidget.h"

#include "abstractvariablehandler.h"
#include "variablerangesdialog.h"

#include <QMenu>
#include <QAction>

IntegerWidget::IntegerWidget(const QString& varName, QWidget *parent) :
  PCToDSPVariableWidget(varName, parent),
  ui(new Ui::IntegerWidget)
{
  ui->setupUi(this);

  initUI();
  initConnects();
}


IntegerWidget::~IntegerWidget()
{
  delete ui;
}


void IntegerWidget::initUI()
{
  setContextMenuPolicy(Qt::CustomContextMenu);

  ui->nameLabel->setText(myRepVariable);
  ui->intVarSlider->setValue(0);
  ui->intVarSpinBox->setValue(0);
  ui->enableCheckBox->setChecked(false);
}


void IntegerWidget::initConnects()
{
  connect(ui->intVarSlider, SIGNAL(sliderReleased()), this, SLOT(onSliderReleased()));
  connect(ui->intVarSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxEdited(int)));
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(showContextMenu(const QPoint&)));
}


void IntegerWidget::onSliderReleased()
{
  ui->intVarSpinBox->blockSignals(true);
  ui->intVarSpinBox->setValue(ui->intVarSlider->value());

  if( ui->enableCheckBox->isChecked() )
    AbstractVariableHandler::Instance()->updateIntegerVariable(myRepVariable, ui->intVarSlider->value());

  ui->intVarSpinBox->blockSignals(false);
}


void IntegerWidget::onSpinBoxEdited(int newValue)
{
  ui->intVarSlider->blockSignals(true);
  ui->intVarSlider->setValue(newValue);

  if( ui->enableCheckBox->isChecked() )
    AbstractVariableHandler::Instance()->updateIntegerVariable(myRepVariable, newValue);

  ui->intVarSlider->blockSignals(false);
}


void IntegerWidget::showContextMenu(const QPoint& pos)
{
  QPoint globalPos = mapToGlobal(pos);

  QMenu myMenu;
  myMenu.addAction("Set Variable Ranges...");

  if( myMenu.exec(globalPos) )
  {
    VariableRangesDialog dialog(this);
    if( dialog.exec() == QDialog::Accepted )
    {
      ui->intVarSlider->setMinimum(static_cast<int>(dialog.getMinValue()));
      ui->intVarSlider->setMaximum(static_cast<int>(dialog.getMaxValue()));
      ui->intVarSpinBox->setMinimum(static_cast<int>(dialog.getMinValue()));
      ui->intVarSpinBox->setMaximum(static_cast<int>(dialog.getMaxValue()));
      ui->intVarSpinBox->setSingleStep(static_cast<int>(dialog.getStepValue()));
    }
  }
}
