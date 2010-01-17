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

#include "fanmenuelement.h"

FanMenuElement::FanMenuElement( FanMenu *menu )
  : QGraphicsPathItem( menu ), m_menu( menu ), m_item( 0 )
{
  setBrush( QColor( 200,200,200 ) );

  setAcceptHoverEvents( true );
}

void FanMenuElement::setup( FanMenu::Item *menuItem, int startAngle,
  int endAngle )
{
  m_item = menuItem;

  qreal blockHeight = 60;
  qreal blockRadius = 90;

  QPoint center( 0,0 );

  QRectF outerBlockRect(
    center.x() - blockRadius,
    center.y() - blockRadius,
    ( blockRadius ) * 2,
    ( blockRadius ) * 2 );
  QRectF innerBlockRect(
    center.x() - blockRadius + blockHeight,
    center.y() - blockRadius + blockHeight,
    ( blockRadius - blockHeight ) * 2,
    ( blockRadius - blockHeight ) * 2 );

  QPainterPath blockPath;

  QPainterPath helperPath;

  helperPath.arcTo( outerBlockRect, startAngle, 0 );
  QPointF upperLeftPos = helperPath.currentPosition();

  blockPath.moveTo( upperLeftPos );
  blockPath.arcTo( outerBlockRect, startAngle, endAngle - startAngle );

  helperPath.arcTo( innerBlockRect, endAngle, 0 );
  QPointF lowerRightPos = helperPath.currentPosition();

  blockPath.lineTo( lowerRightPos );
  blockPath.arcTo( innerBlockRect, endAngle, startAngle - endAngle );

  blockPath.closeSubpath();

  setPath( blockPath );


  QGraphicsTextItem *textItem = new QGraphicsTextItem( menuItem->text(), this );

  qreal textX = boundingRect().x() + boundingRect().width() / 2;
  qreal textY = boundingRect().y() + boundingRect().height() / 2;

  textX -= textItem->boundingRect().width() / 2;
  textY -= textItem->boundingRect().height() / 2;
   
  textItem->setPos( textX, textY );
}

void FanMenuElement::mousePressEvent( QGraphicsSceneMouseEvent * )
{
  qDebug() << m_item->text() << "PRESSED";
  m_menu->emitItemSelected( m_item );
}

void FanMenuElement::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  QGraphicsPathItem::hoverEnterEvent( event );
}
