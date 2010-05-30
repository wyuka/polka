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

#include "labelitem.h"

#include "polkamodel.h"

#include <KLocale>

LabelItem::LabelItem( PolkaModel *model, const Polka::ViewLabel &label )
  : QObject( model ), m_model( model ), m_label( label )
{
  m_textItem = new QGraphicsTextItem( label.text(), this );

  int textWidth = m_textItem->boundingRect().width();
  int textHeight = m_textItem->boundingRect().height();

  setRect( 0, 0, textWidth, textHeight );
  setBrush( QColor( 220,220,220 ) );
  setZValue( 10 );

  m_textItem->setPos( 0, 0 );

  setFlags( ItemIsMovable );
}

void LabelItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  m_movePos = pos();

  QGraphicsRectItem::mousePressEvent( event );
}

void LabelItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  if ( pos() != m_movePos ) {
    emit itemMoved( m_label, pos() );
  }

  QGraphicsRectItem::mouseReleaseEvent( event );
}
