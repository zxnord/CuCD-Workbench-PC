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

#include "consolecommandwidget.h"
#include "ui_consolecommandwidget.h"

#include "comunicationcontroller.h"
#include "commandelement.h"

#include <QVBoxLayout>
#include <QPair>
#include <QByteArray>

ConsoleCommandWidget::ConsoleCommandWidget(QWidget *parent) :
  QWidget(parent),
  _ui(new Ui::ConsoleCommandWidget)
{
  _ui->setupUi(this);

  _layout = new QVBoxLayout(_ui->_commandsArea);
  _layout->setContentsMargins(5,5,5,5);
  _layout->setSpacing(5);

  populateCommandsArea();

  connect(ComunicationController::Instance(), SIGNAL(commandSent(QByteArray)),
          this, SLOT(onComunicationControllerSending(QByteArray)));

  connect(ComunicationController::Instance(), SIGNAL(newDataRawReceived(QByteArray)),
          this, SLOT(onComunicationControllerReceiving(QByteArray)));

  connect(ComunicationController::Instance(), SIGNAL(commandRegistered()),
          this, SLOT(onNewCommandRegistered()));

  connect(_ui->_addCommandButton, SIGNAL(pressed()),
          this, SLOT(onAddButtonPressed()));
}


ConsoleCommandWidget::~ConsoleCommandWidget()
{
  clearCommands();

  delete _ui;
}


void ConsoleCommandWidget::clearCommands()
{
  foreach(CommandElement* command, _commandsList)
  {
    if( command )
    {
      _layout->removeWidget(command);
      command->deleteLater();
    }
  }

  _commandsList.clear();

}


void ConsoleCommandWidget::onComunicationControllerSending(QByteArray msg)
{
  _ui->_usbUartConsole->appendPlainText(QString(msg) + "\n");

  onNewCommandRegistered();
}


void ConsoleCommandWidget::onComunicationControllerReceiving(const QByteArray& msg)
{
  _ui->_usbUartConsole->appendPlainText(QString(msg));
}


void ConsoleCommandWidget::removeOneElement(CommandElement* elem)
{
  if( _commandsList.size() > 1 )
  {
    _layout->removeWidget(elem);
    _commandsList.removeOne(elem);
    elem->deleteLater();

    QList<QPair<unsigned char,QByteArray> > cmdList;

    if( _commandsList.size() == 1 )
    {
      cmdList << QPair<unsigned char,QByteArray>(0x01, QByteArray());
    }
    else
    {
      foreach( CommandElement* elem, _commandsList )
      {
        cmdList << elem->getCommandValue();
      }
    }

    ComunicationController::Instance()->setRegisteredCommands(cmdList);
  }
}


void ConsoleCommandWidget::populateCommandsArea()
{
  const QList<QPair<unsigned char,QByteArray> > cmdList = ComunicationController::Instance()->getRegisteredCommands();

  for(int i = 0; i < cmdList.size(); ++i)
  {
    CommandElement* elem = new CommandElement(cmdList.at(i), !(i == 0), _ui->_commandsArea);

    connect(elem, SIGNAL(deleteCmd(CommandElement*)),
            this, SLOT(removeOneElement(CommandElement*)));
    connect(elem, SIGNAL(cmdChanged()),
            this, SLOT(onCommandUpdated()));

    _commandsList << elem;
    _layout->addWidget(elem);
  }
}

void ConsoleCommandWidget::onCommandUpdated()
{
  QList<QPair<unsigned char,QByteArray> > cmdList;

  foreach( CommandElement* elem, _commandsList )
  {
    cmdList << elem->getCommandValue();
  }

  ComunicationController::Instance()->setRegisteredCommands(cmdList);
}

void ConsoleCommandWidget::onNewCommandRegistered()
{
  clearCommands();

  populateCommandsArea();
}


void ConsoleCommandWidget::onAddButtonPressed()
{
  _commandsList << new CommandElement(QPair<unsigned char, QByteArray>(0x01, QByteArray()), _ui->_commandsArea);

  onCommandUpdated();
}
