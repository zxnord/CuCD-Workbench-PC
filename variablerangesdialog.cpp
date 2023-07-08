/*
 * cucd_i2c_exe_cmd.c
 *
 *  Created on: 19-06-2014
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

#include "variablerangesdialog.h"
#include "ui_variablerangesdialog.h"

#include <QDoubleValidator>

VariableRangesDialog::VariableRangesDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::VariableRangesDialog)
{
  ui->setupUi(this);

  QDoubleValidator* validMinMax = new QDoubleValidator(this);
  validMinMax->setDecimals(2);
  ui->_minLineEdit->setValidator(validMinMax);
  ui->_maxLineEdit->setValidator(validMinMax);

  QDoubleValidator* validStep = new QDoubleValidator(this);
  validStep->setBottom(0);
  validStep->setDecimals(5);
  ui->_stepLineEdit->setValidator(validStep);
}


VariableRangesDialog::~VariableRangesDialog()
{
  delete ui;
}


double VariableRangesDialog::getMinValue() const
{
  return ui->_minLineEdit->text().toDouble();
}


double VariableRangesDialog::getMaxValue() const
{
  return ui->_maxLineEdit->text().toDouble();
}


double VariableRangesDialog::getStepValue() const
{
  return ui->_stepLineEdit->text().toDouble();
}
