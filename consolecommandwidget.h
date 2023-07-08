/*
 * cucd_i2c_exe_cmd.c
 *
 *  Created on: 19-06-2014
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

#ifndef CONSOLECOMMANDWIDGET_H
#define CONSOLECOMMANDWIDGET_H

#include <QWidget>
#include <QList>

namespace Ui {
class ConsoleCommandWidget;
}

class QVBoxLayout;
class CommandElement;

class ConsoleCommandWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ConsoleCommandWidget(QWidget *parent = 0);
  ~ConsoleCommandWidget();

  void populateCommandsArea();

private slots:
  void onComunicationControllerReceiving(const QByteArray&);
  void onComunicationControllerSending(QByteArray);
  void removeOneElement(CommandElement*);
  void onCommandUpdated();
  void onNewCommandRegistered();
  void onAddButtonPressed();

private:
  void clearCommands();

  Ui::ConsoleCommandWidget* _ui;
  QVBoxLayout*              _layout;
  QList<CommandElement*>    _commandsList;
};

#endif // CONSOLECOMMANDWIDGET_H
