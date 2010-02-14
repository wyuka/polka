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

#include "polkamodel.h"

#include "gitdir.h"
#include "gitremote.h"
#include "settings.h"
#include "polkaallitemmodel.h"
#include "polkapersonsitemmodel.h"

#include <KRandom>

#include <QDir>
#include <QDebug>

PolkaModel::PolkaModel( QObject *parent )
  : QObject( parent ), m_allItemModel( 0 ), m_groupItemModel( 0 ),
    m_commitCommand( 0 )
{
  m_gitDir = new GitDir( QDir::homePath() + "/.polka" );
  m_gitDir->addFile( "std.polka" );

  m_gitRemote = new GitRemote( m_gitDir );
  connect( m_gitRemote, SIGNAL( pulled() ), SLOT( readData() ) );
  connect( m_gitRemote, SIGNAL( pushed() ), SLOT( slotPushed() ) );

  connect( m_gitDir, SIGNAL( commandExecuted( int ) ),
    SLOT( slotCommandExecuted( int ) ) );
}

PolkaModel::~PolkaModel()
{
  delete m_gitDir;
}

GitRemote *PolkaModel::gitRemote() const
{
  return m_gitRemote;
}

Identity PolkaModel::findIdentity( const QString &id )
{
  return m_polka.findIdentity( id );
}

Identity::List PolkaModel::allIdentities()
{
  return m_polka.identityList();
}

Identity::List PolkaModel::persons()
{
  // FIXME: Cache it in model.

  Identity::List persons;

  foreach( Identity identity, m_polka.identityList() ) {
    if ( !identity.groups().groupList().isEmpty() ) {
      persons.append( identity );
    }
  }

  return persons;
}

Identity::List PolkaModel::identitiesOfGroup( const Identity &group )
{
  return identitiesOfGroup( group.id() );
}

Identity::List PolkaModel::identitiesOfGroup( const QString &id )
{
  if ( id.isEmpty() ) return m_groups;
  if ( !m_groupMap.contains( id ) ) {
    m_groupMap.insert( id, Identity::List() );
  }
  return m_groupMap[ id ];
}

PolkaItemModel *PolkaModel::allItemModel()
{
  if ( !m_allItemModel ) {
    m_allItemModel = new PolkaAllItemModel( this );
  }
  return m_allItemModel;
}

PolkaItemModel *PolkaModel::personsItemModel()
{
  if ( !m_personsItemModel ) {
    m_personsItemModel = new PolkaPersonsItemModel( this );
  }
  return m_personsItemModel;
}

PolkaItemModel *PolkaModel::itemModel( const QString &id )
{
  if ( !m_itemModels.contains( id ) ) {
    PolkaItemModel *itemModel = new PolkaItemModel( this, id );
    m_itemModels.insert( id, itemModel );
  }
  return m_itemModels.value( id );
}

PolkaItemModel *PolkaModel::groupItemModel()
{
  if ( !m_groupItemModel ) {
    m_groupItemModel = new PolkaItemModel( this );
  }
  return m_groupItemModel;
}

bool PolkaModel::readData()
{
  m_polka = Polka::parseFile( m_gitDir->filePath( "std.polka" ),
    &m_dataIsValid );

  if ( !m_dataIsValid ) {
    return false;
  }

  foreach( Identity identity, m_polka.identityList() ) {
    if ( identity.id().isEmpty() ) {
      identity.setId( KRandom::randomString( 10 ) );
      m_polka.insert( identity );
    }
  }

  setupGroups();

  return true;
}

void PolkaModel::setupGroups()
{
  m_groups.clear();
  m_groupMap.clear();

  foreach( Identity identity, m_polka.identityList() ) {
    if ( identity.groups().groupList().isEmpty() ) {
      m_groups.append( identity );
    } else {
      foreach( Group group, identity.groups().groupList() ) {
        if ( m_groupMap.contains( group.id() ) ) {
          m_groupMap[ group.id() ].append( identity );
        } else {
          Identity::List identityList;
          identityList.append( identity );
          m_groupMap.insert( group.id(), identityList );
        }
      }
    }
  }
}

