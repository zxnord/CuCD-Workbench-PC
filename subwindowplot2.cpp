/*
 * subwindowplot2.cpp
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

#include "subwindowplot2.h"
#include "ui_subwindowplot2.h"

#include "csvexporter.h"

#include "3rd-party/QCustomPlot/qcustomplot.h"

#include <QDebug>

SubWindowPlot2::SubWindowPlot2(QMap<QString, QColor> plotingVariables, double timeStamp, QWidget *parent) :
  QWidget(parent),
  _ui(new Ui::SubWindowPlot2),
  _farTimeStamp(timeStamp)
{
  _ui->setupUi(this);
  _ui->customPlot->replot();
  _ui->customPlot->setInteraction(QCP::iRangeDrag);
  _ui->customPlot->setInteraction(QCP::iRangeZoom);

  _plotValues[QString("time")] = QList<double>();
  _variableNames = plotingVariables.keys();

  foreach( QString varName, _variableNames )
  {
    _plotValues[varName] = QList<double>();
  }

  _lastFpsKey = 0.0;
  _frameCount = 0;

  connect(_ui->quickExportButton, SIGNAL(clicked()),
          this, SLOT(doQuickExport()));

  initPlotVars(plotingVariables.values());
}


SubWindowPlot2::~SubWindowPlot2()
{
  delete _ui;
}


void SubWindowPlot2::initPlotVars(const QList<QColor>& colorList)
{
  int i=0, lastIndexUsed;
  for(  ; i < colorList.size(); ++i )
  {
    _ui->customPlot->addGraph();
    _ui->customPlot->graph(i)->setPen(QPen(colorList.at(i)));
  }

  lastIndexUsed = i;

  for( i = 0; i < colorList.size(); ++i )
  {
    _ui->customPlot->addGraph();
    _ui->customPlot->graph(i+lastIndexUsed)->setPen(QPen(colorList.at(i)));

    _ui->customPlot->graph(i+lastIndexUsed)->setLineStyle(QCPGraph::lsNone);
    _ui->customPlot->graph(i+lastIndexUsed)->setScatterStyle(QCPScatterStyle::ssDisc);
  }

  _ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  _ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  _ui->customPlot->xAxis->setAutoTickStep(false);
  _ui->customPlot->xAxis->setTickStep(2);
  _ui->customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(_ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), _ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(_ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), _ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

  switch(_variableNames.size())
  {

  case 1:
    _ui->colorsLabel->setText(
      QString("Colour Legend: %1 %2")
      .arg(colorList.at(0).name())
      .arg(_variableNames.at(0)));
    break;

  case 2:
    _ui->colorsLabel->setText(
      QString("Colour Legend: %1 %2\t%3 %4")
      .arg(colorList.at(0).name())
      .arg(_variableNames.at(0))
      .arg(colorList.at(1).name())
      .arg(_variableNames.at(1)));
    break;

  case 3:
    _ui->colorsLabel->setText(
      QString("Colour Legend: %1 %2\t%3 %4\t%5 %6")
      .arg(colorList.at(0).name())
      .arg(_variableNames.at(0))
      .arg(colorList.at(1).name())
      .arg(_variableNames.at(1))
      .arg(colorList.at(2).name())
      .arg(_variableNames.at(2)));
    break;

  default:
    break;

  }
}


void SubWindowPlot2::realtimeDataSlot(const QMap<QString, QList<double> >& newValues)
{
  int i=0;
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
  updateValues(newValues);


  for(  ; i < _variableNames.size(); ++i )
  {
    _ui->customPlot->graph(i)->addData(key, _variableValue[i]);
    _ui->customPlot->graph(i)->rescaleValueAxis(true);
    _ui->customPlot->graph(i)->removeDataBefore(key-30);
  }
  for(  ; i < _variableNames.size()*2; ++i )
  {
    _ui->customPlot->graph(i)->clearData();
    _ui->customPlot->graph(i)->addData(key, _variableValue[i-_variableNames.size()]);
  }

    _ui->customPlot->graph(0)->rescaleValueAxis();
    _ui->customPlot->xAxis->setRange(key+0.25, 30, Qt::AlignRight);
    _ui->customPlot->replot();

    // calculate frames per second:
    ++_frameCount;
    if( (key - _lastFpsKey) > 2) // average fps over 2 seconds
    {
      _ui->fpsLabel->setText(
            QString("%1 FPS, Total Data points: %2")
            .arg(_frameCount/(key - _lastFpsKey), 0, 'f', 0)
            .arg(_ui->customPlot->graph(0)->data()->count()+_ui->customPlot->graph(1)->data()->count()));
      _lastFpsKey = key;
      _frameCount = 0;
    }
}


void SubWindowPlot2::updateValues(const QMap<QString, QList<double> >& newValues)
{
  static int receivedTimes = 0;
  _variableValue.clear();

  foreach( QString varName, _variableNames )
  {
    _variableValue.append( newValues.value(varName).last() );
    _plotValues[varName].append(newValues.value(varName).last());
  }

  _plotValues[QString("time")].append(receivedTimes++ * _farTimeStamp);
}


void SubWindowPlot2::doQuickExport()
{
  CSVExporter::ExportVariables(_plotValues);
}
