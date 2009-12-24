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

#include <KStandardDirs>

PolkaItemModel::PolkaItemModel( Polka &polka, QObject *parent )
  : QAbstractListModel( parent ), m_polka( polka )
{
  QString picPath = KStandardDirs::locate( "appdata", "polka_person.png" );
  m_defaultPicture = QPixmap( picPath );
}

int PolkaItemModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED( parent );

  return m_polka.identityList().count();
}

QVariant PolkaItemModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= m_polka.identityList().size())
    return QVariant();

  if (role == Qt::DisplayRole)
    return m_polka.identityList().at(index.row()).name().text();
  else if ( role == Qt::DecorationRole )
    return m_defaultPicture;
  else
    return QVariant();
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

Identity PolkaItemModel::identity( const QModelIndex &index )
{
  return m_polka.identityList().at(index.row());
}
