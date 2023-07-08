/*
 * pctodspvariablewidget.h
 *
 *  Created on: 23-08-2014
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

#ifndef PCTODSPVARIABLEWIDGET_H
#define PCTODSPVARIABLEWIDGET_H

#include <QWidget>

class PCToDSPVariableWidget : public QWidget
{
  Q_OBJECT

public:
  explicit PCToDSPVariableWidget(const QString& varName, QWidget* parent = 0):
    QWidget(parent), myRepVariable(varName)
  {}

  const QString& getRepVariableName(){ return myRepVariable; }

protected:
  const QString myRepVariable;
};

#endif // PCTODSPVARIABLEWIDGET_H
