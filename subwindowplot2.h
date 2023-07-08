/*
 * subwindowplot2.h
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

#ifndef SUBWINDOWPLOT2_H
#define SUBWINDOWPLOT2_H

#include <QWidget>
#include <QColor>
#include <QMap>
#include <QList>

namespace Ui {
class SubWindowPlot2;
}

class SubWindowPlot2 : public QWidget
{
    Q_OBJECT
    
public slots:
    void realtimeDataSlot(const QMap<QString, QList<double> >&);

public:
    explicit SubWindowPlot2(QMap<QString, QColor> plotingVariables, double timeStamp, QWidget *parent = 0);
    ~SubWindowPlot2();
    
private slots:
    void doQuickExport();

private:
    void initPlotVars(const QList<QColor>& );
    void updateValues(const QMap<QString, QList<double> >& );

    Ui::SubWindowPlot2*           _ui;
    QStringList                   _variableNames;
    double                        _farTimeStamp;
    QVector<double>               _variableValue;
    double                        _lastFpsKey;
    int                           _frameCount;
    QMap<QString, QList<double> > _plotValues;
};

#endif // SUBWINDOWPLOT2_H
