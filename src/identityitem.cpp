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

#include "identityitem.h"

#include "polkamodel.h"
#include "fanmenu.h"
#include "roundedrectitem.h"

#include <KLocale>

IdentityItem::IdentityItem( PolkaModel *model, const Polka::Identity &identity )
  : QObject( model ), m_model( model ), m_identity( identity )
{
  init();
}

IdentityItem::IdentityItem( QGraphicsItem *item, PolkaModel *model,
  const Polka::Identity &identity )
  : QObject( model ), QGraphicsEllipseItem( item ), m_model( model ),
    m_identity( identity )
{
  init();
}

void IdentityItem::init()
{
  m_checked = false;
  m_checkItem = 0;
  m_itemSize = 110;
  m_menusEnabled = true;

  setRect( -m_itemSize/2, -m_itemSize/2, m_itemSize, m_itemSize );
  setBrush( Qt::white );

  QPen pen;
  pen.setBrush( Qt::NoBrush );
  setPen( pen );

  setAcceptHoverEvents( true );

  setFlags( ItemIsMovable );

  updateItem( m_identity );
}

void IdentityItem::enableMenus( bool enabled )
{
  m_menusEnabled = enabled;
  
  if ( !m_menusEnabled ) hidePopups();
}

void IdentityItem::updateItem( const Polka::Identity &identity )
{
  m_identity = identity;
  
  foreach( QGraphicsItem *child, childItems() ) {
    delete child;
  }
  
  if ( identity.type() == "group" ) {
    int circleSize = m_itemSize + 14;
    QGraphicsEllipseItem *groupCircle = new QGraphicsEllipseItem( this );
    groupCircle->setRect( -circleSize/2, -circleSize/2,
      circleSize, circleSize );
    QPen pen;
    pen.setBrush( Qt::white );
    pen.setWidth( 4 );
    groupCircle->setPen( pen );
  }
  
  QPixmap pixmap = m_model->pixmap( identity );

  QGraphicsItem *item = new QGraphicsPixmapItem( pixmap, this );  
  item->setPos( -pixmap.width() / 2, -pixmap.height() / 2 );

  QGraphicsTextItem *textItem = new QGraphicsTextItem( identity.name().value() );

  int textWidth = textItem->boundingRect().width();
  int textHeight = textItem->boundingRect().height();

  m_nameItem = new RoundedRectItem( this );
  m_nameItem->setRect( 0, 0, textWidth, textHeight );
  m_nameItem->setBrush( QColor( 255,255,230 ) );
  m_nameItem->setZValue( 10 );

  textItem->setParentItem( m_nameItem );

  m_nameItem->setPos( - textWidth / 2, 30 );

  m_fanMenu = new FanMenu( this );
  connect( m_fanMenu, SIGNAL( itemSelected( FanMenuItem * ) ),
    SLOT( slotItemSelected( FanMenuItem * ) ) );
  m_fanMenu->setZValue( 50 );

  m_removeMenuItem = m_fanMenu->addItem( i18n("Remove") );
  m_checkMenuItem = m_fanMenu->addItem( i18n("Check") );
  if ( identity.type() == "group" ) {
    m_showMenuItem = m_fanMenu->addItem( i18n("Go to") );
//    m_groupShowMenuItem = m_fanMenu->addItem( i18n("Show") );
  } else {
    m_showMenuItem = m_fanMenu->addItem( i18n("Show") );
  }
  m_fanMenu->setupItems();

  hidePopups();
}

Polka::Identity IdentityItem::identity() const
{
  return m_identity;
}

void IdentityItem::setDefaultPos( const QPointF &pos )
{
  m_defaultPos = pos;
}

QPointF IdentityItem::defaultPos() const
{
  return m_defaultPos;
}

void IdentityItem::rememberPos( const QPointF &pos )
{
  m_rememberedPos = pos;
}

QPointF IdentityItem::rememberedPos() const
{
  return m_rememberedPos;
}

void IdentityItem::hidePopups()
{
  if ( hasPicture() ) {
    m_nameItem->hide();
  }
  m_fanMenu->hide();
}

void IdentityItem::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  if ( m_menusEnabled ) {
    m_nameItem->show();
    m_fanMenu->show();
    emit menuShown();
  }
}

void IdentityItem::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
  Q_UNUSED( event );

  hidePopups();
}

void IdentityItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  m_movePos = pos();

  QGraphicsEllipseItem::mousePressEvent( event );
}

void IdentityItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  if ( pos() != m_movePos ) {
    emit itemMoved( this, pos() );
  }

  QGraphicsEllipseItem::mouseReleaseEvent( event );
}

void IdentityItem::slotItemSelected( FanMenuItem *item )
{
  if ( item == m_removeMenuItem ) {
    emit removeIdentity( m_identity );
  } else if ( item == m_showMenuItem ) {
    emit showIdentity( m_identity );
  } else if ( item == m_checkMenuItem ) {
    checkItem();
  }
}

void IdentityItem::checkItem()
{
  if ( !m_checkItem ) {
    QPainterPath path;
    path.lineTo( 10,20 );
    path.lineTo( 30, -20 );
    path.lineTo( 20, -20 );
    path.lineTo( 10, 10 );
    path.lineTo( 0, 0 );
    
    m_checkItem = new QGraphicsPathItem( path, this );
    m_checkItem->setBrush( Qt::darkGreen );
    m_checkItem->setPos( 40, 20 );
  }
  if ( m_checked ) {
    m_checkItem->hide();
    m_checkMenuItem->setText( i18n("Check") );
  } else {
    m_checkItem->show();
    m_checkMenuItem->setText( i18n("Uncheck") );
  }
  m_checked = !m_checked;
  emit itemChecked( m_identity, m_checked );
}

QVariant IdentityItem::itemChange( GraphicsItemChange change,
  const QVariant &value )
{
  return QGraphicsEllipseItem::itemChange( change, value );
}

bool IdentityItem::hasPicture() const
{
  return !m_identity.pictures().pictureList().isEmpty();
}

void IdentityItem::undoMove()
{
  setPos( m_movePos );
}
