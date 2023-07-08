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

#ifndef VARIABLERANGESDIALOG_H
#define VARIABLERANGESDIALOG_H

#include <QDialog>

namespace Ui {
class VariableRangesDialog;
}

class VariableRangesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit VariableRangesDialog(QWidget *parent = 0);
  ~VariableRangesDialog();

  double getMinValue() const;
  double getMaxValue() const;
  double getStepValue() const;

private:
  Ui::VariableRangesDialog *ui;
};

#endif // VARIABLERANGESDIALOG_H
