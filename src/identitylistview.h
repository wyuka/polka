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
#ifndef IDENTITYLISTVIEW_H
#define IDENTITYLISTVIEW_H

#include "polka/polka.h"

#include <QtGui>

class PolkaItemModel;
class PolkaModel;

class IdentityListView : public QWidget
{
    Q_OBJECT
  public:
    IdentityListView( PolkaModel *, QWidget *parent = 0 );

    void setGroup( const Polka::Identity & );
    Polka::Identity group() const;

    void setItemModel( PolkaItemModel * );

    void setGroupName( const QString & );

  signals:
    void goBack();
    void newPerson();
    void showIdentity( const Polka::Identity & );

  protected slots:
    void slotItemClicked( const QModelIndex &index );

  private:
    PolkaModel *m_model;
    PolkaItemModel *m_itemModel;

    Polka::Identity m_group;

    QPushButton *m_backButton;
    QLabel *m_groupNameLabel;  
    QListView *m_flatView;
};

#endif
