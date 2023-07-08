/*
 * cucdutil.cpp
 *
 *  Created on: 17-08-2014
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

#include "cucdutil.h"
#include <QDebug>
#include <iomanip>
#include <sstream>
#include <string>

QByteArray CuCDUtil::convertShortToHexStr(const unsigned short& num)
{
  QString resultStr;
  std::stringstream sstream;

  sstream << std::hex << num;
  resultStr = QString::fromStdString(sstream.str());

  while( resultStr.size() < 4)
  {
    resultStr.prepend(QChar('0'));
  }

  qDebug() << "CuCDUtil: Converted " << num << "to " << QString::fromStdString(sstream.str());
  return resultStr.toUpper().toUtf8();
}


const char CuCDUtil::convertCharToHexStr(const unsigned char& num)
{
  std::stringstream sstream;

  // Only returns 1 char so supports only the numbers between 0x00 and 0x0F
  //clear the rest.
  unsigned char resultNum = num & 0x0F;

  sstream << std::hex << resultNum;
  qDebug() << "CuCDUtil: Converted " << num << "to " << QString::fromStdString(sstream.str()).at(0);

  return QString::fromStdString(sstream.str()).at(0).toAscii();
}


void CuCDUtilThread::sleep(unsigned long secs)
{
  qDebug() << "Sleeping " << secs << " secs.";
  QThread::sleep(secs);
}


void CuCDUtilThread::msleep(unsigned long msecs)
{
  qDebug() << "Sleeping " << msecs << " milisecs.";
  QThread::msleep(msecs);
}


void CuCDUtilThread::usleep(unsigned long usecs)
{
  qDebug() << "Sleeping " << usecs << " microsecs.";
  QThread::usleep(usecs);
}
