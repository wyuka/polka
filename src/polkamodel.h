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
    void insert( Identity ); // create

    Identity &identity( const QString &id );

    // FIXME: consisten API for querying lists of identities
    Identity::List &identityList( const QString &id );
    Identity::List &identities();
    Identity::List persons();

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

  public slots:
    bool readData();

  signals:
    void dataWritten();

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

    Identity m_invalidIdentity;
    Identity::List m_identities;

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
