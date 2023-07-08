/*
 * exportcsvdialog.h
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

#ifndef _EXPORT_CSV_DIALOG_H_
#define _EXPORT_CSV_DIALOG_H_

#include <QDialog>

namespace Ui {
class ExportCSVDialog;
}

class ExportCSVDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ExportCSVDialog(QWidget *parent = 0);
  ~ExportCSVDialog();

public slots:
  virtual void accept();

protected:
  void initAvailableVariables();
  void addTimeVariable(QMap<QString, QList<double> >&);

protected slots:
  void onAddButtonClicked();
  void onRemoveButtonClicked();
  void onTimeStampCBClicked();

private:
  Ui::ExportCSVDialog* _ui;
};

#endif // _EXPORT_CSV_DIALOG_H_
