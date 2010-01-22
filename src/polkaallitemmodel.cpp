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

#include "polkaallitemmodel.h"

#include "polkamodel.h"

#include <QDebug>

PolkaAllItemModel::PolkaAllItemModel( PolkaModel *polkaModel )
  : PolkaItemModel( polkaModel )
{
}

int PolkaAllItemModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED( parent );

  return model()->identities().count();
}

Identity PolkaAllItemModel::getIdentityData( const QModelIndex &index ) const
{
  return model()->identities().at( index.row() );
}

Identity PolkaAllItemModel::identity( const QModelIndex &index )
{
  return model()->identities().at( index.row() );
}
