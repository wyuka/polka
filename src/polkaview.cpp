/*
 * polkaview.cpp
 *
 * Copyright (C) 2009 Cornelius Schumacher <schumacher@kde.org>
 */

#include "polkaview.h"

#include "polkamodel.h"
#include "identitylistview.h"
#include "identitygraphicsview.h"
#include "grouplistview.h"
#include "newpersondialog.h"
#include "settings.h"
#include "gitremote.h"
#include "personview.h"

#include <KMessageBox>
#include <KLocale>
#include <KInputDialog>

PolkaView::PolkaView(QWidget *parent)
  : QWidget( parent )
{
  m_model = new PolkaModel( this );
  connect( m_model, SIGNAL( dataWritten() ), SIGNAL( dataWritten() ) );
  
  QBoxLayout *topLayout = new QVBoxLayout( this );

  QBoxLayout *controlLayout = new QHBoxLayout;
  topLayout->addLayout( controlLayout );

  m_syncingCheck = new QCheckBox( i18n("Syncing enabled") );
  controlLayout->addWidget( m_syncingCheck );
  connect( m_syncingCheck, SIGNAL( stateChanged( int ) ),
    SLOT( slotSyncingCheckChanged() ) );

  QLabel *label = new QLabel;
  controlLayout->addWidget( label );
  connect( m_model->gitRemote(), SIGNAL( statusChanged( const QString & ) ),
    label, SLOT( setText( const QString & ) ) );

  controlLayout->addStretch( 1 );

  m_graphicsModeCheck = new QCheckBox( i18n("Use fancy view") );
  controlLayout->addWidget( m_graphicsModeCheck );
  connect( m_graphicsModeCheck, SIGNAL( stateChanged( int ) ),
    SLOT( showView() ) );

  m_viewLayout = new QStackedLayout;
  topLayout->addLayout( m_viewLayout );

  m_groupListView = new GroupListView;
  m_viewLayout->addWidget( m_groupListView );
  connect( m_groupListView, SIGNAL( groupClicked( const Polka::Identity & ) ),
    SLOT( showGroupView( const Polka::Identity & ) ) );
  connect( m_groupListView, SIGNAL( newGroup() ), SLOT( newGroup() ) );

  m_groupView = new IdentityListView( m_model );
  m_viewLayout->addWidget( m_groupView );
  connect( m_groupView, SIGNAL( goBack() ), SLOT( showGroupList() ) );
  connect( m_groupView, SIGNAL( newPerson() ), SLOT( newPerson() ) );
  connect( m_groupView, SIGNAL( showPerson( const Polka::Identity & ) ),
    SLOT( showPerson( const Polka::Identity & ) ) );

  m_groupGraphicsView = new IdentityGraphicsView( m_model );
  m_viewLayout->addWidget( m_groupGraphicsView );
  connect( m_groupGraphicsView, SIGNAL( goBack() ), SLOT( showGroupList() ) );
  connect( m_groupGraphicsView, SIGNAL( newPerson() ), SLOT( newPerson() ) );
  connect( m_groupGraphicsView, SIGNAL( showPerson( const Polka::Identity & ) ),
    SLOT( showPerson( const Polka::Identity & ) ) );
  connect( m_groupGraphicsView, SIGNAL( removePerson( const Polka::Identity &,
    const Polka::Identity & ) ),
    SLOT( removePerson( const Polka::Identity &, const Polka::Identity & ) ) );
  connect( m_groupGraphicsView, SIGNAL( cloneGroup( const Polka::Identity & ) ),
    SLOT( cloneGroup( const Polka::Identity & ) ) );
  connect( m_groupGraphicsView, SIGNAL( removeGroup( const Polka::Identity & ) ),
    SLOT( removeGroup( const Polka::Identity & ) ) );

  readConfig();

  showGroupList();

  readData();
}

PolkaView::~PolkaView()
{
}

void PolkaView::readConfig()
{
  Settings::self()->readConfig();

  m_syncingCheck->setChecked( Settings::remoteSyncingEnabled() );
  m_graphicsModeCheck->setChecked( Settings::fancyMode() );
}

void PolkaView::writeConfig()
{
  Settings::setFancyMode( m_graphicsModeCheck->isChecked() );
  Settings::setShownGroup( m_group.id() );

  Settings::self()->writeConfig();
}

void PolkaView::slotSyncingCheckChanged()
{
  Settings::setRemoteSyncingEnabled( m_syncingCheck->isChecked() );
}

void PolkaView::readData()
{
  if ( !m_model->readData() ) {
    KMessageBox::error( this, i18n("Error reading data file") );
    return;
  }

  m_groupView->setItemModel( m_model->itemModel() );
  m_groupListView->setItemModel( m_model->groupItemModel() );

  if ( Settings::shownGroup().isEmpty() ) {
    showGroupList();
  } else {
    Polka::Identity group = m_model->findIdentity( Settings::shownGroup() );
    showGroupView( group );
  }

  if ( Settings::remoteSyncingEnabled() ) {
    m_model->gitRemote()->pull();
  }
}

void PolkaView::writeData()
{
  m_model->writeData();

  if ( Settings::remoteSyncingEnabled() ) {
    m_model->gitRemote()->push();
  }
}

void PolkaView::newGroup()
{
  bool ok;
  QString name = KInputDialog::getText( i18n("New Group"),
    i18n("Enter name of group to add"), QString(), &ok );
  if ( ok ) {
    Polka::Identity identity;
    Polka::Name n;
    n.setText( name );
    identity.setName( n );
    
    m_model->insert( identity );
  }
}

void PolkaView::cloneGroup( const Polka::Identity &group )
{
  bool ok;
  QString name = KInputDialog::getText( i18n("Clone Group"),
    i18n("Enter name of new group"),
    i18n("Clone of %1").arg( group.name().text() ),
    &ok );
  if ( ok ) {
    Polka::Identity identity;
    Polka::Name n;
    n.setText( name );
    identity.setName( n );
    Polka::Identity group = m_model->insert( identity );
    
    showGroupView( group );
  }
}

void PolkaView::removeGroup( const Polka::Identity &group )
{
  m_model->removeGroup( group );
  showGroupList();
}

void PolkaView::newPerson()
{
  NewPersonDialog *dialog = new NewPersonDialog( m_model, this );
  if ( dialog->exec() == QDialog::Accepted ) {
    Polka::Identity identity = dialog->identity();

    m_model->addPerson( identity, m_group );
  }
  return;
}

void PolkaView::showGroupList()
{
  m_group = Polka::Identity();

  m_viewLayout->setCurrentWidget( m_groupListView );
}

void PolkaView::showGroupView( const Polka::Identity &group )
{
  m_group = group;

  if ( m_graphicsModeCheck->isChecked() ) {
    m_groupGraphicsView->setGroup( group );
    m_viewLayout->setCurrentWidget( m_groupGraphicsView );
  } else {
    m_groupView->setGroup( group );
    m_viewLayout->setCurrentWidget( m_groupView );
  }
}

void PolkaView::showView()
{
  if ( m_group.id().isEmpty() ) {
    showGroupList();
  } else {
    showGroupView( m_group );
  }
}

void PolkaView::showPerson( const Polka::Identity &identity )
{
  KDialog *dialog = new KDialog( this );
  dialog->setButtons( KDialog::Ok );
  PersonView *view = new PersonView( m_model );
  view->showIdentity( identity );
  dialog->setMainWidget( view );
  dialog->show();
}

void PolkaView::removePerson( const Polka::Identity &identity,
  const Polka::Identity &group )
{
  m_model->removePerson( identity, group );
}

#include "polkaview.moc"
