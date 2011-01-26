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

#include "closeitem.h"

#include <KLocale>

CloseItem::CloseItem()
  : QObject()
{
  int height = 100;
  int width = 20;

  QPolygonF polygon;
  polygon << QPointF( - width / 2, - height / 2 );
  polygon << QPointF( width / 2, 0 );
  polygon << QPointF( -width / 2, height / 2 );

  setPolygon( polygon );
  setBrush( QColor( 230,229,229 ) );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  setAcceptHoverEvents( true );
}

void CloseItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  setBrush( Qt::white );

  Q_UNUSED( event );
}

void CloseItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  setBrush( QColor( 230,229,229 ) );

  Q_UNUSED( event );
}

void CloseItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event );

  emit closeRequested();
}

void CloseItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  Q_UNUSED( event );
}
