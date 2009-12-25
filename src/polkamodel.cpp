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

#include <QDir>
#include <QDebug>

PolkaModel::PolkaModel( QObject *parent )
  : QObject( parent ), m_groupItemModel( 0 ),
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

Identity::List &PolkaModel::identityList( const QString &id )
{
  if ( id.isEmpty() || !m_groupMap.contains( id ) ) return m_groups;
  else return m_groupMap[ id ];
}

PolkaItemModel *PolkaModel::itemModel( const QString &id )
{
  if ( !m_itemModels.contains( id ) ) {
    PolkaItemModel *itemModel = new PolkaItemModel( this, id );
    m_itemModels.insert( id, itemModel );
  } 
  return m_itemModels.value( id );
}

PolkaItemModel *PolkaModel::groupItemModel() const
{
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

  delete m_groupItemModel;
  m_groupItemModel = new PolkaItemModel( this );

  return true;
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
    emit dataWritten();
  }
}

void PolkaModel::insert( const Identity &identity )
{
  m_polka.addIdentity( identity );
  if ( identity.groups().groupList().isEmpty() ) {
    m_groupItemModel->updateData();
  } else {
    foreach( Group group, identity.groups().groupList() ) {
      m_groupMap[ group.id() ].append( identity );
      m_itemModels[ group.id() ]->updateData();
    }
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
