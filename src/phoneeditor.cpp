/*
    This file is part of KDE.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/

#include "phoneeditor.h"

#include <KLocale>

PhoneEditor::PhoneEditor( QWidget *parent )
  : KDialog( parent )
{
  setCaption( i18n("Add phone number") );
  setButtons( Ok | Cancel );
  setModal( true );

  m_numberEdit = new QLineEdit;
  setMainWidget( m_numberEdit );

  m_numberEdit->setFocus();
}

void PhoneEditor::setPhone( const Polka::Phone &phone )
{
  m_phone = phone;

  setCaption( i18n("Edit phone number") );

  m_numberEdit->setText( phone.phoneNumber() );
}

Polka::Phone PhoneEditor::phone()
{
  m_phone.setPhoneNumber( m_numberEdit->text() );

  return m_phone;
}
