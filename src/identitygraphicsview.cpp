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
#include "trackinggraphicsview.h"
#include "mainmenuitem.h"
#include "magicmenuitem.h"

#include <KLocale>
#include <KInputDialog>
#include <KRandom>

#if QT_VERSION >= 0x040600
#include <QPropertyAnimation>
#endif

IdentityGraphicsView::IdentityGraphicsView( PolkaModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model ), m_mainMenu( 0 ), m_magicMenu( 0 ),
    m_compactLayout( false ),
    m_morphToAnimation( 0 ), m_morphFromAnimation( 0 ), m_globalMenu( 0 )
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

  m_scene = new QGraphicsScene;
//  m_scene->setBackgroundBrush( Qt::red );
  m_scene->setBackgroundBrush( QColor( 70,70,100 ) );
  m_scene->setSceneRect( -1000, -1000, 2000, 2000 );

  m_view = new TrackingGraphicsView( m_scene );
  m_view->setRenderHint( QPainter::Antialiasing );
  topLayout->addWidget( m_view );
  m_view->show();
  m_view->installEventFilter( this );
  connect( m_view, SIGNAL( mouseMoved( const QPoint & ) ),
    SLOT( slotMouseMoved( const QPoint & ) ) );
  connect( m_view, SIGNAL( viewportMoved() ), SLOT( positionAbsoluteItems() ) );

  connect( m_model, SIGNAL( identityInserted( const Polka::Identity & ) ),
    SLOT( createItems() ) );
  connect( m_model, SIGNAL( identityChanged( const Polka::Identity & ) ),
    SLOT( createItems() ) );
  connect( m_model, SIGNAL( identityRemoved( const Polka::Identity & ) ),
    SLOT( createItems() ) );

  setMinimumWidth( 50 );
}

