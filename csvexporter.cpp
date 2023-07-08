/*
 * csvexporter.c
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

#include "csvexporter.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

CSVExporter::CSVExporter(const QMap<QString, QList<double> >& exportVars, QObject *parent) :
  QObject(parent),
  _variablesMap(exportVars)
{
}


void CSVExporter::ExportVariables(const QMap<QString, QList<double> > &exportVars, bool forceExport)
{
  CSVExporter exporter(exportVars);

  if( exporter.checkMap() || forceExport)
  {
    QString fileName =
      QFileDialog::getSaveFileName(
        0,
        QString("Exporting Data To File"),
        QString("data.csv"),
        QString("*.csv"));

    if( !fileName.isEmpty() )
    {
      exporter.exportDataToFile(fileName);
    }
  }
  else
  {
    QMessageBox::warning(0,QString("Exporting Error!"), QString("Data is corrupted, could not be saved."));
  }
}


bool CSVExporter::checkMap()
{
  bool mapIsValid = true;

  // all lenghts should be equal. So, lets assume that first one is Ok.
  int mapSize = _variablesMap.begin().value().size();

  QMap<QString, QList<double> >::const_iterator it;

  for(it = _variablesMap.begin(); it != _variablesMap.end(); ++it)
  {
    if( it.value().size() != mapSize )
    {
      mapIsValid = false;
      break;
    }
  }

  return mapIsValid;
}


void CSVExporter::exportDataToFile(const QString& fileName)
{
  QFile data(fileName);
  if(data.open(QFile::WriteOnly | QFile::Truncate))
  {
    QTextStream output(&data);

    for(int i = 0; i < _variablesMap.keys().size(); ++i)
    {
      output << _variablesMap.keys().at(i);

      if( i < (_variablesMap.keys().size() - 1) )
        output << ",";
      else
        output << "\n";

    }

    int mapSize = _variablesMap.begin().value().size();
    int valueIt = 0;
    QMap<QString, QList<double> >::const_iterator it;

    while( valueIt < mapSize )
    {
      int localIter = 0;

      for(it = _variablesMap.begin(); it != _variablesMap.end(); ++it)
      {
        output << it.value().at(valueIt);

        if( localIter < (_variablesMap.keys().size() - 1) )
          output << ",";
        else
          output << "\n";

        ++localIter;
      }
      ++valueIt;
    }

    data.close();
  }
}
