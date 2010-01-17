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
#ifndef IDENTITYITEM_H
#define IDENTITYITEM_H

#include "polka.h"

#include <QtGui>

class PolkaModel;
class FanMenu;

class IdentityItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
  public:
    IdentityItem( PolkaModel *, const Identity & );

  protected:
    void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

    void mousePressEvent( QGraphicsSceneMouseEvent *event );

  private:
    PolkaModel *m_model;
    Identity m_identity;

    QGraphicsRectItem *m_nameItem;
    FanMenu *m_fanMenu;
};

#endif
