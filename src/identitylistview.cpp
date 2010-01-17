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

#include "identitylistview.h"

#include "polkamodel.h"
#include "polkaitemmodel.h"

#include <KLocale>

IdentityListView::IdentityListView( PolkaModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model ), m_itemModel( 0 )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  QBoxLayout *buttonLayout = new QHBoxLayout;
  topLayout->addLayout( buttonLayout );

  // FIXME: Use proper icon
  m_backButton = new QPushButton( "<" );
  buttonLayout->addWidget( m_backButton );
  connect( m_backButton, SIGNAL( clicked() ), SIGNAL( goBack() ) );

  buttonLayout->addStretch( 1 );

  m_groupNameLabel = new QLabel;
  buttonLayout->addWidget( m_groupNameLabel );

  buttonLayout->addStretch( 1 );

  QPushButton *button = new QPushButton( i18n("New Person") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SIGNAL( newPerson() ) );
  
  m_flatView = new QListView;
  topLayout->addWidget( m_flatView );
  connect( m_flatView, SIGNAL( clicked( const QModelIndex & ) ),
    SLOT( slotItemClicked( const QModelIndex & ) ) );
}

void IdentityListView::setGroup( const Identity &group )
{
  m_group = group;

  setItemModel( m_model->itemModel( group.id() ) );
  setGroupName( group.displayName() );
}

Identity IdentityListView::group() const
{
  return m_group;
}

void IdentityListView::setItemModel( PolkaItemModel *itemModel )
{
  m_itemModel = itemModel;

  m_flatView->setModel( m_itemModel );
}

void IdentityListView::setGroupName( const QString &name )
{
  m_groupNameLabel->setText( "<b>" + name + "</b>" );
}

void IdentityListView::slotItemClicked( const QModelIndex &index )
{
  Identity identity = m_itemModel->identity( index );

  emit showPerson( identity );
}
