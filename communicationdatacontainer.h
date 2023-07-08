/*
 * communicationdatacontainer.h
 *
 *  Created on: 16-08-2014
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

#ifndef COMMUNICATIONDATACONTAINER_H
#define COMMUNICATIONDATACONTAINER_H

#include <QMap>
#include <QString>

class CommunicationDataContainer
{
public:
  CommunicationDataContainer(bool incomingContainer = true);
  bool isInconmigContainer() const;

  unsigned int                         getBooleanVariable(const QString&) const;
  int                                  getIntegerVariable(const QString&) const;
  double                               getDoubleVariable(const QString&) const;
  const QMap<QString, QList<double> >& getAllVariables() const;

  bool setBooleanVariable(const QString&, const unsigned int&);
  bool setIntegerVariable(const QString&, const int&);
  bool setDoubleVariable(const QString&, const double&);

  void initializeBooleanVariable(const QString&);
  void initializeIntegerVariable(const QString&);
  void initializeDoubleVariable(const QString&);

private:
  bool                          _incomingContainer;
  QMap<QString, unsigned int>   _booleanVariables;
  QMap<QString, int>            _integerVariables;
  QMap<QString, double>         _doubleVariables;
  QMap<QString, QList<double> > _allVariables;
};

#endif // COMMUNICATIONDATACONTAINER_H
