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

class PolkaModel : public QObject
{
    Q_OBJECT
  public:
    PolkaModel( QObject *parent = 0 );
    ~PolkaModel();

    void readData();
    void writeData();

    Identity::List &identityList( const QString &id );

    PolkaItemModel *groupItemModel() const;
    PolkaItemModel *itemModel( const QString &id = QString() );

    void insert( const Identity & );

    QPixmap picture( const Identity & ) const;

  signals:
    void dataWritten();

  protected slots:
    void slotCommandExecuted( int id );

  private:
    GitDir *m_gitDir;

    Polka m_polka;

    Identity::List m_groups;
    QMap<QString,Identity::List> m_groupMap;

    QMap<QString,PolkaItemModel *> m_itemModels;
    PolkaItemModel *m_groupItemModel;
    
    int m_commitCommand;

    QMap<QString,QPixmap> m_pictures;
    mutable QMap<QString,LocalPicture *> m_localPictures;
};

#endif
