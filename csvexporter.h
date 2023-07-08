/*
 * csvexporter.h
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

#ifndef _CSV_EXPORTER_H_
#define _CSV_EXPORTER_H_

#include <QObject>
#include <QString>
#include <QMap>

class CSVExporter : public QObject
{
  Q_OBJECT
public:
  static void ExportVariables(const QMap<QString, QList<double> >& exportVars, bool forceExport = false);

signals:

public slots:

protected:
  CSVExporter(const QMap<QString, QList<double> >& exportVars, QObject *parent = 0);

  bool checkMap();
  void exportDataToFile(const QString& fileName);

  const QMap<QString, QList<double> > _variablesMap;
};

#endif // _CSV_EXPORTER_H_
