/*
 * communicationdatacontainer.cpp
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

#include "communicationdatacontainer.h"

CommunicationDataContainer::CommunicationDataContainer(bool incomingContainer):
  _incomingContainer(incomingContainer),
  _booleanVariables(QMap<QString, unsigned int>()),
  _integerVariables(QMap<QString, int>()),
  _doubleVariables(QMap<QString, double>()),
  _allVariables(QMap<QString, QList<double> >())
{
}


unsigned int CommunicationDataContainer::getBooleanVariable(const QString& varName) const
{
  if( _booleanVariables.contains(varName) )
  {
    return _booleanVariables[varName];
  }
  else
    return 0;
}


int CommunicationDataContainer::getIntegerVariable(const QString& varName) const
{
  if( _integerVariables.contains(varName) )
  {
    return _integerVariables[varName];
  }
  else
    return 0;
}


double CommunicationDataContainer::getDoubleVariable(const QString& varName) const
{
  if( _doubleVariables.contains(varName) )
  {
    return _doubleVariables[varName];
  }
  else
    return 0;
}


const QMap<QString, QList<double> >&
CommunicationDataContainer::getAllVariables() const
{
  return _allVariables;
}


bool CommunicationDataContainer::setBooleanVariable(const QString& varName, const unsigned int& value)
{
  bool result = false;

  if( _booleanVariables.contains(varName) )
  {
    _booleanVariables[varName] = value;

    _allVariables[varName].append(double(value));
    result = true;
  }

  return result;
}


bool CommunicationDataContainer::setIntegerVariable(const QString& varName, const int& value)
{
  bool result = false;

  if( _integerVariables.contains(varName) )
  {
    _integerVariables[varName] = value;

    _allVariables[varName].append(double(value));
    result = true;
  }

  return result;
}


bool CommunicationDataContainer::setDoubleVariable(const QString& varName, const double& value)
{
  bool result = false;

  if( _doubleVariables.contains(varName) )
  {
    _doubleVariables[varName] = value;

    _allVariables[varName].append(value);
    result = true;
  }

  return result;
}


void CommunicationDataContainer::initializeBooleanVariable(const QString& varName)
{
  if( !_booleanVariables.contains(varName) )
  {
    _booleanVariables[varName] = 0;

    _allVariables[varName] = QList<double>();
  }
}


void CommunicationDataContainer::initializeIntegerVariable(const QString& varName)
{
  if( !_integerVariables.contains(varName) )
  {
    _integerVariables[varName] = 0;

    _allVariables[varName] = QList<double>();
  }
}


void CommunicationDataContainer::initializeDoubleVariable(const QString& varName)
{
  if( !_doubleVariables.contains(varName) )
  {
    _doubleVariables[varName] = 0.0;

    _allVariables[varName] = QList<double>();
//    Enable following code for .CSV file export testing
//    _allVariables[varName].append(1);
//    _allVariables[varName].append(2);
//    _allVariables[varName].append(3);
//    _allVariables[varName].append(4);
//    _allVariables[varName].append(5);
//    End .CSV files tensting
  }
}


bool CommunicationDataContainer::isInconmigContainer() const
{
  return _incomingContainer;
}
