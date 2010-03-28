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

#include "newpersondialog.h"

#include <KConfig>
#include <KMessageBox>
#include <KPushButton>

NewPersonDialog::NewPersonDialog( PolkaModel *model, QWidget *parent )
  : KDialog( parent ), m_model( model ), m_proxyModel( 0 ), m_matchList( 0 )
{
  setCaption( "New Person" );
  setButtons( KDialog::Ok | KDialog::Cancel );
  setModal( true );

  QWidget *topWidget = new QWidget;
  
  QBoxLayout *topLayout = new QVBoxLayout( topWidget );

  QLabel *label = new QLabel( "Enter name of new person" );
  topLayout->addWidget( label );
  
  m_nameInput = new QLineEdit;
  topLayout->addWidget( m_nameInput );
  connect( m_nameInput, SIGNAL( textChanged( const QString & ) ),
    SLOT( checkOkButton() ) );

  if ( !m_model->persons().isEmpty() ) {
    m_matchList = new QListView;
    
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setFilterCaseSensitivity( Qt::CaseInsensitive );

    topLayout->addWidget( m_matchList );
    connect( m_matchList, SIGNAL( activated( const QModelIndex & ) ),
      SLOT( accept() ) );

    m_proxyModel->setSourceModel( m_model->personsItemModel() );
    m_matchList->setModel( m_proxyModel );

    connect( m_nameInput, SIGNAL( textChanged( const QString & ) ),
      m_proxyModel, SLOT( setFilterWildcard( const QString & ) ) );
  }

  setMainWidget( topWidget );

  restoreDialogSize( KGlobal::config()->group("newpersondialog") );

  m_nameInput->setFocus();

  checkOkButton();
}

NewPersonDialog::~NewPersonDialog()
{
  KConfigGroup cg( KGlobal::config(), "newpersondialog" );
  saveDialogSize( cg );
}

Polka::Identity NewPersonDialog::identity()
{
  if ( !m_matchList || m_matchList->selectionModel()->selectedIndexes().isEmpty() ) {
    Polka::Identity identity;
    Polka::Name name;
    name.setText( m_nameInput->text() );
    identity.setName( name );
    return identity;
  } else {
    QModelIndexList selectedIndexes =
      m_matchList->selectionModel()->selectedIndexes();

    if ( selectedIndexes.count() > 1 ) {
      KMessageBox::information( 0, "More than one person selected" );
    }
    return m_model->findIdentity( m_proxyModel->data( selectedIndexes.first(),
      Qt::UserRole ).toString() );
  }
}

void NewPersonDialog::checkOkButton()
{
  button( Ok )->setEnabled( !m_nameInput->text().isEmpty() );
}
