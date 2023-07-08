/*
 * refcontrolpanel.h
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

#ifndef REFCONTROLPANEL_H
#define REFCONTROLPANEL_H

#include <QWidget>

namespace Ui {
class RefControlPanel;
}

class PCToDSPVariableWidget;
class QHBoxLayout;

class RefControlPanel : public QWidget
{
  Q_OBJECT

public:
  enum RefVariableType {
    REF_VAR_NONE = -1,
    REF_VAR_ONOFF,
    REF_VAR_INT,
    REF_VAR_DOUBLE
  };

  explicit RefControlPanel(const QString& varName, RefVariableType varType, QWidget *parent = 0);
  ~RefControlPanel();

  const QString& getRefVariableName() const;

private:
  void initUI(const QString&);

  Ui::RefControlPanel*   _ui;
  PCToDSPVariableWidget* _refVarWidget;
  RefVariableType        _varType;
  QHBoxLayout*           _layout;
};

#endif // REFCONTROLPANEL_H
