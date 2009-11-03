/*
 * polkaview.cpp
 *
 * Copyright (C) 2008 Cornelius Schumacher <schumacher@kde.org>
 */

#include "polkaview.h"

#include "polkamodel.h"

#include <klocale.h>

PolkaView::PolkaView(QWidget *parent)
  : QWidget( parent )
{
  m_model = new PolkaModel( this );
  connect( m_model, SIGNAL( dataWritten() ), SIGNAL( dataWritten() ) );
  
  QBoxLayout *topLayout = new QVBoxLayout( this );

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

#include "polkaview.moc"
