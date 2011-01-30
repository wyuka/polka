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

#include "groupgraphicsview.h"

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

GroupGraphicsView::GroupGraphicsView( PolkaModel *model, QWidget *parent )
  : GroupView( model, parent ), m_mainMenu( 0 ), m_magicMenu( 0 ),
    m_compactLayout( false ),
    m_morphToAnimation( 0 ), m_morphFromAnimation( 0 ),
    m_removeItemsAnimation( 0 ), m_placeItemsAnimation( 0 ), 
    m_unplaceItemsAnimation( 0 ), m_unhideItemsAnimation( 0 ), m_globalMenu( 0 )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

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

  connect( model, SIGNAL( identityAdded( const Polka::Identity & ) ),
    SLOT( slotIdentityAdded( const Polka::Identity & ) ) );
  connect( model, SIGNAL( identityChanged( const Polka::Identity & ) ),
    SLOT( slotIdentityChanged( const Polka::Identity & ) ) );
  connect( model, SIGNAL( identityRemoved( const Polka::Identity & ) ),
    SLOT( slotIdentityRemoved( const Polka::Identity & ) ) );

  setMinimumWidth( 50 );
}

void GroupGraphicsView::slotIdentityChanged( const Polka::Identity &identity )
{
  IdentityItem *i = item( identity );
    
  if ( identity.groups().findGroup( group().id() ).isValid() && i ) {
    i->updateItem( identity );
  } else {
    recreateItems();
  }
}

void GroupGraphicsView::slotIdentityAdded( const Polka::Identity &identity )
{
  Q_UNUSED( identity )

  recreateItems();
}

void GroupGraphicsView::slotIdentityRemoved( const Polka::Identity &identity )
{
  Q_UNUSED( identity )

  recreateItems();
}

void GroupGraphicsView::recreateItems()
{
  m_previousItem = 0;
  placeItems();
}

void GroupGraphicsView::setCompactLayout( bool enabled )
{
  if ( enabled == m_compactLayout ) return;

  m_compactLayout = enabled;

  if ( m_compactLayout ) morphToCompact();
  else morphFromCompact();
}

void GroupGraphicsView::doShowGroup()
{
  m_previousItem = 0;

  if ( m_removeItemsAnimation ) m_removeItemsAnimation->stop();
  if ( m_placeItemsAnimation ) m_placeItemsAnimation->stop();
  if ( m_unplaceItemsAnimation ) m_unplaceItemsAnimation->stop();
  if ( m_unhideItemsAnimation ) m_unhideItemsAnimation->stop();

  if ( group().isValid() ) {
    m_previousItem = item( group() );
  }

  if ( m_previousItem ) {
    hideItems();
  } else {
    if ( m_items.isEmpty() ) {
      placeItems();
    } else {
      unplaceItems();
    }
  }
}

void GroupGraphicsView::hideItems()
{
  if ( !m_removeItemsAnimation ) {
    m_removeItemsAnimation = new QParallelAnimationGroup( this );
    connect( m_removeItemsAnimation, SIGNAL( finished() ),
      SLOT( placeItems() ) );
  }
  m_removeItemsAnimation->clear();

  foreach( IdentityItem *item, m_items ) {
    if ( item == m_previousItem ) continue;

    QPropertyAnimation *animation = new QPropertyAnimation(item, "opacity", this);
    m_removeItemsAnimation->insertAnimation( 0, animation );
    animation->setStartValue( 1 );
    animation->setEndValue( 0 );
    animation->setDuration( 200 );
  }

  m_removeItemsAnimation->start();  
}

void GroupGraphicsView::placeItems()
{
  m_compactLayout = false;

  bool doAnimation = false;
  QPoint previousItemPos;

  if ( m_previousItem ) {
    doAnimation = true;

    if ( !m_placeItemsAnimation ) {
      m_placeItemsAnimation = new QParallelAnimationGroup( this );
      connect( m_placeItemsAnimation, SIGNAL( finished() ),
        SLOT( finishPlaceItems() ) );
    }
    m_placeItemsAnimation->clear();
    m_placeItemsAnimations.clear();

    previousItemPos = m_view->mapFromScene( m_previousItem->pos() );
  }

  m_scene->clear();
  m_items.clear();
  m_labelItems.clear();
  m_globalMenu = 0;

  IdentityItemGroup items = prepareIdentityItems( doAnimation );

  m_items = items.items;
  foreach( IdentityItem *item, m_items ) {
    m_scene->addItem( item );
  }

  createMenuItems();
  positionMenuItems();

  m_view->centerOn( items.center );

  if ( doAnimation ) {
    foreach( QPropertyAnimation *animation, m_placeItemsAnimations ) {
      animation->setStartValue( m_view->mapToScene( previousItemPos ) );
    }
  
    m_placeItemsAnimation->start();
  } else {
    createLabelItems();
  }

  foreach( IdentityItem *item, m_items ) {
    item->hidePopups();
  }
}

