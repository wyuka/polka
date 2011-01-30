/*
    This file is part of KDE.

    Copyright (C) 2011 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef PICTURESELECTORCONTROLS_H
#define PICTURESELECTORCONTROLS_H

#include "polka/polka.h"

#include <QtGui>

class PolkaModel;

class PictureSelectorControls : public QWidget
{
    Q_OBJECT
  public:
    PictureSelectorControls( PolkaModel *model, QWidget *parent = 0 );
    virtual ~PictureSelectorControls();

    void setIdentity( const Polka::Identity & );

  public slots:
    void setPicture( const Polka::Picture & );

  protected slots:
    void makeDefault();
    void removePicture();

  private:
    PolkaModel *m_model;

    Polka::Identity m_identity;
    Polka::Picture m_picture;

    QLabel *m_pictureLabel;
    QLabel *m_urlLabel;
};

#endif
