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

#include "polkaitemmodel.h"

#include "polkamodel.h"

#include <QDebug>

PolkaItemModel::PolkaItemModel( PolkaModel *polkaModel, const QString &groupId )
  : QAbstractListModel( polkaModel ), m_model( polkaModel ),
    m_groupId( groupId )
{
}

PolkaItemModel::~PolkaItemModel()
{
}

PolkaModel *PolkaItemModel::model() const
{
  return m_model;
}

int PolkaItemModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED( parent );

  return m_model->identitiesOfGroup( m_groupId ).count();
}

QVariant PolkaItemModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if ( index.row() >= rowCount( QModelIndex() ) ) return QVariant();

  Polka::Identity identity = getIdentityData( index );

  if (role == Qt::DisplayRole) {
    return identity.name().value();
  } else if ( role == Qt::UserRole ) {
    return identity.id();
  } else if ( role == Qt::DecorationRole ) {
    return m_model->picture( identity );
  } else {
    return QVariant();
  }
}

Polka::Identity PolkaItemModel::getIdentityData( const QModelIndex &index ) const
{
  return m_model->identitiesOfGroup( m_groupId ).at( index.row() );
}

QVariant PolkaItemModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal)
    return QString("Column %1").arg(section);
  else
    return QString("Row %1").arg(section);
}

void PolkaItemModel::updateData()
{
  reset();
}

Polka::Identity PolkaItemModel::identity( const QModelIndex &index )
{
  return m_model->identitiesOfGroup( m_groupId ).at( index.row() );
}