void GroupGraphicsView::finishPlaceItems()
{
  createLabelItems();
}

void GroupGraphicsView::unplaceItems()
{
  foreach( LabelItem *item, m_labelItems ) {
    delete item;
  }
  m_labelItems.clear();

  m_compactLayout = false;

  if ( !m_unplaceItemsAnimation ) {
    m_unplaceItemsAnimation = new QParallelAnimationGroup( this );
    connect( m_unplaceItemsAnimation, SIGNAL( finished() ),
      SLOT( unhideItems() ) );
  }
  m_unplaceItemsAnimation->clear();

  m_newItems = prepareIdentityItems( false );

  if ( !m_newItems.previousGroup ) {
    recreateItems();
    return;
  }

  QPointF target = m_newItems.previousGroup->pos();

  QRect viewportRect = m_view->viewport()->rect();
  QPoint currentViewportCenter( viewportRect.width() / 2,
    viewportRect.height() / 2 );
  QPointF currentCenter = m_view->mapToScene( currentViewportCenter );
  
  target.setX( target.x() - m_newItems.center.x() + currentCenter.x() );
  target.setY( target.y() - m_newItems.center.y() + currentCenter.y() );
  
  foreach( IdentityItem *item, m_items ) {
    QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);
    m_unplaceItemsAnimation->insertAnimation( 0, animation );

    animation->setDuration( 300 );
    animation->setEndValue( target );
    animation->setEasingCurve( QEasingCurve::OutCubic );    
  }

  m_unplaceItemsAnimation->start();
}

void GroupGraphicsView::unhideItems()
{
  m_scene->clear();
  m_items.clear();
  m_labelItems.clear();
  m_globalMenu = 0;

  m_items = m_newItems.items;
  foreach( IdentityItem *item, m_items ) {
    item->setOpacity( 0 );
    m_scene->addItem( item );
  }

  createLabelItems();

  createMenuItems();
  positionMenuItems();

  m_view->centerOn( m_newItems.center );

  if ( !m_unhideItemsAnimation ) {
    m_unhideItemsAnimation = new QParallelAnimationGroup( this );
  }
  m_unhideItemsAnimation->clear();

  foreach( IdentityItem *item, m_items ) {
    if ( item == m_newItems.previousGroup ) {
      item->setOpacity( 1 );
    } else {
      QPropertyAnimation *animation = new QPropertyAnimation(item, "opacity", this);
      m_unhideItemsAnimation->insertAnimation( 0, animation );
      animation->setStartValue( 0 );
      animation->setEndValue( 1 );
      animation->setDuration( 200 );
    }
  }

  m_newItems = IdentityItemGroup();

  m_unhideItemsAnimation->start();
}

IdentityItemGroup GroupGraphicsView::prepareIdentityItems( bool doAnimation )
{
  IdentityItemGroup result;
  
  Polka::Identity::List identities = model()->identitiesOfGroup( group() );

  int columns = sqrt( identities.size() );
  int spacing = 150;

  int x = 0;
  int y = 0;

  qreal minX = 0;
  qreal minY = 0;
  qreal maxX = 0;
  qreal maxY = 0;

  bool firstItem = true;

  Polka::GroupView view = model()->groupView( group() );

  foreach( Polka::Identity identity, identities ) {
    qreal posX = x * spacing + ( y % 2 ) * spacing / 2;
    qreal posY = y * spacing * 0.866; // sin(60 degree)

    IdentityItem *item = new IdentityItem( model(), identity );
    result.items.append( item );

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

    if ( doAnimation ) {
      QPropertyAnimation *animation = new QPropertyAnimation(item, "pos", this);
      m_placeItemsAnimation->insertAnimation( 0, animation );
      m_placeItemsAnimations.append( animation );

      animation->setDuration( 300 );
      QPointF target( itemX, itemY );
      animation->setEndValue( target );
      animation->setEasingCurve( QEasingCurve::OutCubic );
    } else {
      item->setPos( itemX, itemY );
    }

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

    x++;
    
    if ( x >= ( columns + ( y + 1 ) % 2 ) ) {
      x = 0;
      y++;
    }

    if ( previousGroup().isValid() &&
         item->identity().id() == previousGroup().id() ) {
      result.previousGroup = item;
    }
  }
  
  qreal centerX = ( minX + maxX ) / 2;
  qreal centerY = ( minY + maxY ) / 2;

  result.center = QPointF( centerX, centerY );

  return result;
}

