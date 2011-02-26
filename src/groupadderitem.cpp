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

#include "groupadderitem.h"

#include "polkamodel.h"
#include "fanmenu.h"
#include "buttonitem.h"
#include "identityitem.h"

#include <KLocale>

GroupAdderItem::GroupAdderItem( PolkaModel *model )
  : m_model( model ), m_defaultItemSize( 100 ), m_expanded( false )
{
  setItemSize( m_defaultItemSize );
 
  setBrush( QColor( 230,229,229 ) );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  m_expandButton = new ButtonItem( this );
  m_expandButton->setPos( 21, -21 );
  connect( m_expandButton, SIGNAL( clicked() ), SLOT( expand() ) );

  int groupOffset = 85;

  m_groupItem = new IdentityItem( this, m_model, m_model->rootGroup() );
  m_groupItem->setPos( groupOffset, -groupOffset );
  m_groupItem->hide();
  m_groupItem->enableMenus( false );
}

void GroupAdderItem::setItemSize( int size )
{
  setRect( -size/2, -size/2, size, size );
}

void GroupAdderItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void GroupAdderItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );
}

void GroupAdderItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse press does nothing.
}

void GroupAdderItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event )

  // Don't call event handler from parent, so mouse release does nothing.
}

void GroupAdderItem::expand()
{
  m_expanded = !m_expanded;

  if ( m_expanded ) {
    setItemSize( 400 );
    m_expandButton->setMinus();
    m_groupItem->show();
  } else {
    setItemSize( 100 );
    m_expandButton->setPlus();
    m_groupItem->hide();
  }
}
