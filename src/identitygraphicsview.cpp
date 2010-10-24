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
#include "labelitem.h"

#include <KLocale>
#include <KInputDialog>
#include <KRandom>

#if QT_VERSION >= 0x040600
#include <QPropertyAnimation>
#endif

IdentityGraphicsView::IdentityGraphicsView( PolkaModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model ), m_compactLayout( false ),
    m_morphAnimationGroup( 0 )
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

  QPushButton *button = new QPushButton( i18n("Clone Group") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( emitCloneGroup() ) );

  button = new QPushButton( i18n("Remove Group") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( emitRemoveGroup() ) );

  button = new QPushButton( i18n("Add Label") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( addLabel() ) );

  button = new QPushButton( i18n("New Person") );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SIGNAL( newPerson() ) );

  m_scene = new QGraphicsScene;
//  m_scene->setBackgroundBrush( Qt::red );
  m_scene->setBackgroundBrush( QColor( 70,70,100 ) );

  m_view = new QGraphicsView( m_scene );
  m_view->setRenderHint( QPainter::Antialiasing );
  topLayout->addWidget( m_view );
  m_view->show();

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

  setMinimumWidth( 50 );
}

void IdentityGraphicsView::setCompactLayout( bool enabled )
{
  if ( enabled == m_compactLayout ) return;

  m_compactLayout = enabled;

  if ( m_compactLayout ) morphToCompact();
  else morphFromCompact();
}

void IdentityGraphicsView::setGroup( const Polka::Identity &group )
{
  m_group = group;

  setGroupName( group.name().value() );

  createItems();
}

void IdentityGraphicsView::createItems()
{
  m_compactLayout = false;

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
    connect( item, SIGNAL( itemChecked( const Polka::Identity &, bool ) ),
      SLOT( saveCheck( const Polka::Identity &, bool ) ) );

    item->setDefaultPos( QPointF( posX, posY ) );

    Polka::IdentityPosition p = view.findIdentityPosition( identity.id() );
    if ( p.isValid() ) {
      item->setPos( p.x(), p.y() );
    } else {
      item->setPos( posX, posY );
    }

    Polka::IdentityCheck c = view.findIdentityCheck( identity.id() );
    if ( c.isValid() ) {
      item->checkItem();
    }

    m_scene->addItem( item );

    x++;
    
    if ( x >= ( columns + ( y + 1 ) % 2 ) ) {
      x = 0;
      y++;
    }
  }
  
  foreach( Polka::ViewLabel label, view.viewLabelList() ) {
    createLabelItem( label );
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
  if ( !m_compactLayout ) {
    m_model->saveViewPosition( m_group, identity, pos );
  }
}

void IdentityGraphicsView::saveLabel( const Polka::ViewLabel &label,
  const QPointF &pos )
{
  Polka::ViewLabel l = label;
  l.setX( pos.x() );
  l.setY( pos.y() );
  m_model->saveViewLabel( m_group, l );
}


void IdentityGraphicsView::saveCheck( const Polka::Identity &identity,
  bool checked )
{
  m_model->saveViewCheck( m_group, identity, checked );
}

void IdentityGraphicsView::addLabel()
{
  bool ok;
  QString name = KInputDialog::getText( i18n("Add Label"),
    i18n("Enter text of label"), QString(),
    &ok );
  if ( ok ) {
    Polka::ViewLabel label;
    label.setId( KRandom::randomString( 10 ) );
    label.setText( name );
    
    QPointF pos = m_view->mapToScene( QPoint( 10, 10 ) );
    label.setX( pos.x() );
    label.setY( pos.y() );

    createLabelItem( label );
    
    m_model->saveViewLabel( m_group, label );
  }
}

void IdentityGraphicsView::removeLabel( LabelItem *item,
  const Polka::ViewLabel &label )
{
  delete item;
  m_model->removeViewLabel( m_group, label );
}

void IdentityGraphicsView::renameLabel( LabelItem *item,
  Polka::ViewLabel label )
{
  bool ok;
  QString name = KInputDialog::getText( i18n("Rename Label"),
    i18n("Enter new text of label"), label.text(),
    &ok );
  if ( ok ) {
    item->setText( name );
  
    label.setText( name );
    m_model->saveViewLabel( m_group, label );
  }
}

LabelItem *IdentityGraphicsView::createLabelItem( const Polka::ViewLabel &label )
{
  LabelItem *item = new LabelItem( m_model, label );

  connect( item, SIGNAL( itemMoved( const Polka::ViewLabel &, const QPointF & ) ),
    SLOT( saveLabel( const Polka::ViewLabel &, const QPointF & ) ) );
  connect( item, SIGNAL( removeLabel( LabelItem *, const Polka::ViewLabel & ) ),
    SLOT( removeLabel( LabelItem *, const Polka::ViewLabel & ) ) );
  connect( item, SIGNAL( renameLabel( LabelItem *, const Polka::ViewLabel & ) ),
    SLOT( renameLabel( LabelItem *, const Polka::ViewLabel & ) ) );

  m_scene->addItem( item );

  item->setPos( label.x(), label.y() );

  return item;
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

void IdentityGraphicsView::emitCloneGroup()
{
  emit cloneGroup( m_group );
}

void IdentityGraphicsView::emitRemoveGroup()
{
  emit removeGroup( m_group );
}

void IdentityGraphicsView::morphToCompact()
{
  int x = 0;
  int y = 0;
  int spacing = 20;

  if ( !m_morphAnimationGroup ) {
    m_morphAnimationGroup = new QParallelAnimationGroup( this );
    connect( m_morphAnimationGroup, SIGNAL( finished() ),
      SIGNAL( morphedToCompact() ) );
  }
  m_morphAnimationGroup->clear();

  foreach( IdentityItem *item, m_items ) {
    QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);
    m_morphAnimationGroup->insertAnimation( 0, animation );

    animation->setDuration(500);
    animation->setStartValue( item->pos() );
    animation->setEndValue( QPoint( x, y ) );
    animation->setEasingCurve( QEasingCurve::OutCubic );
    
    y += spacing;
  }

  m_morphAnimationGroup->start();
}

void IdentityGraphicsView::morphFromCompact()
{
}
