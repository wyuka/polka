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
#ifndef IDENTITYGRAPHICSVIEW_H
#define IDENTITYGRAPHICSVIEW_H

#include "polka/polka.h"

#include "fanmenu.h"

#include <QtGui>

class PolkaItemModel;
class PolkaModel;
class IdentityItem;
class LabelItem;
class QAnimationGroup;
class MainMenuItem;
class MagicMenuItem;
class CloseItem;

class IdentityGraphicsView : public QWidget
{
    Q_OBJECT
  public:
    IdentityGraphicsView( PolkaModel *, QWidget *parent = 0 );

    void setGroup( const Polka::Identity & );
    Polka::Identity group() const;

    void setGroupName( const QString & );

    void setCompactLayout( bool enabled );

  signals:
    void goBack();
    void newPerson();
    void newGroup();
    void showIdentity( const Polka::Identity & );
    void removeIdentity( const Polka::Identity &person,
      const Polka::Identity &group );
    void cloneGroup( const Polka::Identity &group );
    void removeGroup( const Polka::Identity &group );

    void morphedToCompact();
    void morphedFromCompact();

    void showSettings();

    void closeRequested();

  protected:
    LabelItem *createLabelItem( const Polka::ViewLabel &label );

    void morphToCompact();
    void morphFromCompact();

    IdentityItem *item( const Polka::Identity & ) const;

    bool eventFilter( QObject * watched, QEvent * event );

    void positionMenuItems();

  protected slots:
    void resetLayout();
  
    void createItems();
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

    void slotIdentityChanged( const Polka::Identity & );

  private:
    PolkaModel *m_model;

    Polka::Identity m_group;

    QList<IdentityItem *> m_items;
    QList<LabelItem *> m_labelItems;

    MainMenuItem *m_mainMenu;
    MagicMenuItem *m_magicMenu;
    CloseItem *m_closeItem;

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

    bool m_compactLayout;

    QAnimationGroup *m_morphToAnimation;
    QAnimationGroup *m_morphFromAnimation;

    FanMenu *m_globalMenu;
    FanMenu::Item *m_addLabelItem;
};

#endif
