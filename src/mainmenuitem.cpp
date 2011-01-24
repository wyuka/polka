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

#include "mainmenuitem.h"

#include "polkamodel.h"
#include "fanmenu.h"

#include <KLocale>

MainMenuItem::MainMenuItem()
{
  int itemSize = 55;

  setRect( -itemSize/2, -itemSize/2, itemSize, itemSize );
  setBrush( Qt::white );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  QGraphicsTextItem *textItem = new QGraphicsTextItem( i18n("Menu"), this );

  int textWidth = textItem->boundingRect().width();
  int textHeight = textItem->boundingRect().height();

  textItem->setPos( - textWidth / 2, - textHeight / 2 );

  m_fanMenu = new FanMenu( this );
  connect( m_fanMenu, SIGNAL( itemSelected( FanMenu::Item * ) ),
    SLOT( slotItemSelected( FanMenu::Item * ) ) );
  m_fanMenu->setZValue( 50 );
  m_fanMenu->hide();

  m_cloneGroupMenuItem = m_fanMenu->addItem( i18n("Clone\ngroup") );
  m_removeGroupMenuItem = m_fanMenu->addItem( i18n("Remove\ngroup") );
  m_addGroupMenuItem = m_fanMenu->addItem( i18n("Add\ngroup") );
  m_addPersonMenuItem = m_fanMenu->addItem( i18n("Add\nperson") );

  m_fanMenu->setupItems( 80 );

  setAcceptHoverEvents( true );
}

void MainMenuItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_fanMenu->show();
  emit menuShown();
}

void MainMenuItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_fanMenu->hide();
}

void MainMenuItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsEllipseItem::mousePressEvent( event );
}

void MainMenuItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  QGraphicsEllipseItem::mouseReleaseEvent( event );
}

void MainMenuItem::slotItemSelected( FanMenu::Item *item )
{
  if ( item == m_cloneGroupMenuItem ) {
    emit cloneGroup();
  } else if ( item == m_removeGroupMenuItem ) {
    emit removeGroup();
  } else if ( item == m_addGroupMenuItem ) {
    emit addGroup();
  } else if ( item == m_addPersonMenuItem ) {
    emit addPerson();
  }
}
