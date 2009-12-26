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

  QBoxLayout *buttonLayout = new QHBoxLayout;
  topLayout->addLayout( buttonLayout );

  // FIXME: Use proper icon
  m_backButton = new QPushButton( "<" );
  buttonLayout->addWidget( m_backButton );
  connect( m_backButton, SIGNAL( clicked() ), SLOT( showGroupList() ) );

  buttonLayout->addStretch( 1 );

  m_groupNameLabel = new QLabel;
  buttonLayout->addWidget( m_groupNameLabel );

  buttonLayout->addStretch( 1 );

  QPushButton *button = new QPushButton( i18n("New Person") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( newPerson() ) );

  m_viewLayout = new QStackedLayout;
  topLayout->addLayout( m_viewLayout );

  m_groupListView = new GroupListView;
  m_viewLayout->addWidget( m_groupListView );
  connect( m_groupListView, SIGNAL( groupClicked( const Identity & ) ),
    SLOT( showGroupView( const Identity & ) ) );

  m_groupView = new IdentityListView;
  m_viewLayout->addWidget( m_groupView );

  showGroupList();

  readData();

  KMessageBox::information( this, "FIXME: creation of ids on load" );
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
  m_backButton->setEnabled( false );
  m_groupNameLabel->setText( QString() );
}

void PolkaView::showGroupView( const Identity &group )
{
  m_groupView->setItemModel( m_model->itemModel( group.id() ) );
  m_groupNameLabel->setText( "<b>" + group.name().text() + "</b>" );

  m_viewLayout->setCurrentWidget( m_groupView );
  m_backButton->setEnabled( true );
}

#include "polkaview.moc"
