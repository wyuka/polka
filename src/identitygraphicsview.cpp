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

#include "identitygraphicsview.h"

#include "polkamodel.h"
#include "polkaitemmodel.h"
#include "personview.h"
#include "identityitem.h"

#include <KLocale>

#if QT_VERSION >= 0x040600
#include <QPropertyAnimation>
#endif

IdentityGraphicsView::IdentityGraphicsView( PolkaModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model )
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

  m_scene = new QGraphicsScene;
//  m_scene->setBackgroundBrush( Qt::red );
  m_scene->setBackgroundBrush( QColor( 70,70,100 ) );

  QGraphicsView *view = new QGraphicsView( m_scene );
  view->setRenderHint( QPainter::Antialiasing );
  topLayout->addWidget( view );
  view->show();

  buttonLayout = new QHBoxLayout;
  topLayout->addLayout( buttonLayout );
  
  buttonLayout->addStretch( 1 );
  
  button = new QPushButton( i18n("Reset Layout") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( resetLayout() ) );

  connect( m_model, SIGNAL( identityInserted( const Polka::Identity & ) ),
    SLOT( createItems() ) );
  connect( m_model, SIGNAL( identityChanged( const Polka::Identity & ) ),
    SLOT( createItems() ) );
  connect( m_model, SIGNAL( identityRemoved( const Polka::Identity & ) ),
    SLOT( createItems() ) );
}

void IdentityGraphicsView::setGroup( const Polka::Identity &group )
{
  m_group = group;

  setGroupName( group.name().text() );

  createItems();
}

void IdentityGraphicsView::createItems()
{
  m_scene->clear();
  m_items.clear();

  Polka::Identity::List identities = m_model->identitiesOfGroup( m_group );

  int columns = sqrt( identities.size() );
  int spacing = 150;

  int x = 0;
  int y = 0;

  Polka::GroupView view = m_model->groupView( m_group );

  foreach( Polka::Identity identity, identities ) {
    qreal posX = x * spacing + ( y % 2 ) * spacing / 2;
    qreal posY = y * spacing * 0.866; // sin(60 degree)

    IdentityItem *item = new IdentityItem( m_model, identity );
    m_items.append( item );

    connect( item, SIGNAL( showPerson( const Polka::Identity & ) ),
      SIGNAL( showPerson( const Polka::Identity & ) ) );
    connect( item, SIGNAL( removePerson( const Polka::Identity & ) ),
      SLOT( slotRemovePerson( const Polka::Identity & ) ) );

    connect( item, SIGNAL( itemMoved( const Polka::Identity &, const QPointF & ) ),
      SLOT( savePosition( const Polka::Identity &, const QPointF & ) ) );

    item->setDefaultPos( QPointF( posX, posY ) );

    Polka::IdentityPosition p = view.findIdentityPosition( identity.id() );
    if ( p.isValid() ) {
      item->setPos( p.x(), p.y() );
    } else {
      item->setPos( posX, posY );
    }
    m_scene->addItem( item );

    x++;
    
    if ( x >= ( columns + ( y + 1 ) % 2 ) ) {
      x = 0;
      y++;
    }
  }
}

Polka::Identity IdentityGraphicsView::group() const
{
  return m_group;
}

void IdentityGraphicsView::setGroupName( const QString &name )
{
  m_groupNameLabel->setText( "<b>" + name + "</b>" );
}

void IdentityGraphicsView::slotRemovePerson( const Polka::Identity &identity )
{
  emit removePerson( identity, m_group );
}

void IdentityGraphicsView::savePosition( const Polka::Identity &identity,
  const QPointF &pos )
{
  m_model->saveViewPosition( m_group, identity, pos );
}

void IdentityGraphicsView::resetLayout()
{
  m_model->clearViewPositions( m_group );

  foreach( IdentityItem *item, m_items ) {
    if ( item->pos() != item->defaultPos() ) {
#if QT_VERSION >= 0x040600
      QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);

      animation->setDuration(500);
      animation->setStartValue( item->pos() );
      animation->setEndValue( item->defaultPos() );
      animation->setEasingCurve( QEasingCurve::OutCubic );

      animation->start();
#else
      item->setPos( item->defaultPos() );
#endif
    }
  }
}
