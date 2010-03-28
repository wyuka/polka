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

#include "pictureselectorbutton.h"

#include "imageloader.h"

PictureSelectorButton::PictureSelectorButton( QWidget *parent )
  : QWidget( parent )
{
  QBoxLayout *topLayout = new QHBoxLayout( this );

  m_label = new QLabel;
  topLayout->addWidget( m_label );
}

void PictureSelectorButton::setPicture( const Polka::Picture &picture )
{
  m_picture = picture;
  connect( ImageLoader::load( m_picture.url() ),
    SIGNAL( loaded( const QPixmap & ) ), SLOT( setPixmap( const QPixmap & ) ) );
}

Polka::Picture PictureSelectorButton::picture() const
{
  return m_picture;
}

void PictureSelectorButton::setPixmap( const QPixmap &pixmap )
{
  m_label->setPixmap( pixmap );
}
