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

#include "personview.h"

#include <klocale.h>

PersonView::PersonView( QWidget *parent )
  : QWidget( parent )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );
  
  m_webView = new QWebView;
  topLayout->addWidget( m_webView );

  QPushButton *button = new QPushButton( i18n("Close") );
  topLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( close() ) );
}

void PersonView::showIdentity( const Identity &identity )
{
  QString html = "<h1>Name</h1>";
  html += identity.name().text();

  m_webView->setHtml( html );
}