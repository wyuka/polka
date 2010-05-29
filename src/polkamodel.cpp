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
  : QObject( parent ), m_allItemModel( 0 ), m_personsItemModel( 0 ),
    m_groupItemModel( 0 ),
    m_commitCommand( 0 )
{
  m_gitDir = new GitDir( QDir::homePath() + "/.polka" );

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

Polka::Identity PolkaModel::findIdentity( const QString &id )
{
  return m_polka.findIdentity( id );
}

Polka::Identity::List PolkaModel::allIdentities()
{
  return m_polka.identityList();
}

Polka::Identity::List PolkaModel::persons()
{
  // FIXME: Cache it in model.

  Polka::Identity::List persons;

  foreach( Polka::Identity identity, m_polka.identityList() ) {
    if ( !identity.groups().groupList().isEmpty() ) {
      persons.append( identity );
    }
  }

  return persons;
}

Polka::Identity::List PolkaModel::identitiesOfGroup( const Polka::Identity &group )
{
  return identitiesOfGroup( group.id() );
}

Polka::Identity::List PolkaModel::identitiesOfGroup( const QString &id )
{
  if ( id.isEmpty() ) return m_groups;
  if ( !m_groupMap.contains( id ) ) {
    m_groupMap.insert( id, Polka::Identity::List() );
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
  QString dataFile = m_gitDir->filePath( "std.polka" );

  if ( QFile::exists( dataFile ) ) {
    m_polka = Polka::Polka::parseFile( dataFile, &m_dataIsValid );

    if ( !m_dataIsValid ) {
      return false;
    }
  } else {
    m_dataIsValid = true;
  }

  foreach( Polka::Identity identity, m_polka.identityList() ) {
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

  foreach( Polka::Identity identity, m_polka.identityList() ) {
    if ( identity.groups().groupList().isEmpty() ) {
      m_groups.append( identity );
    } else {
      foreach( Polka::Group group, identity.groups().groupList() ) {
        if ( m_groupMap.contains( group.id() ) ) {
          m_groupMap[ group.id() ].append( identity );
        } else {
          Polka::Identity::List identityList;
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
  m_gitDir->addFile( "std.polka" );
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

Polka::Identity PolkaModel::insert( Polka::Identity identity )
{
  if ( identity.id().isEmpty() ) {
    identity.setId( KRandom::randomString( 10 ) );
  }
  m_polka.insert( identity );

  setupGroups();

  if ( identity.groups().groupList().isEmpty() ) {
    m_groupItemModel->updateData();
  } else {
    foreach( Polka::Group group, identity.groups().groupList() ) {
      itemModel( group.id() )->updateData();
    }
  }

  emit identityInserted( identity );

  return identity;
}

void PolkaModel::removePerson( const Polka::Identity &person,
  const Polka::Identity &group )
{
  Polka::Group::List groups = person.groups().groupList();
  Polka::Group::List newGroups;
  
  foreach( Polka::Group g, groups ) {
    if ( g.id() != group.id() ) {
      newGroups.append( g );
    }
  }
  
  if ( newGroups.isEmpty() ) {
    m_polka.remove( person );
    
    setupGroups();
    
    emit identityRemoved( person );
  } else {
    Polka::Identity newPerson = person;
  
    Polka::Groups gg;
    gg.setGroupList( newGroups );
    newPerson.setGroups( gg );
    m_polka.insert( newPerson );
    
    setupGroups();
    
    emit identityChanged( newPerson );
  }
}

void PolkaModel::removeGroup( const Polka::Identity &group )
{
  Polka::Identity::List members = m_groupMap.value( group.id() );
  foreach( Polka::Identity member, members ) {
    removePerson( member, group );
  }
  m_polka.remove( group );

  setupGroups();
  
  emit identityRemoved( group );
}

QPixmap PolkaModel::picture( const Polka::Identity &identity ) const
{
  if ( m_localPictures.contains( identity.id() ) ) {
    return m_localPictures.value( identity.id() )->pixmap();
  }

  LocalPicture *localPicture = new LocalPicture( m_gitDir );

  Polka::Picture::List pictures = identity.pictures().pictureList();

  if ( !pictures.isEmpty() ) {
    localPicture->setPicture( pictures.first() );
  }

  m_localPictures.insert( identity.id(), localPicture );

  return localPicture->pixmap();
}

void PolkaModel::importPicture( const QPixmap &pixmap,
  const Polka::Identity &target )
{
  Polka::Identity identity = findIdentity( target.id() );
  
  Polka::Pictures pictures = identity.pictures();
  Polka::Picture::List pictureList = pictures.pictureList();
  
  Polka::Picture picture;
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

void PolkaModel::saveViewPosition( const Polka::Identity &group,
  const Polka::Identity &identity,
  const QPointF &pos )
{
  Polka::GroupView v = m_polka.findGroupView( group.id(),
    Polka::Polka::AutoCreate );
  Polka::IdentityPosition p = v.findIdentityPosition( identity.id(),
    Polka::GroupView::AutoCreate );
  p.setX( pos.x() );
  p.setY( pos.y() );
  v.insert( p );
  m_polka.insert( v );
}

void PolkaModel::clearViewPositions( const Polka::Identity &group )
{
  Polka::GroupView view = m_polka.findGroupView( group.id() );
  view.setIdentityPositionList( Polka::IdentityPosition::List() );
  m_polka.insert( view );
}

void PolkaModel::saveViewCheck( const Polka::Identity &group,
  const Polka::Identity &identity,
  bool checked )
{
  Polka::GroupView v = m_polka.findGroupView( group.id(),
    Polka::Polka::AutoCreate );
  if ( checked ) {
    Polka::IdentityCheck c = v.findIdentityCheck( identity.id(),
      Polka::GroupView::AutoCreate );
    v.insert( c );
  } else {
    Polka::IdentityCheck c = v.findIdentityCheck( identity.id(),
      Polka::GroupView::AutoCreate );
    if ( c.isValid() ) v.remove( c );
  }
  m_polka.insert( v );
}

Polka::GroupView PolkaModel::groupView( const Polka::Identity &group )
{
  return m_polka.findGroupView( group.id() );
}
