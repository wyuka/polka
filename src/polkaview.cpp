/*
 * polkaview.cpp
 *
 * Copyright (C) 2009 Cornelius Schumacher <schumacher@kde.org>
 */

#include "polkaview.h"

#include "polkamodel.h"
#include "identitylistview.h"
#include "grouplistview.h"

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

  m_groupView = new IdentityListView;
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
  if ( !m_model->readData() ) {
    KMessageBox::error( this, i18n("Error reading data file") );
    return;
  }

  m_groupView->setItemModel( m_model->itemModel() );
  m_groupListView->setItemModel( m_model->groupItemModel() );
}

void PolkaView::writeData()
{
  m_model->writeData();
}

void PolkaView::newPerson()
{
  bool ok;
  QString name = KInputDialog::getText( i18n("New Person"),
    i18n("Enter name of person to add"), QString(), &ok );
  if ( ok ) {
    Identity identity;
    identity.setDisplayName( name );
    
    m_model->insert( identity );
  }
}

void PolkaView::showGroupList()
{
  m_viewLayout->setCurrentWidget( m_groupListView );
}

void PolkaView::showGroupView( const Identity &group )
{
  m_groupView->setItemModel( m_model->itemModel( group.id() ) );
  m_groupView->setGroupName( group.displayName() );

  m_viewLayout->setCurrentWidget( m_groupView );
}

#include "polkaview.moc"
