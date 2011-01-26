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

#include "polka/polka.h"
#include "fanmenu.h"

#include <QtGui>

class PolkaModel;
class RoundedRectItem;

class IdentityItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

  public:
    IdentityItem( PolkaModel *, const Polka::Identity & );

    Polka::Identity identity() const;

    void updateItem( const Polka::Identity & );

    void setDefaultPos( const QPointF & );
    QPointF defaultPos() const;

    void rememberPos( const QPointF & );
    QPointF rememberedPos() const;

    void checkItem();

  signals:
    void showGroup( const Polka::Identity & );
  
    void showIdentity( const Polka::Identity & );
    void removeIdentity( const Polka::Identity & );

    void itemMoved( const Polka::Identity &, const QPointF & );
    
    void itemChecked( const Polka::Identity &, bool );

    void menuShown();

  protected:
    void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
    void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );

    void mousePressEvent( QGraphicsSceneMouseEvent *event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

    QVariant itemChange( GraphicsItemChange change, const QVariant &value );

  protected slots:
    void slotItemSelected( FanMenu::Item * );

  private:
    PolkaModel *m_model;
    Polka::Identity m_identity;

    QPointF m_defaultPos;
    QPointF m_rememberedPos;
    bool m_checked;

    RoundedRectItem *m_nameItem;
    QGraphicsPathItem *m_checkItem;

    FanMenu *m_fanMenu;
    FanMenu::Item *m_removeMenuItem;
    FanMenu::Item *m_showMenuItem;
    FanMenu::Item *m_groupShowMenuItem;
    FanMenu::Item *m_checkMenuItem;

    QPointF m_movePos;

    int m_itemSize;
};

#endif