void IdentityGraphicsView::setBackEnabled( bool enabled )
{
  m_backButton->setEnabled( enabled );
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
  m_labelItems.clear();
  m_globalMenu = 0;

  Polka::Identity::List identities = m_model->identitiesOfGroup( m_group );

  int columns = sqrt( identities.size() );
  int spacing = 150;

  int x = 0;
  int y = 0;

  qreal minX = 0;
  qreal minY = 0;
  qreal maxX = 0;
  qreal maxY = 0;

  bool firstItem = true;

  Polka::GroupView view = m_model->groupView( m_group );

  foreach( Polka::Identity identity, identities ) {
    qreal posX = x * spacing + ( y % 2 ) * spacing / 2;
    qreal posY = y * spacing * 0.866; // sin(60 degree)

    IdentityItem *item = new IdentityItem( m_model, identity );
    m_items.append( item );

    connect( item, SIGNAL( showIdentity( const Polka::Identity & ) ),
      SIGNAL( showIdentity( const Polka::Identity & ) ) );
    connect( item, SIGNAL( removeIdentity( const Polka::Identity & ) ),
      SLOT( slotRemoveIdentity( const Polka::Identity & ) ) );

    connect( item, SIGNAL( itemMoved( const Polka::Identity &, const QPointF & ) ),
      SLOT( savePosition( const Polka::Identity &, const QPointF & ) ) );
    connect( item, SIGNAL( itemChecked( const Polka::Identity &, bool ) ),
      SLOT( saveCheck( const Polka::Identity &, bool ) ) );

    connect( item, SIGNAL( menuShown() ), SLOT( hideGlobalMenu() ) );

    item->setDefaultPos( QPointF( posX, posY ) );

    qreal itemX;
    qreal itemY;

    Polka::IdentityPosition p = view.findIdentityPosition( identity.id() );
    if ( p.isValid() ) {
      itemX = p.x();
      itemY = p.y();
    } else {
      itemX = posX;
      itemY = posY;
    }
    item->setPos( itemX, itemY );

    if ( firstItem ) {
      firstItem = false;
    
      minX = itemX;
      minY = itemY;
      maxX = itemX;
      maxY = itemY;
    } else {
      if ( itemX < minX ) minX = itemX;
      if ( itemY < minY ) minY = itemY;
      if ( itemX > maxX ) maxX = itemX;
      if ( itemY > maxY ) maxY = itemY;
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

  
  m_magicMenu = new MagicMenuItem();
  m_scene->addItem( m_magicMenu );
  
  connect( m_magicMenu, SIGNAL( resetLayout() ), SLOT( resetLayout() ) );
  connect( m_magicMenu, SIGNAL( showSettings() ), SIGNAL( showSettings() ) );

  m_mainMenu = new MainMenuItem();
  m_scene->addItem( m_mainMenu );

  connect( m_mainMenu, SIGNAL( cloneGroup() ), SLOT( emitCloneGroup() ) );
  connect( m_mainMenu, SIGNAL( removeGroup() ), SLOT( emitRemoveGroup() ) );
  connect( m_mainMenu, SIGNAL( addGroup() ), SIGNAL( newGroup() ) );
  connect( m_mainMenu, SIGNAL( addPerson() ), SIGNAL( newPerson() ) );

  positionMenuItems();

  qreal centerX = ( minX + maxX ) / 2;
  qreal centerY = ( minY + maxY ) / 2;

  // TODO: Replace by a hidden setting
  if ( false ) {
    QGraphicsEllipseItem *centerItem = new QGraphicsEllipseItem( -5, -5, 10, 10 );
    centerItem->setBrush( Qt::red );
    centerItem->setZValue( 1000 );
    m_scene->addItem( centerItem );  
    centerItem->setPos( centerX, centerY );
  }

  m_view->centerOn( centerX, centerY );
}

void IdentityGraphicsView::positionMenuItems()
{
  QRect viewportRect = m_view->viewport()->rect();
  QPoint upperRight( viewportRect.width(), 0 );
  QPointF upperRightScene = m_view->mapToScene( upperRight );

  if ( m_mainMenu ) {
    m_mainMenu->setPos( upperRightScene.x() - 50, upperRightScene.y() + 50 );
  }
  if ( m_magicMenu ) {
    m_magicMenu->setPos( upperRightScene.x() - 50, upperRightScene.y() + 130 );
  }
}

void IdentityGraphicsView::positionAbsoluteItems()
{
  positionMenuItems();
}

Polka::Identity IdentityGraphicsView::group() const
{
  return m_group;
}

void IdentityGraphicsView::setGroupName( const QString &name )
{
  m_groupNameLabel->setText( "<b>" + name + "</b>" );
}

void IdentityGraphicsView::slotRemoveIdentity( const Polka::Identity &identity )
{
  emit removeIdentity( identity, m_group );
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
  addLabel( m_view->mapToScene( QPoint( 10, 10 ) ) );
}

void IdentityGraphicsView::addLabel( const QPointF &pos )
{
  bool ok;
  QString name = KInputDialog::getText( i18n("Add Label"),
    i18n("Enter text of label"), QString(),
    &ok );
  if ( ok ) {
    Polka::ViewLabel label;
    label.setId( KRandom::randomString( 10 ) );
    label.setText( name );

    label.setX( pos.x() );
    label.setY( pos.y() );
    
    createLabelItem( label );
    
    m_model->saveViewLabel( m_group, label );
  }
}

void IdentityGraphicsView::removeLabel( LabelItem *item,
  const Polka::ViewLabel &label )
{
  m_labelItems.removeAll( item );

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
  connect( item, SIGNAL( menuShown() ), SLOT( hideGlobalMenu() ) );

  m_scene->addItem( item );

  item->setPos( label.x(), label.y() );

  m_labelItems.append( item );

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
  m_mainMenu->hide();
  m_magicMenu->hide();
  foreach( LabelItem *item, m_labelItems ) {
    item->hide();
  }

  QRectF rect = m_scene->sceneRect();

  QPointF leftBorder = m_view->mapToScene( 0, 0 );

  int x = leftBorder.x() + 60;
  int y = leftBorder.y() + 60;
  int spacing = 60;

  if ( !m_morphToAnimation ) {
    m_morphToAnimation = new QParallelAnimationGroup( this );
    connect( m_morphToAnimation, SIGNAL( finished() ),
      SIGNAL( morphedToCompact() ) );
  }
  m_morphToAnimation->clear();

  foreach( IdentityItem *item, m_items ) {
    QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);
    m_morphToAnimation->insertAnimation( 0, animation );

    animation->setDuration(500);
    item->rememberPos( item->pos() );
    animation->setStartValue( item->pos() );
    QPointF target( x, y );
    animation->setEndValue( target );
    animation->setEasingCurve( QEasingCurve::OutCubic );
    
    animation = new QPropertyAnimation(item, "scale", this );
    m_morphToAnimation->insertAnimation( 0, animation );
    
    animation->setDuration( 300 );
    animation->setStartValue( item->scale() );
    animation->setEndValue( 0.5 );
    animation->setEasingCurve( QEasingCurve::OutCubic );
    
    y += spacing;
  }

  m_morphToAnimation->start();
}

void IdentityGraphicsView::morphFromCompact()
{
  if ( !m_morphFromAnimation ) {
    m_morphFromAnimation = new QParallelAnimationGroup( this );
    connect( m_morphFromAnimation, SIGNAL( finished() ),
      SIGNAL( morphedFromCompact() ) );
    connect( m_morphFromAnimation, SIGNAL( finished() ),
      SLOT( finishMorphFromCompact() ) );
  }
  m_morphFromAnimation->clear();

  foreach( IdentityItem *item, m_items ) {
    QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);
    m_morphFromAnimation->insertAnimation( 0, animation );

    animation->setDuration(500);
    animation->setStartValue( item->pos() );
    animation->setEndValue( item->rememberedPos() );
    animation->setEasingCurve( QEasingCurve::OutCubic );

    animation = new QPropertyAnimation(item, "scale", this );
    m_morphFromAnimation->insertAnimation( 0, animation );
    
    animation->setDuration( 300 );
    animation->setStartValue( item->scale() );
    animation->setEndValue( 1 );
    animation->setEasingCurve( QEasingCurve::OutCubic );
  }
  
  m_morphFromAnimation->start();
}

