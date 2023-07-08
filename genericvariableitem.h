/*
 * genericvariableitem.h
 *
 *  Created on: 15-08-2014
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

#ifndef GENERICVARIABLEITEM_H
#define GENERICVARIABLEITEM_H

#include <QWidget>
#include <QStringList>

namespace Ui {
class GenericVariableItem;
}

class GenericVariableItem : public QWidget
{
  Q_OBJECT

public:
  explicit GenericVariableItem(QWidget* parent = 0);
  ~GenericVariableItem();

  QStringList    getVariableInfo();
  QString        getResumeVariableInfo();
  const QString& getMyID();

signals:
  void removeVariableItem(QString);

private slots:
  void onRemoveButtonPressed();

private:
  Ui::GenericVariableItem* _ui;
  QString                  _myID;
  QString                  _defaultName;
};

#endif // GENERICVARIABLEITEM_H
