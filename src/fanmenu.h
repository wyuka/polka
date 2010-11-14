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
#ifndef FANMENU_H
#define FANMENU_H

#include <QtGui>

class IdentityItem;
class FanMenuElement;

class FanMenu : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
  public:
    class Item {
      public:
        Item( const QString &text );

        void setText( const QString &text );        
        QString text() const;
        
        void setElement( FanMenuElement * );
        
      private:
        QString m_text;
        FanMenuElement *m_element;
    };

    FanMenu( QGraphicsItem *parent );

    /**
      Setup items. freeSpace is the percentage of the radius the menu covers.
    */
    void setupItems( int coverage = 67 );

    Item *addItem( const QString &text ); 

    void emitItemSelected( Item * );

    bool isCloseTo( const QPointF & );

    qreal radius() const;

  signals:
    void itemSelected( FanMenu::Item * );

  private:
    QList<Item *> m_items;
};

#endif
