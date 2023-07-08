/*
 * doublewidget.cpp
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

#include "doublewidget.h"
#include "ui_doublewidget.h"

#include "abstractvariablehandler.h"
#include "variablerangesdialog.h"

#include <QMenu>

DoubleWidget::DoubleWidget(const QString &varName, QWidget *parent) :
  PCToDSPVariableWidget(varName, parent),
  ui(new Ui::DoubleWidget)
{
  ui->setupUi(this);
  initUI();
  initConnects();
}


DoubleWidget::~DoubleWidget()
{
  delete ui;
}


void DoubleWidget::initUI()
{
  setContextMenuPolicy(Qt::CustomContextMenu);

  _minValue = 0.0;
  _maxValue = 100.0;
  ui->nameLabel->setText(myRepVariable);
  ui->doubleSpinBox->setValue(0.0);
  ui->doubleSpinBox->setMinimum(_minValue);
  ui->doubleSpinBox->setMaximum(_maxValue);
  ui->doubleVerticalSlider->setValue(0);
  ui->doubleVerticalSlider->setMinimum(0);
  ui->doubleVerticalSlider->setMaximum(100);
  ui->enableCheckBox->setChecked(false);
}


void DoubleWidget::initConnects()
{
  connect(ui->doubleVerticalSlider, SIGNAL(sliderReleased()), this, SLOT(onSliderReleased()));
  connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onSpinBoxEdited(double)));
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(showContextMenu(const QPoint&)));
}


void DoubleWidget::onSliderReleased()
{
  double value = _maxValue - ( _maxValue - _minValue ) *
                 (-0.01*ui->doubleVerticalSlider->value() + 1);

  ui->doubleSpinBox->blockSignals(true);
  ui->doubleSpinBox->setValue(value);

  if( ui->enableCheckBox->isChecked() )
    AbstractVariableHandler::Instance()->updateDoubleVariable(myRepVariable, value);

  ui->doubleSpinBox->blockSignals(false);
}


void DoubleWidget::onSpinBoxEdited(double newValue)
{
  int value = 100 * (1 - (_maxValue - newValue)/(_maxValue - _minValue));

  ui->doubleVerticalSlider->blockSignals(true);
  ui->doubleVerticalSlider->setValue(value);

  if( ui->enableCheckBox->isChecked() )
    AbstractVariableHandler::Instance()->updateDoubleVariable(myRepVariable, newValue);

  ui->doubleVerticalSlider->blockSignals(false);
}


void DoubleWidget::showContextMenu(const QPoint& pos)
{
  QPoint globalPos = mapToGlobal(pos);

  QMenu myMenu;
  myMenu.addAction("Set Variable Ranges...");

  if( myMenu.exec(globalPos) )
  {
    VariableRangesDialog dialog(this);
    if( dialog.exec() == QDialog::Accepted )
    {
      _minValue = dialog.getMinValue();
      _maxValue = dialog.getMaxValue();
      ui->doubleSpinBox->setMinimum(_minValue);
      ui->doubleSpinBox->setMaximum(_maxValue);
      ui->doubleSpinBox->setSingleStep(dialog.getStepValue());
    }
  }
}
