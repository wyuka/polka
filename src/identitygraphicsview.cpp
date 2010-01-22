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

#include "identitygraphicsview.h"

#include "polkamodel.h"
#include "polkaitemmodel.h"
#include "personview.h"
#include "identityitem.h"

#include <KLocale>

IdentityGraphicsView::IdentityGraphicsView( PolkaModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  QBoxLayout *buttonLayout = new QHBoxLayout;
  topLayout->addLayout( buttonLayout );

  // FIXME: Use proper icon
  m_backButton = new QPushButton( "<" );
  buttonLayout->addWidget( m_backButton );
  connect( m_backButton, SIGNAL( clicked() ), SIGNAL( goBack() ) );

  buttonLayout->addStretch( 1 );

  m_groupNameLabel = new QLabel;
  buttonLayout->addWidget( m_groupNameLabel );

  buttonLayout->addStretch( 1 );

  QPushButton *button = new QPushButton( i18n("New Person") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SIGNAL( newPerson() ) );

  m_scene = new QGraphicsScene;

  QGraphicsView *view = new QGraphicsView( m_scene );
  view->setRenderHint( QPainter::Antialiasing );
  topLayout->addWidget( view );
  view->show();

  connect( m_model, SIGNAL( identityInserted( const Identity & ) ),
    SLOT( createItems() ) );
  connect( m_model, SIGNAL( identityChanged( const Identity & ) ),
    SLOT( createItems() ) );
  connect( m_model, SIGNAL( identityRemoved( const Identity & ) ),
    SLOT( createItems() ) );
}

void IdentityGraphicsView::setGroup( const Identity &group )
{
  m_group = group;

  setGroupName( group.displayName() );

  createItems();
}

void IdentityGraphicsView::createItems()
{
  qDebug() << "CREATE ITEMS";

  m_scene->clear();

  Identity::List identities = m_model->identityList( m_group.id() );

  int columns = sqrt( identities.size() );
  int spacing = 125;

  int x = 0;
  int y = 0;

  foreach( Identity identity, identities ) {
    int posX = x * spacing + ( y % 2 ) * spacing / 2;
    int posY = y * spacing;

    IdentityItem *item = new IdentityItem( m_model, identity );
    connect( item, SIGNAL( showPerson( const Identity & ) ),
      SIGNAL( showPerson( const Identity & ) ) );
    connect( item, SIGNAL( removePerson( const Identity & ) ),
      SLOT( slotRemovePerson( const Identity & ) ) );
    item->setPos( posX, posY );
    m_scene->addItem( item );

    x++;
    
    if ( x >= ( columns + ( y + 1 ) % 2 ) ) {
      x = 0;
      y++;
    }
  }
}

Identity IdentityGraphicsView::group() const
{
  return m_group;
}

void IdentityGraphicsView::setGroupName( const QString &name )
{
  m_groupNameLabel->setText( "<b>" + name + "</b>" );
}

void IdentityGraphicsView::slotRemovePerson( const Identity &identity )
{
  emit removePerson( identity, m_group );
}
