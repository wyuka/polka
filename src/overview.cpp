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

#include "overview.h"

#include <KLocale>

Overview::Overview()
{
  QBoxLayout *topLayout = new QHBoxLayout( this );

  topLayout->addStretch( 1 );

  QBoxLayout *buttonLayout = new QVBoxLayout;
  topLayout->addLayout( buttonLayout );

  QPushButton *button = new QPushButton( i18n("Group view") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SIGNAL( showGroupView() ) );

  button = new QPushButton( i18n("List view") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SIGNAL( showListView() ) );

  button = new QPushButton( i18n("History") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SIGNAL( showHistory() ) );

  topLayout->addStretch( 1 );

  QLabel *about = new QLabel;
  topLayout->addWidget( about );
  
  QString text = "<qt>";
  text += i18n("Polka - the humane address book for the cloud");
  text += "<br/>";
  text += "<br/>";
  text += "Copyright 2011 Cornelius Schumacher";
  text += "<br/>";
  text += "<br/>";
  text += "http://lst.de/polka";
  text += "</qt>";

  about->setText( text );

  topLayout->addStretch( 1 );
}
