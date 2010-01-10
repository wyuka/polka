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

#include <KRandom>

#include <QDir>
#include <QDebug>

PolkaModel::PolkaModel( QObject *parent )
  : QObject( parent ), m_allItemModel( 0 ), m_groupItemModel( 0 ),
    m_commitCommand( 0 )
{
  m_gitDir = new GitDir( QDir::homePath() + "/.polka" );
  m_gitDir->addFile( "std.polka" );

  connect( m_gitDir, SIGNAL( commandExecuted( int ) ),
    SLOT( slotCommandExecuted( int ) ) );
}

PolkaModel::~PolkaModel()
{
  delete m_gitDir;
}

Identity &PolkaModel::identity( const QString &id )
{
  for( int i = 0; i < m_identities.size(); ++i ) {
    if ( m_identities[i].id() == id ) return m_identities[i]; 
  }
  return m_invalidIdentity;
}

Identity::List &PolkaModel::identities()
{
  return m_identities;
}

Identity::List &PolkaModel::identityList( const QString &id )
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
    m_allItemModel = new PolkaItemModel( this );
    m_allItemModel->setAll( true );
  }
  return m_allItemModel;
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

  m_identities.clear();

  foreach( Identity identity, m_polka.identityList() ) {
    if ( identity.id().isEmpty() ) {
      identity.setId( KRandom::randomString( 10 ) );
    }

    m_identities.append( identity );
  }

  setupGroups();

  return true;
}

void PolkaModel::setupGroups()
{
  m_groups.clear();
  m_groupMap.clear();

  foreach( Identity identity, m_identities ) {
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

  m_polka.setIdentityList( m_identities );
  
  m_polka.writeFile( m_gitDir->filePath( "std.polka" ) );
  m_commitCommand = m_gitDir->commitData();
}

void PolkaModel::slotCommandExecuted( int id )
{
  if ( id == m_commitCommand ) {
    m_commitCommand = 0;
    emit dataWritten();
  }
}

void PolkaModel::insert( Identity identity )
{
  if ( identity.id().isEmpty() ) {
    identity.setId( KRandom::randomString( 10 ) );
    m_identities.append( identity );
  } else {
    this->identity( identity.id() ) = identity;
  }

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
  Identity &identity = this->identity( target.id() );
  
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
}
