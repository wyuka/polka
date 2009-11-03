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
  : QObject( parent ), m_commitCommand( 0 )
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

void PolkaModel::readData()
{
  bool ok;
  m_polka = Polka::parseFile( m_gitDir->filePath( "std.polka" ), &ok );
}

void PolkaModel::writeData()
{
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
