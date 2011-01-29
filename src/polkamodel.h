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

#include "polka/polka.h"
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

    void writeData( const QString &msg );

    // FIXME: create create,read,update,delete identity functions
    Polka::Identity findIdentity( const QString &id );
    Polka::Identity insert( Polka::Identity, const QString &msg ); // create/update

    Polka::Identity rootGroup();

    Polka::Identity::List groups();

    // FIXME: consistent API for querying lists of identities
    Polka::Identity::List persons();
    Polka::Identity::List identitiesOfGroup( const Polka::Identity &group );
    Polka::Identity::List identitiesOfGroup( const QString &id );
    // FIXME: maybe replace by read-only polka access
    Polka::Identity::List allIdentities();

    void addIdentity( const Polka::Identity &person,
      const Polka::Identity &group );

    void removeIdentity( const Polka::Identity &person,
      const Polka::Identity &group );

    void removeGroup( const Polka::Identity &group );

    PolkaItemModel *allItemModel();
    PolkaItemModel *personsItemModel();
    PolkaItemModel *groupItemModel();
    PolkaItemModel *itemModel( const QString &id = QString() );

    QPixmap picture( const Polka::Picture & ) const;

    QPixmap picture( const Polka::Identity & ) const;
    QString picturePath( const Polka::Identity & ) const;

    void importPicture( const QPixmap &, const Polka::Identity & );

    void removePicture( const Polka::Picture &, Polka::Identity & );

    void saveViewLabel( const Polka::Identity &group,
      const Polka::ViewLabel &label );
    void removeViewLabel( const Polka::Identity &group,
      const Polka::ViewLabel &label );

    void saveViewPosition( const Polka::Identity &group,
      const Polka::Identity &identity,
      const QPointF &pos );
    void clearViewPositions( const Polka::Identity &group );

    void saveViewCheck( const Polka::Identity &group,
      const Polka::Identity &identity, bool checked );

    Polka::GroupView groupView( const Polka::Identity &group );

  public slots:
    bool readData();

  signals:
    void dataWritten();

    void identityAdded( const Polka::Identity & );
    void identityChanged( const Polka::Identity & );
    void identityRemoved( const Polka::Identity & );

  protected slots:
    void slotCommandExecuted( int id );
    void slotPushed();

  protected:
    void setupGroups();

    LocalPicture *localPicture( const Polka::Picture & ) const;

    QPixmap defaultPixmap( const Polka::Identity &identity ) const;
    QString defaultPixmapPath( const Polka::Identity &identity ) const;

  private:
    GitDir *m_gitDir;
    GitRemote *m_gitRemote;
  
    Polka::Polka m_polka;
    bool m_dataIsValid;

    Polka::Identity m_rootGroup;
    Polka::Identity::List m_groups;
    QMap<QString,Polka::Identity::List> m_groupMap;

    PolkaItemModel *m_allItemModel;
    PolkaItemModel *m_personsItemModel;
    PolkaItemModel *m_groupItemModel;
    QMap<QString,PolkaItemModel *> m_itemModels;
    
    int m_commitCommand;

    QMap<QString,QPixmap> m_pictures;
    mutable QMap<QString,LocalPicture *> m_localPictures;

    QPixmap m_defaultGroupPixmap;
    QPixmap m_defaultPersonPixmap;

    QString m_defaultGroupPixmapPath;
    QString m_defaultPersonPixmapPath;
};

#endif
