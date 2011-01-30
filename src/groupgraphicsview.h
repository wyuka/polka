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
#ifndef GROUPGRAPHICSVIEW_H
#define GROUPGRAPHICSVIEW_H

#include "fanmenu.h"
#include "groupview.h"

#include <QtGui>

class PolkaModel;
class IdentityItem;
class LabelItem;
class QAnimationGroup;
class MainMenuItem;
class MagicMenuItem;

struct IdentityItemGroup {

  IdentityItemGroup() : previousGroup( 0 ) {}

  QList<IdentityItem *> items;
  QPointF center;
  IdentityItem *previousGroup;
};

class GroupGraphicsView : public GroupView
{
    Q_OBJECT
  public:
    GroupGraphicsView( PolkaModel *, QWidget *parent = 0 );

    void setCompactLayout( bool enabled );

  signals:
    void newGroup();
    void removeIdentity( const Polka::Identity &person,
      const Polka::Identity &group );
    void cloneGroup( const Polka::Identity &group );
    void removeGroup( const Polka::Identity &group );

    void morphedToCompact();
    void morphedFromCompact();

    void closeRequested();

  protected:
    void doShowGroup();

    IdentityItemGroup prepareIdentityItems( bool doAnimation );
    void createMenuItems();
    void createLabelItems();
  
    LabelItem *createLabelItem( const Polka::ViewLabel &label );

    void morphToCompact();
    void morphFromCompact();

    IdentityItem *item( const Polka::Identity & ) const;

    bool eventFilter( QObject * watched, QEvent * event );

    void positionMenuItems();

  protected slots:
    void resetLayout();
  
    void hideItems();
    void placeItems();
    void unplaceItems();
    void unhideItems();
    void recreateItems();

    void slotRemoveIdentity( const Polka::Identity & );

    void addLabel();
    void addLabel( const QPointF & );
    void removeLabel( LabelItem *, const Polka::ViewLabel & );
    void renameLabel( LabelItem *, Polka::ViewLabel );

    void savePosition( const Polka::Identity &, const QPointF & );
    void saveLabel( const Polka::ViewLabel &, const QPointF & );
    void saveCheck( const Polka::Identity &, bool checked );

    void emitCloneGroup();
    void emitRemoveGroup();

    void slotItemSelected( FanMenu::Item *item );

    void hideGlobalMenu();

    void slotMouseMoved( const QPoint &pos );

    void positionAbsoluteItems();

    void finishMorphFromCompact();
    void finishMorphToCompact();

    void finishPlaceItems();

    void slotIdentityAdded( const Polka::Identity & );
    void slotIdentityChanged( const Polka::Identity & );
    void slotIdentityRemoved( const Polka::Identity & );

  private:
    QList<IdentityItem *> m_items;
    QList<LabelItem *> m_labelItems;

    IdentityItem *m_previousItem;

    IdentityItemGroup m_newItems;

    MainMenuItem *m_mainMenu;
    MagicMenuItem *m_magicMenu;

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

    bool m_compactLayout;

    QAnimationGroup *m_morphToAnimation;
    QAnimationGroup *m_morphFromAnimation;

    QAnimationGroup *m_removeItemsAnimation;
    QAnimationGroup *m_placeItemsAnimation;
    QList<QPropertyAnimation *> m_placeItemsAnimations;
    QAnimationGroup *m_unplaceItemsAnimation;
    QAnimationGroup *m_unhideItemsAnimation;

    FanMenu *m_globalMenu;
    FanMenu::Item *m_addLabelItem;
};

#endif
