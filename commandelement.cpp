/*
 * commandelement.cpp
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

#include "commandelement.h"
#include "ui_commandelement.h"

#include <QIntValidator>

#include "cucdutil.h"

CommandElement::CommandElement(const QPair<unsigned char, QByteArray>& cmd, bool enableCombo, QWidget* parent) :
  QWidget(parent),
  _ui(new Ui::CommandElement),
  _cmd(cmd)
{
  _ui->setupUi(this);

  if( _cmd.first == 0x01 )
  {
    _ui->_cmdComboBox->setCurrentIndex(0);
    _ui->_cmdComboBox->setEnabled(enableCombo);
    _ui->_argsLineEdit->setReadOnly(true);
    _ui->_argsLineEdit->clear();
  }
  else if( _cmd.first == 0x02 )
  {
    _ui->_cmdComboBox->setCurrentIndex(1);
    _ui->_argsLineEdit->setReadOnly(false);
    _ui->_argsLineEdit->setText(QString(_cmd.second));
    _ui->_argsLineEdit->setValidator(new QIntValidator(0, 0xFFFF, _ui->_argsLineEdit));
  }
  else if( _cmd.first == 0x03 )
  {
    _ui->_cmdComboBox->setCurrentIndex(2);
    _ui->_argsLineEdit->setReadOnly(true);
    _ui->_argsLineEdit->clear();
  }
  else if( _cmd.first == 0x04 )
  {
    _ui->_cmdComboBox->setCurrentIndex(4);
    _ui->_argsLineEdit->setReadOnly(true);
    _ui->_argsLineEdit->setText(QString(_cmd.second));
  }
  else // if( _cmd.first == 0x05 ) shouldn't be more.
  {
    _ui->_cmdComboBox->setCurrentIndex(3);
    _ui->_argsLineEdit->setReadOnly(true);
    _ui->_argsLineEdit->clear();
  }

  connect(_ui->_cmdComboBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(onComboBoxChanged(int)));
  connect(_ui->_argsLineEdit, SIGNAL(editingFinished()),
          this, SLOT(onLineEditChanged()));
  connect(_ui->_deleteButton, SIGNAL(pressed()),
          this, SLOT(onDeleteButtonPressed()));
}


CommandElement::~CommandElement()
{
  delete _ui;
}


void CommandElement::onComboBoxChanged(int index)
{
  _cmd.second = QByteArray();
  _ui->_argsLineEdit->setReadOnly(true);

  if( index == 1 )
  {
    _ui->_argsLineEdit->setText("1");
    _ui->_argsLineEdit->setReadOnly(false);

    _cmd.second = QString("1").toUtf8();
  }
  else if( index == 4 )
  {
    _ui->_cmdComboBox->blockSignals(true);
    _ui->_cmdComboBox->setCurrentIndex(0);
    _ui->_argsLineEdit->clear();
    _ui->_cmdComboBox->blockSignals(false);
    return;
  }
  else
  {
    _ui->_argsLineEdit->clear();
  }

  _cmd.first = (index == 0)? 0x01:
               (index == 1)? 0x02:
               (index == 2)? 0x03:
               (index == 3)? 0x05:
               (index == 4)? 0x04: 0x01;  //neither of these should never happen!

  emit cmdChanged();
}


void CommandElement::onLineEditChanged()
{
  if( _ui->_cmdComboBox->currentIndex() != 1 )
  {
    //should never happen =(
    return;
  }

//  _cmd.second = _ui->_argsLineEdit->text().toUtf8();
  _cmd.second = CuCDUtil::convertShortToHexStr(_ui->_argsLineEdit->text().toShort());

  emit cmdChanged();
}


void CommandElement::onDeleteButtonPressed()
{
  emit deleteCmd(this);
}


const QPair<unsigned char, QByteArray>& CommandElement::getCommandValue() const
{
  return _cmd;
}