void PolkaModel::writeData()
{
  if ( !m_dataIsValid) {
    emit dataWritten();
    return;
  }

  if ( m_commitCommand > 0 ) {
    qDebug() << "ERROR" << "Commit command still running";
    return;
  }

  m_polka.writeFile( m_gitDir->filePath( "std.polka" ) );
  m_commitCommand = m_gitDir->commitData();
}

void PolkaModel::slotCommandExecuted( int id )
{
  if ( id == m_commitCommand ) {
    m_commitCommand = 0;
    if ( !Settings::remoteSyncingEnabled() ) {
      emit dataWritten();
    }
  }
}

void PolkaModel::slotPushed()
{
  emit dataWritten();
}

void PolkaModel::insert( Identity identity )
{
  if ( identity.id().isEmpty() ) {
    identity.setId( KRandom::randomString( 10 ) );
  }
  m_polka.insert( identity );

  setupGroups();

  if ( identity.groups().groupList().isEmpty() ) {
    m_groupItemModel->updateData();
  } else {
    foreach( Group group, identity.groups().groupList() ) {
      itemModel( group.id() )->updateData();
    }
  }

  emit identityInserted( identity );
}

void PolkaModel::removePerson( const Identity &person, const Identity &group )
{
  Group::List groups = person.groups().groupList();
  Group::List newGroups;
  
  foreach( Group g, groups ) {
    if ( g.id() != group.id() ) {
      newGroups.append( g );
    }
  }
  
  if ( newGroups.isEmpty() ) {
    m_polka.remove( person );
    
    setupGroups();
    
    emit identityRemoved( person );
  } else {
    Identity newPerson = person;
  
    Groups gg;
    gg.setGroupList( newGroups );
    newPerson.setGroups( gg );
    m_polka.insert( newPerson );
    
    setupGroups();
    
    emit identityChanged( newPerson );
  }
}

QPixmap PolkaModel::picture( const Identity &identity ) const
{
  if ( m_localPictures.contains( identity.id() ) ) {
    return m_localPictures.value( identity.id() )->pixmap();
  }

  LocalPicture *localPicture = new LocalPicture( m_gitDir );

  Picture::List pictures = identity.pictures().pictureList();

  if ( !pictures.isEmpty() ) {
    localPicture->setPicture( pictures.first() );
  }

  m_localPictures.insert( identity.id(), localPicture );

  return localPicture->pixmap();
}

void PolkaModel::importPicture( const QPixmap &pixmap,
  const Identity &target )
{
  Identity identity = findIdentity( target.id() );
  
  Pictures pictures = identity.pictures();
  Picture::List pictureList = pictures.pictureList();
  
  Picture picture;
  picture.setId( KRandom::randomString( 10 ) );

  LocalPicture *localPicture = new LocalPicture( m_gitDir );
  localPicture->setPicture( picture );
  localPicture->setPixmap( pixmap );  
  
  pictureList.prepend( picture );
  pictures.setPictureList( pictureList );
  identity.setPictures( pictures );
  m_polka.insert( identity );

  m_localPictures.insert( identity.id(), localPicture );

  emit identityChanged( identity );
}

void PolkaModel::saveViewPosition( const Identity &group,
  const Identity &identity,
  const QPointF &pos )
{
  GroupView v = m_polka.findGroupView( group.id(), Polka::AutoCreate );
  IdentityPosition p = v.findIdentityPosition( identity.id(),
    GroupView::AutoCreate );
  p.setX( pos.x() );
  p.setY( pos.y() );
  v.insert( p );
  m_polka.insert( v );
}

void PolkaModel::clearViewPositions( const Identity &group )
{
  GroupView view = m_polka.findGroupView( group.id() );
  m_polka.remove( view );
}

GroupView PolkaModel::groupView( const Identity &group )
{
  return m_polka.findGroupView( group.id() );
}
