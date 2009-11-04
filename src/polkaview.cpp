/*
 * polkaview.cpp
 *
 * Copyright (C) 2008 Cornelius Schumacher <schumacher@kde.org>
 */

#include "polkaview.h"

#include "polkamodel.h"

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

  buttonLayout->addStretch( 1 );

  QPushButton *button = new QPushButton( i18n("New Person") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( newPerson() ) );

  m_flatView = new QListView;
  topLayout->addWidget( m_flatView );

  readData();
}

PolkaView::~PolkaView()
{
}

void PolkaView::readData()
{
  m_model->readData();
  m_flatView->setModel( m_model->itemModel() );
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

#include "polkaview.moc"
