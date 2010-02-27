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

#include "linkeditor.h"

#include <KLocale>

LinkEditor::LinkEditor( QWidget *parent )
  : KDialog( parent )
{
  setCaption( i18n("Add link") );
  setButtons( Ok | Cancel );
  setModal( true );

  m_urlEdit = new QLineEdit;
  setMainWidget( m_urlEdit );

  m_urlEdit->setFocus();
}

void LinkEditor::setLink( const Link &link )
{
  m_link = link;

  setCaption( i18n("Edit link") );

  m_urlEdit->setText( link.url() );
}

Link LinkEditor::link()
{
  m_link.setUrl( m_urlEdit->text() );

  return m_link;
}
