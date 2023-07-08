/*
 * plotmdiwindow.h
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

#ifndef PLOTMDIWINDOW_H
#define PLOTMDIWINDOW_H

#include <QMdiSubWindow>
#include <QIcon>
#include <QMap>
#include <QColor>
#include <QList>
#include "subwindowplot2.h"

class PlotMdiWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit PlotMdiWindow(QMap<QString, QColor> plotingVariables,
                           double timeStamp,
                           QString title,
                           QWidget *parent = 0);
    ~PlotMdiWindow();
    
signals:
    void variablesReady(const QMap<QString, QList<double> >&);
    
public slots:
    void onVariablesUpdated();

private:
    void initConnects();
    const QMap<QString, QColor>   _variablesDescription;
    double                        _timeStamp;
    SubWindowPlot2*               _plotWidget;
    QMap<QString, QList<double> > _variablesValues;
    
};

#endif // PLOTMDIWINDOW_H
