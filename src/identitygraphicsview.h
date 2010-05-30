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

#include <QtGui>

class PolkaItemModel;
class PolkaModel;
class IdentityItem;
class LabelItem;

class IdentityGraphicsView : public QWidget
{
    Q_OBJECT
  public:
    IdentityGraphicsView( PolkaModel *, QWidget *parent = 0 );

    void setGroup( const Polka::Identity & );
    Polka::Identity group() const;

    void setGroupName( const QString & );

  signals:
    void goBack();
    void newPerson();
    void showPerson( const Polka::Identity & );
    void removePerson( const Polka::Identity &person,
      const Polka::Identity &group );
    void cloneGroup( const Polka::Identity &group );
    void removeGroup( const Polka::Identity &group );

  protected:
    LabelItem *createLabelItem( const Polka::ViewLabel &label );

  protected slots:
    void resetLayout();
  
    void createItems();
    void slotRemovePerson( const Polka::Identity & );

    void addLabel();
    void removeLabel( LabelItem *, const Polka::ViewLabel & );
    void renameLabel( LabelItem *, Polka::ViewLabel );

    void savePosition( const Polka::Identity &, const QPointF & );
    void saveLabel( const Polka::ViewLabel &, const QPointF & );
    void saveCheck( const Polka::Identity &, bool checked );

    void emitCloneGroup();
    void emitRemoveGroup();

  private:
    PolkaModel *m_model;

    Polka::Identity m_group;

    QList<IdentityItem *> m_items;

    QPushButton *m_backButton;
    QLabel *m_groupNameLabel;  
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;
};

#endif
