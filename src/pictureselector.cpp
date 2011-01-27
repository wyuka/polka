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

#include "pictureselector.h"

#include "pictureselectorbutton.h"
#include "polkamodel.h"

PictureSelector::PictureSelector( PolkaModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model ), m_layout( 0 )
{
}

void PictureSelector::setPictures( const Polka::Picture::List &pictures )
{
  delete m_layout;
  m_layout = new QHBoxLayout( this );

  foreach( Polka::Picture picture, pictures ) {
    PictureSelectorButton *button = new PictureSelectorButton( m_model );
    m_layout->addWidget( button );
    
    button->setPicture( picture );
  }

  QPushButton *addButton = new QPushButton( "+" );
  addButton->setFixedSize( 78, 78 );
  m_layout->addWidget( addButton );
  connect( addButton, SIGNAL( clicked() ), SIGNAL( grabPicture() ) );
}
