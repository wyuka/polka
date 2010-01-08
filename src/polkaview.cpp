/*
 * polkaview.cpp
 *
 * Copyright (C) 2009 Cornelius Schumacher <schumacher@kde.org>
 */

#include "polkaview.h"

#include "polkamodel.h"
#include "identitylistview.h"
#include "grouplistview.h"
#include "newpersondialog.h"
#include "settings.h"

#include <KMessageBox>
#include <KLocale>
#include <KInputDialog>

PolkaView::PolkaView(QWidget *parent)
  : QWidget( parent )
{
  m_model = new PolkaModel( this );
  connect( m_model, SIGNAL( dataWritten() ), SIGNAL( dataWritten() ) );
  
  QBoxLayout *topLayout = new QVBoxLayout( this );

  m_viewLayout = new QStackedLayout;
  topLayout->addLayout( m_viewLayout );

  m_groupListView = new GroupListView;
  m_viewLayout->addWidget( m_groupListView );
  connect( m_groupListView, SIGNAL( groupClicked( const Identity & ) ),
    SLOT( showGroupView( const Identity & ) ) );
  connect( m_groupListView, SIGNAL( newGroup() ), SLOT( newGroup() ) );

  m_groupView = new IdentityListView( m_model );
  m_viewLayout->addWidget( m_groupView );
  connect( m_groupView, SIGNAL( goBack() ), SLOT( showGroupList() ) );
  connect( m_groupView, SIGNAL( newPerson() ), SLOT( newPerson() ) );

  showGroupList();

  readData();
}

PolkaView::~PolkaView()
{
}

void PolkaView::readData()
{
  Settings::self()->readConfig();

  if ( !m_model->readData() ) {
    KMessageBox::error( this, i18n("Error reading data file") );
    return;
  }

  m_groupView->setItemModel( m_model->itemModel() );
  m_groupListView->setItemModel( m_model->groupItemModel() );

  if ( Settings::shownGroup().isEmpty() ) {
    showGroupList();
  } else {
    Identity group = m_model->identity( Settings::shownGroup() );
    showGroupView( group );
  }
}

void PolkaView::writeData()
{
  m_model->writeData();
}

void PolkaView::newGroup()
{
  bool ok;
  QString name = KInputDialog::getText( i18n("New Group"),
    i18n("Enter name of group to add"), QString(), &ok );
  if ( ok ) {
    Identity identity;
    identity.setDisplayName( name );
    
    m_model->insert( identity );
  }
}

void PolkaView::newPerson()
{
  NewPersonDialog *dialog = new NewPersonDialog( m_model, this );
  if ( dialog->exec() == QDialog::Accepted ) {
    Identity identity = dialog->identity();

    Groups groups;
    Group group;
    group.setId( m_groupView->group().id() );
    groups.addGroup( group );
    identity.setGroups( groups );
    
    m_model->insert( identity );
  }
  return;
}

void PolkaView::showGroupList()
{
  m_viewLayout->setCurrentWidget( m_groupListView );

  Settings::setShownGroup( QString() );
}

void PolkaView::showGroupView( const Identity &group )
{
  m_groupView->setGroup( group );

  m_viewLayout->setCurrentWidget( m_groupView );

  Settings::setShownGroup( group.id() );
}

#include "polkaview.moc"
