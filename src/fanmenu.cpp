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
  : m_text( text )
{
}

QString FanMenu::Item::text() const
{
  return m_text;
}


FanMenu::FanMenu( IdentityItem *parent )
  : QObject( parent ), QGraphicsItemGroup( parent )
{
}

void FanMenu::setupItems()
{
  int startAngle = -5;
  int endAngle = 185;
  int spacing = 10;
  int count = m_items.count();
 
  int width = ( ( endAngle - startAngle ) - ( count - 1 ) * spacing ) / count;

  for( int i = 0; i < count; ++i ) {
    int angle = startAngle + i * width + i * spacing;
    FanMenuElement *element = new FanMenuElement( this );
    element->setup( m_items[i], angle, angle + width );
  }
}

FanMenu::Item *FanMenu::addItem( const QString &text )
{
  Item *item = new Item( text );
  m_items.append( item );
  return item;
}