void IdentityGraphicsView::finishMorphFromCompact()
{
  m_mainMenu->show();
  m_magicMenu->show();

  foreach( LabelItem *item, m_labelItems ) {
    item->show();
  }  
}

IdentityItem *IdentityGraphicsView::item( const Polka::Identity &identity ) const
{
  foreach( IdentityItem *item, m_items ) {
    if ( item->identity().id() == identity.id() ) return item;
  }
  return 0;
}

bool IdentityGraphicsView::eventFilter( QObject *watched, QEvent *event )
{
  if ( watched == m_view ) {
    if ( event->type() == QEvent::MouseButtonPress ) {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>( event );

      if ( !m_globalMenu ) {
        m_globalMenu = new FanMenu( 0 );
        connect( m_globalMenu, SIGNAL( itemSelected( FanMenu::Item * ) ),
          SLOT( slotItemSelected( FanMenu::Item * ) ) );
        m_globalMenu->setZValue( 50 );
        m_addLabelItem = m_globalMenu->addItem( i18n("Add label") );
        m_globalMenu->setupItems();

        m_scene->addItem( m_globalMenu );
      }
      m_globalMenu->setPos( m_view->mapToScene( mouseEvent->pos() ) );
      m_globalMenu->show();
    } else if ( event->type() == QEvent::KeyPress ) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent*>( event );
      if ( keyEvent->key() == Qt::Key_Escape ) {
        hideGlobalMenu();
      }
    }
  }
  return QWidget::eventFilter( watched, event );
}

void IdentityGraphicsView::slotItemSelected( FanMenu::Item *item )
{
  hideGlobalMenu();

  if ( item == m_addLabelItem ) {
    addLabel( m_globalMenu->pos() );
  }
}

void IdentityGraphicsView::hideGlobalMenu()
{
  if ( m_globalMenu ) {
    m_globalMenu->hide();
  }
}

void IdentityGraphicsView::slotMouseMoved( const QPoint &pos )
{
  if ( !m_globalMenu || !m_globalMenu->isVisible() ) return;

  if ( !m_globalMenu->isCloseTo( m_view->mapToScene( pos ) ) ) {
    hideGlobalMenu();
  }
}
