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

#include "identityitem.h"

#include "polkamodel.h"
#include "fanmenu.h"

#include <KLocale>

IdentityItem::IdentityItem( PolkaModel *model, const Identity &identity )
  : QObject( model ), m_model( model ), m_identity( identity )
{
  int itemSize = 110;

  setRect( -itemSize/2, -itemSize/2, itemSize, itemSize );
  setBrush( QColor( "red" ) );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  QPixmap pixmap = m_model->picture( identity );

  QGraphicsItem *item = new QGraphicsPixmapItem( pixmap, this );  
  item->setPos( -pixmap.width() / 2, -pixmap.height() / 2 );

  QGraphicsTextItem *textItem = new QGraphicsTextItem( identity.displayName() );

  int textWidth = textItem->boundingRect().width();
  int textHeight = textItem->boundingRect().height();

  m_nameItem = new QGraphicsRectItem( this );
  m_nameItem->setRect( 0, 0, textWidth, textHeight );
  m_nameItem->setBrush( QColor( 200,200,200 ) );
  m_nameItem->setZValue( 10 );

  textItem->setParentItem( m_nameItem );

  m_nameItem->setPos( - textWidth / 2, 30 );
  m_nameItem->hide();

  m_fanMenu = new FanMenu( this );
  m_fanMenu->setZValue( 20 );
//  m_fanMenu->hide();

  m_fanMenu->addItem( i18n("Remove") );
  m_fanMenu->addItem( i18n("Show") );
  m_fanMenu->setupItems();

  setAcceptHoverEvents( true );

  setFlags( ItemIsMovable );
}

void IdentityItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_nameItem->show();
  m_fanMenu->show();
}

void IdentityItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_nameItem->hide();
}
