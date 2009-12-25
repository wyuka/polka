/*
 * polkaview.cpp
 *
 * Copyright (C) 2009 Cornelius Schumacher <schumacher@kde.org>
 */

#include "polkaview.h"

#include "polkamodel.h"
#include "identitylistview.h"
#include "grouplistview.h"

#include <klocale.h>
#include <kinputdialog.h>

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
  connect( m_groupListView, SIGNAL( showGroup( const QString & ) ),
    SLOT( showGroupView() ) );

  m_groupView = new IdentityListView;
  m_viewLayout->addWidget( m_groupView );

  showGroupList();

  readData();
}

PolkaView::~PolkaView()
{
}

void PolkaView::readData()
{
  m_model->readData();
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
    Name n;
    n.setText( name );
    identity.setName( n );
    
    m_model->insert( identity );
  }
}

void PolkaView::showGroupList()
{
  m_viewLayout->setCurrentWidget( m_groupListView );
  m_backButton->setEnabled( false );
  m_groupNameLabel->setText( QString() );
}

void PolkaView::showGroupView()
{
  m_viewLayout->setCurrentWidget( m_groupView );
  m_backButton->setEnabled( true );
  // FIXME: Set real name of group
  m_groupNameLabel->setText( "<b>Friends</b>" );
}

#include "polkaview.moc"
