/*
    This file is part of KDE.

    Copyright (C) 2009-2011 Cornelius Schumacher <schumacher@kde.org>

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

#include "polkaview.h"

#include "polkamodel.h"
#include "grouplistview.h"
#include "groupgraphicsview.h"
#include "newpersondialog.h"
#include "newgroupdialog.h"
#include "settings.h"
#include "gitremote.h"
#include "personview.h"
#include "settingswidget.h"
#include "overview.h"
#include "historyview.h"

#include <KMessageBox>
#include <KLocale>
#include <KInputDialog>

PolkaView::PolkaView(QWidget *parent)
  : QWidget( parent )
{
  m_model = new PolkaModel( this );
  connect( m_model, SIGNAL( dataWritten() ), SIGNAL( dataWritten() ) );
  
  QBoxLayout *topLayout = new QVBoxLayout( this );

  
  QBoxLayout *buttonLayout = new QHBoxLayout;
  topLayout->addLayout( buttonLayout );

  // FIXME: Use proper icon
  m_backButton = new QPushButton( "<" );
  buttonLayout->addWidget( m_backButton );
  connect( m_backButton, SIGNAL( clicked() ), SLOT( goBack() ) );
  m_backButton->setEnabled( false );

  buttonLayout->addStretch( 1 );

  m_groupNameLabel = new QLabel;
  buttonLayout->addWidget( m_groupNameLabel );

  buttonLayout->addStretch( 1 );

  QPushButton *button = new QPushButton( i18n("...") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( showOverview() ) );

  QBoxLayout *viewLayout = new QHBoxLayout;
  topLayout->addLayout( viewLayout );

  m_groupWidget = new QWidget;
  viewLayout->addWidget( m_groupWidget );
  
  m_listLayout = new QStackedLayout( m_groupWidget );

  m_overview = new Overview;
  m_listLayout->addWidget( m_overview );
  connect( m_overview, SIGNAL( showGroupView() ), SLOT( showGroupView() ) );
  connect( m_overview, SIGNAL( showListView() ), SLOT( showListView() ) );
  connect( m_overview, SIGNAL( showHistory() ), SLOT( showHistory() ) );

  m_groupListView = new GroupListView( m_model );
  m_listLayout->addWidget( m_groupListView );
  connectGroupView( m_groupListView );

  m_groupGraphicsView = new GroupGraphicsView( m_model );
  m_listLayout->addWidget( m_groupGraphicsView );
  connectGroupView( m_groupGraphicsView );
  connect( m_groupGraphicsView, SIGNAL( newGroup() ), SLOT( newSubGroup() ) );
  connect( m_groupGraphicsView, SIGNAL( removeIdentity( const Polka::Identity &,
    const Polka::Identity & ) ),
    SLOT( removeIdentity( const Polka::Identity &, const Polka::Identity & ) ) );
  connect( m_groupGraphicsView, SIGNAL( cloneGroup( const Polka::Identity & ) ),
    SLOT( cloneGroup( const Polka::Identity & ) ) );
  connect( m_groupGraphicsView, SIGNAL( removeGroup( const Polka::Identity & ) ),
    SLOT( removeGroup( const Polka::Identity & ) ) );
  connect( m_groupGraphicsView, SIGNAL( morphedToCompact() ),
    SLOT( finishShowPerson() ) );
  connect( m_groupGraphicsView, SIGNAL( closeRequested() ),
    SLOT( closePersonView() ) );

  m_personView = new PersonView( m_model );
  viewLayout->addWidget( m_personView );
  connect( m_personView, SIGNAL( closeRequested() ),
    SLOT( closePersonView() ) );

  m_historyView = new HistoryView( m_model );
  m_listLayout->addWidget( m_historyView );

  m_settingsWidget = new SettingsWidget( m_model );
  topLayout->addWidget( m_settingsWidget );
  connect( m_settingsWidget, SIGNAL( showView() ), SLOT( showView() ) );

  m_settingsWidget->hide();

  readConfig();

  readData();
}

PolkaView::~PolkaView()
{
}

void PolkaView::connectGroupView( GroupView *groupView )
{
  connect( groupView, SIGNAL( goBack() ), SLOT( goBack() ) );
  connect( groupView, SIGNAL( newPerson() ), SLOT( newPerson() ) );
  connect( groupView, SIGNAL( showIdentity( const Polka::Identity & ) ),
    SLOT( showIdentity( const Polka::Identity & ) ) );
  connect( groupView, SIGNAL( showSettings() ),
    SLOT( showSettings() ) );
}

void PolkaView::readConfig()
{
  Settings::self()->readConfig();

  m_settingsWidget->readConfig();
}

void PolkaView::writeConfig()
{
  m_settingsWidget->writeConfig();

  Settings::setHistory( m_history );

  Settings::self()->writeConfig();
}

void PolkaView::readData()
{
  if ( !m_model->readData() ) {
    KMessageBox::error( this, i18n("Error reading data file") );
    return;
  }

  m_groupListView->setItemModel( m_model->itemModel() );

  m_history = Settings::history();

  if ( m_history.isEmpty() ) {
    showRoot();
  } else {
    Polka::Identity group = m_model->findIdentity( m_history.last() );
    showGroup( group );
  }

  if ( Settings::remoteSyncingEnabled() ) {
    m_model->gitRemote()->pull();
  }
}

void PolkaView::writeData( const QString &msg )
{
  m_model->writeData( msg );

  if ( Settings::remoteSyncingEnabled() ) {
    m_model->gitRemote()->push();
  }
}

void PolkaView::newSubGroup()
{
  NewGroupDialog *dialog = new NewGroupDialog( m_model, this );
  if ( dialog->exec() == QDialog::Accepted ) {
    Polka::Identity group = dialog->identity();
    m_model->addIdentity( group, m_group );
  }
  return;
}

void PolkaView::cloneGroup( const Polka::Identity &group )
{
  bool ok;
  QString name = KInputDialog::getText( i18n("Clone Group"),
    i18n("Enter name of new group"),
    i18n("Clone of %1").arg( group.name().value() ),
    &ok );
  if ( ok ) {
    Polka::Identity identity;
    identity.setType( "group" );
    Polka::Name n;
    n.setValue( name );
    identity.setName( n );
    Polka::Identity new_group = m_model->insert( identity,
      i18n("Clone group '%1' to '%2'").arg( group.name().value() )
        .arg( name ) );

    Polka::Identity::List members = m_model->identitiesOfGroup( group );
    foreach( Polka::Identity member, members ) {
      m_model->addIdentity( member, new_group );
    }

    m_model->addIdentity( new_group, m_group );
    
    showGroup( new_group );
  }
}

void PolkaView::removeGroup( const Polka::Identity &group )
{
  m_model->removeGroup( group );
  goBack();
}

void PolkaView::newPerson()
{
  NewPersonDialog *dialog = new NewPersonDialog( m_model, this );
  if ( dialog->exec() == QDialog::Accepted ) {
    Polka::Identity identity = dialog->identity();

    m_model->addIdentity( identity, m_group );
  }
  return;
}

void PolkaView::showRoot()
{
  showGroup( m_model->rootGroup() );
}

void PolkaView::showGroup( const Polka::Identity &group )
{
  m_groupWidget->setMaximumWidth( QWIDGETSIZE_MAX );
  m_personView->hide();

  m_group = group;

  // Return to event loop, so layout gets adjusted before new group is rendered.
  QTimer::singleShot( 0, this, SLOT( continueShowGroup() ) );
}

void PolkaView::continueShowGroup()
{
  if ( m_history.isEmpty() || m_history.last() != m_group.id() ) {
    m_history.append( m_group.id() );
  }

  m_backButton->setEnabled( m_history.size() > 1 );
  m_groupNameLabel->setText( "<b>" + m_group.name().value() + "</b>" );

  if ( m_settingsWidget->fancyMode() ) {
    m_groupGraphicsView->showGroup( m_group );
    m_listLayout->setCurrentWidget( m_groupGraphicsView );
  } else {
    m_groupListView->showGroup( m_group );
    m_listLayout->setCurrentWidget( m_groupListView );
  }
}

void PolkaView::showView()
{
  if ( m_group.id().isEmpty() ) {
    showRoot();
  } else {
    showGroup( m_group );
  }
  m_groupGraphicsView->setCompactLayout( false );
}

void PolkaView::showIdentity( const Polka::Identity &identity )
{
  if ( identity.type() == "group" ) {
    showGroup( identity );
  } else {
    showPerson( identity );
  }
}

void PolkaView::showPerson( const Polka::Identity &identity )
{
#if 0
  KDialog *dialog = new KDialog( this );
  dialog->setButtons( KDialog::Ok );
  PersonView *view = new PersonView( m_model );
  view->showIdentity( identity );
  dialog->setMainWidget( view );
  dialog->show();
#else
  m_personView->showIdentity( identity );
  m_groupGraphicsView->setCompactLayout( true );
#endif
}

void PolkaView::finishShowPerson()
{
  m_personView->show();

  m_groupWidget->setMaximumWidth( 170 );

  m_backButton->setEnabled( true );
}

void PolkaView::removeIdentity( const Polka::Identity &identity,
  const Polka::Identity &group )
{
  m_model->removeIdentity( identity, group );
}

void PolkaView::closePersonView()
{
  m_groupWidget->setMaximumWidth( QWIDGETSIZE_MAX );
  m_personView->hide();
  m_groupGraphicsView->setCompactLayout( false );
}

void PolkaView::showSettings()
{
  if ( m_settingsWidget->isVisible() ) {
    m_settingsWidget->hide();
  } else {
    m_settingsWidget->show();
  }
}

void PolkaView::goBack()
{
  if ( m_personView->isVisible() ) {
    closePersonView();
    m_backButton->setEnabled( m_history.size() > 1 );
    return;
  }

  m_history.takeLast();
  while ( !m_history.isEmpty() ) {
    QString id = m_history.last();
    Polka::Identity group = m_model->findIdentity( id );
    if ( group.isValid() && group.type() == "group" ) {
      showGroup( group );
      return;
    }
    m_history.takeLast();
  }
  showRoot();
}

void PolkaView::showOverview()
{
  m_backButton->hide();
  m_groupNameLabel->setText( QString() );
  m_listLayout->setCurrentWidget( m_overview );
}

void PolkaView::showGroupView()
{
  m_backButton->show();
  showView();
}

void PolkaView::showListView()
{
  m_backButton->show();
  showView();
}

void PolkaView::showHistory()
{
  m_backButton->hide();
  m_groupNameLabel->setText( i18n("<b>History</b>") );
  m_listLayout->setCurrentWidget( m_historyView );

  m_historyView->loadHistory();
}

#include "polkaview.moc"