void GroupGraphicsView::createLabelItems()
{
  Polka::GroupView view = model()->groupView( group() );

  foreach( Polka::ViewLabel label, view.viewLabelList() ) {
    createLabelItem( label );
  }
}

void GroupGraphicsView::createMenuItems()
{
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
}

void GroupGraphicsView::positionMenuItems()
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

void GroupGraphicsView::positionAbsoluteItems()
{
  positionMenuItems();
}

void GroupGraphicsView::slotRemoveIdentity( const Polka::Identity &identity )
{
  emit removeIdentity( identity, group() );
}

void GroupGraphicsView::savePosition( const Polka::Identity &identity,
  const QPointF &pos )
{
  if ( !m_compactLayout ) {
    model()->saveViewPosition( group(), identity, pos );
  }
}

void GroupGraphicsView::saveLabel( const Polka::ViewLabel &label,
  const QPointF &pos )
{
  Polka::ViewLabel l = label;
  l.setX( pos.x() );
  l.setY( pos.y() );
  model()->saveViewLabel( group(), l );
}


void GroupGraphicsView::saveCheck( const Polka::Identity &identity,
  bool checked )
{
  model()->saveViewCheck( group(), identity, checked );
}

void GroupGraphicsView::addLabel()
{
  addLabel( m_view->mapToScene( QPoint( 10, 10 ) ) );
}

void GroupGraphicsView::addLabel( const QPointF &pos )
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
    
    model()->saveViewLabel( group(), label );
  }
}

void GroupGraphicsView::removeLabel( LabelItem *item,
  const Polka::ViewLabel &label )
{
  m_labelItems.removeAll( item );

  delete item;
  model()->removeViewLabel( group(), label );
}

void GroupGraphicsView::renameLabel( LabelItem *item,
  Polka::ViewLabel label )
{
  bool ok;
  QString name = KInputDialog::getText( i18n("Rename Label"),
    i18n("Enter new text of label"), label.text(),
    &ok );
  if ( ok ) {
    item->setText( name );
  
    label.setText( name );
    model()->saveViewLabel( group(), label );
  }
}

LabelItem *GroupGraphicsView::createLabelItem( const Polka::ViewLabel &label )
{
  LabelItem *item = new LabelItem( model(), label );

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

void GroupGraphicsView::resetLayout()
{
  model()->clearViewPositions( group() );

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

void GroupGraphicsView::emitCloneGroup()
{
  emit cloneGroup( group() );
}

void GroupGraphicsView::emitRemoveGroup()
{
  emit removeGroup( group() );
}

void GroupGraphicsView::morphToCompact()
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
    connect( m_morphToAnimation, SIGNAL( finished() ),
      SLOT( finishMorphToCompact() ) );
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

void GroupGraphicsView::finishMorphToCompact()
{
}

void GroupGraphicsView::morphFromCompact()
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

void GroupGraphicsView::finishMorphFromCompact()
{
  m_mainMenu->show();
  m_magicMenu->show();

  foreach( LabelItem *item, m_labelItems ) {
    item->show();
  }  
}

IdentityItem *GroupGraphicsView::item( const Polka::Identity &identity ) const
{
  foreach( IdentityItem *item, m_items ) {
    if ( item->identity().id() == identity.id() ) return item;
  }
  return 0;
}

bool GroupGraphicsView::eventFilter( QObject *watched, QEvent *event )
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

void GroupGraphicsView::slotItemSelected( FanMenu::Item *item )
{
  hideGlobalMenu();

  if ( item == m_addLabelItem ) {
    addLabel( m_globalMenu->pos() );
  }
}

void GroupGraphicsView::hideGlobalMenu()
{
  if ( m_globalMenu ) {
    m_globalMenu->hide();
  }
}

void GroupGraphicsView::slotMouseMoved( const QPoint &pos )
{
  if ( !m_globalMenu || !m_globalMenu->isVisible() ) return;

  if ( !m_globalMenu->isCloseTo( m_view->mapToScene( pos ) ) ) {
    hideGlobalMenu();
  }
}
