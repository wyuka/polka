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
#ifndef LOCALPICTURE_H
#define LOCALPICTURE_H

#include "polka.h"
#include "gitdir.h"

#include <QPixmap>

class LocalPicture : public QObject
{
    Q_OBJECT
  public:
    LocalPicture( GitDir * );

    void setPicture( const Picture & );

    QPixmap pixmap();

  public slots:
    void setPixmap( const QPixmap &pixmap );

  signals:
    void pixmapChanged( const QPixmap & );

  protected:
    QString fullFilePath() const;
    QString localFilePath() const;

    bool fileExists() const;

  private:
    Picture m_picture;
    GitDir *m_gitDir;

    QPixmap m_pixmap;
    QPixmap m_defaultPixmap;
};

#endif
