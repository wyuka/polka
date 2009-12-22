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
#include "htmlcreator.h"
#include "imageloader.h"

#include <klocale.h>
#include <KUrl>

PersonView::PersonView( QWidget *parent )
  : QWidget( parent )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  m_titleLabel = new QLabel;
  topLayout->addWidget( m_titleLabel );
  
  m_webView = new QWebView;
  topLayout->addWidget( m_webView );

  QPushButton *button = new QPushButton( i18n("Close") );
  topLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( close() ) );
}

void PersonView::showIdentity( const Identity &identity )
{
  if ( !identity.pictures().pictureList().isEmpty() ) {
    KUrl u( identity.pictures().pictureList().first().url() );
    connect( ImageLoader::load(u), SIGNAL( loaded(const QPixmap &) ),
      SLOT( setImage( const QPixmap & ) ) );
  }

  HtmlDoc doc;
  doc.element("h1").text(identity.name().text());
  HtmlElement &div = doc.element("div");
  foreach( Email email, identity.emails().emailList() ) {
    HtmlElement &a = div.element("div").element("a");
    a.attribute("href","mailto:" + email.text());
    a.text(email.text());
  }

  qDebug() << doc.html();

  m_webView->setHtml( doc.html() );
}

void PersonView::setImage( const QPixmap &pixmap )
{
  m_titleLabel->setPixmap( pixmap );
}
