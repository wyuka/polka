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

    Polka &polka();

    PolkaItemModel *itemModel() const;

    void insert( const Identity & );

    bool hasPicture( const Identity & ) const;
    QPixmap picture( const Identity & ) const;

  signals:
    void dataWritten();

  protected slots:
    void slotCommandExecuted( int id );

  private:
    GitDir *m_gitDir;

    Polka m_polka;
    
    PolkaItemModel *m_itemModel;
    
    int m_commitCommand;

    QMap<QString,QPixmap> m_pictures;
};

#endif
