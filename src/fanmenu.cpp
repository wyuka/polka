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

#include "fanmenu.h"

#include "identityitem.h"
#include "fanmenuelement.h"

FanMenu::Item::Item( const QString &text )
  : m_text( text ), m_element( 0 )
{
}

void FanMenu::Item::setText( const QString &text )
{
  m_text = text;
  if ( m_element ) {
    m_element->updateText();
  }
}

QString FanMenu::Item::text() const
{
  return m_text;
}

void FanMenu::Item::setElement( FanMenuElement *element )
{
  m_element = element;
}


FanMenu::FanMenu( QGraphicsItem *parent )
  : QGraphicsLineItem( parent ), m_startAngle( -5 ), m_endAngle( 185 ),
    m_spacing( 10 ), m_radius( 90 )
{
}

void FanMenu::setRadius( qreal radius )
{
  m_radius = radius;
}

qreal FanMenu::radius() const
{
  return m_radius;
}

void FanMenu::setupItems( int coverage )
{
  int count = m_items.count();
 
  int width = ( ( m_endAngle - m_startAngle ) - ( count - 1 ) * m_spacing ) /
    count;

  for( int i = 0; i < count; ++i ) {
    int angle = m_startAngle + i * width + i * m_spacing;
    FanMenuElement *element = new FanMenuElement( this );
    element->setup( m_items[i], angle, angle + width, coverage );
  }
}

FanMenu::Item *FanMenu::addItem( const QString &text )
{
  Item *item = new Item( text );
  m_items.append( item );
  return item;
}

void FanMenu::emitItemSelected( Item *item )
{
  emit itemSelected( item );
}

bool FanMenu::isCloseTo( const QPointF &point )
{
  return QLineF( point, pos() ).length() <= radius() + 10;
}

void FanMenu::setStartAngle( int angle )
{
  m_startAngle = angle;
}

void FanMenu::setEndAngle( int angle )
{
  m_endAngle = angle;
}

void FanMenu::setSpacing( int spacing )
{
  m_spacing = spacing;
}
