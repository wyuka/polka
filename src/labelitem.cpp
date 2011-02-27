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
  setBrush( QColor( 255,255,230 ) );

  m_textItem = new QGraphicsTextItem( this );
  setText( label.text() );

  m_fanMenu = new FanMenu( this );
  connect( m_fanMenu, SIGNAL( itemSelected( FanMenuItem * ) ),
    SLOT( slotItemSelected( FanMenuItem * ) ) );
  m_fanMenu->setZValue( 50 );
  m_fanMenu->hide();

  m_removeMenuItem = m_fanMenu->addItem( i18n("Remove") );
  m_renameMenuItem = m_fanMenu->addItem( i18n("Rename") );
  m_fanMenu->setupItems( 80 );

  setAcceptHoverEvents( true );

  setFlags( ItemIsMovable );
}

void LabelItem::setText( const QString &text )
{
  m_textItem->setPlainText( text );

  int textWidth = m_textItem->boundingRect().width();
  int textHeight = m_textItem->boundingRect().height();

  setRect( - textWidth / 2, - textHeight / 2, textWidth, textHeight );

  m_textItem->setPos( - textWidth / 2, - textHeight / 2 );
}

void LabelItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_fanMenu->show();
  emit menuShown();
}

void LabelItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  m_fanMenu->hide();
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

void LabelItem::slotItemSelected( FanMenuItem *item )
{
  if ( item == m_removeMenuItem ) {
    emit removeLabel( this, m_label );
  } else if ( item == m_renameMenuItem ) {
    emit renameLabel( this, m_label );
  }
}
