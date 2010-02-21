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
#ifndef POLKAMODEL_H
#define POLKAMODEL_H

#include "polka.h"
#include "polkaitemmodel.h"
#include "localpicture.h"

#include <QObject>

class GitDir;
class GitRemote;

class PolkaModel : public QObject
{
    Q_OBJECT
  public:
    PolkaModel( QObject *parent = 0 );
    ~PolkaModel();

    GitRemote *gitRemote() const;

    void writeData();

    // FIXME: create create,read,update,delete identity functions
    Identity findIdentity( const QString &id );
    void insert( Identity ); // create

    // FIXME: consistent API for querying lists of identities
    Identity::List persons();
    Identity::List identitiesOfGroup( const Identity &group );
    Identity::List identitiesOfGroup( const QString &id );
    // FIXME: maybe replace by read-only polka access
    Identity::List allIdentities();

    void removePerson( const Identity &person, const Identity &group );

    PolkaItemModel *allItemModel();
    PolkaItemModel *personsItemModel();
    PolkaItemModel *groupItemModel();
    PolkaItemModel *itemModel( const QString &id = QString() );

    QPixmap picture( const Identity & ) const;

    void importPicture( const QPixmap &, const Identity & );

    void saveViewPosition( const Identity &group,
      const Identity &identity,
      const QPointF &pos );
    void clearViewPositions( const Identity &group );

    GroupView groupView( const Identity &group );

  public slots:
    bool readData();

  signals:
    void dataWritten();

    // FIXME: What's the difference between inserted and changed?
    void identityInserted( const Identity & );
    void identityChanged( const Identity & );
    void identityRemoved( const Identity & );

  protected slots:
    void slotCommandExecuted( int id );
    void slotPushed();

  protected:
    void setupGroups();

  private:
    GitDir *m_gitDir;
    GitRemote *m_gitRemote;
  
    Polka m_polka;
    bool m_dataIsValid;

    Identity::List m_groups;
    QMap<QString,Identity::List> m_groupMap;

    PolkaItemModel *m_allItemModel;
    PolkaItemModel *m_personsItemModel;
    PolkaItemModel *m_groupItemModel;
    QMap<QString,PolkaItemModel *> m_itemModels;
    
    int m_commitCommand;

    QMap<QString,QPixmap> m_pictures;
    mutable QMap<QString,LocalPicture *> m_localPictures;
};

#endif
