/*
 * commandelement.h
 *
 *  Created on: 23-08-2014
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

#ifndef COMMANDELEMENT_H
#define COMMANDELEMENT_H

#include <QWidget>
#include <QPair>

namespace Ui {
class CommandElement;
}

class CommandElement : public QWidget
{
  Q_OBJECT

public:
  explicit CommandElement(const QPair<unsigned char, QByteArray>& cmd, bool enableCombo = true, QWidget* parent = 0);
  ~CommandElement();

  const QPair<unsigned char, QByteArray>& getCommandValue() const;

signals:
  void cmdChanged();
  void deleteCmd(CommandElement*);

private slots:
  void onComboBoxChanged(int);
  void onLineEditChanged();
  void onDeleteButtonPressed();

private:
  Ui::CommandElement*              _ui;
  QPair<unsigned char, QByteArray> _cmd;
};

#endif // COMMANDELEMENT_H
