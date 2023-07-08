/*
 * plotmdiwindow.cpp
 *
 *  Created on: 15-12-2013
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

#include "plotmdiwindow.h"

#include <limits>

#include "abstractvariablehandler.h"
#include "communicationdatacontainer.h"

unsigned long long PLOT_DATA_COUNTER = 0;

PlotMdiWindow::PlotMdiWindow(QMap<QString, QColor> plotingVariables,
                             double                timeStamp,
                             QString               title,
                             QWidget*              parent) :
  QMdiSubWindow(parent),
  _variablesDescription(plotingVariables),
  _timeStamp(timeStamp)
{
  _plotWidget = new SubWindowPlot2(_variablesDescription, timeStamp, this);
  setWidget(_plotWidget);
  setWindowTitle(title);
  setWindowIcon(QIcon(":/images/icon-graph2.png"));
  setMinimumSize(300,300);
  initConnects();

  _variablesValues = QMap<QString, QList<double> >();
  QStringList keys = _variablesDescription.keys();

  foreach (QString key, keys)
  {
    _variablesValues[key] = QList<double>();
  }
  _variablesValues["Time"] = QList<double>();
}

PlotMdiWindow::~PlotMdiWindow()
{
  delete _plotWidget;
}

void PlotMdiWindow::initConnects()
{
  connect(this, SIGNAL(variablesReady(const QMap<QString, QList<double> >&)),
          _plotWidget, SLOT(realtimeDataSlot(const QMap<QString, QList<double> >&)));

  connect(AbstractVariableHandler::Instance(), SIGNAL(dataHasBeenUpdated()),
          this, SLOT(onVariablesUpdated()));
}

void PlotMdiWindow::onVariablesUpdated()
{
  QStringList mapKeys = _variablesDescription.keys();

  foreach( QString varName, mapKeys )
  {
    double newValue = std::numeric_limits<double>::quiet_NaN();

    const QList<QPair<QString, QString> > comVariables =
        AbstractVariableHandler::Instance()->getDSPtoPCVarList();

    for( int i = 0; i < comVariables.size(); ++i )
    {
      if( comVariables.at(i).first == varName )
      {
        if( comVariables.at(i).second == "uint32_t" )
        {
          newValue = AbstractVariableHandler::Instance()->
              getDSPtoPCVarContainer()->getBooleanVariable(varName);
        }
        else if( comVariables.at(i).second == "int32_t" )
        {
          newValue = AbstractVariableHandler::Instance()->
              getDSPtoPCVarContainer()->getIntegerVariable(varName);
        }
        else if( comVariables.at(i).second == "double" )
        {
          newValue = AbstractVariableHandler::Instance()->
              getDSPtoPCVarContainer()->getDoubleVariable(varName);
        }


        if( !(newValue != newValue) )
        {
          _variablesValues[varName].append(newValue);
        }
        else
          return;
      }
    }
  }

  _variablesValues["Time"].append(_timeStamp * PLOT_DATA_COUNTER++);

  emit variablesReady(_variablesValues);
}
