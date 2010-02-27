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

#include "personview.h"

#include "imageloader.h"
#include "pictureselector.h"
#include "regiongrabber.h"
#include "polkamodel.h"
#include "commenteditor.h"
#include "htmlrenderer.h"
#include "phoneeditor.h"
#include "linkeditor.h"

#include <klocale.h>
#include <KUrl>
#include <KInputDialog>
#include <KRandom>
#include <KRun>

PersonView::PersonView( PolkaModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  m_titleLabel = new QLabel;
  topLayout->addWidget( m_titleLabel );
  
  m_webView = new QWebView;
  topLayout->addWidget( m_webView );
  m_webView->page()->setLinkDelegationPolicy( QWebPage::DelegateAllLinks );
  connect( m_webView->page(), SIGNAL( linkClicked( const QUrl & ) ),
    SLOT( slotLinkClicked( const QUrl & ) ) );

  QHBoxLayout *pictureSelectorLayout = new QHBoxLayout;
  topLayout->addLayout( pictureSelectorLayout );

  m_pictureSelector = new PictureSelector;
  pictureSelectorLayout->addWidget( m_pictureSelector );

  pictureSelectorLayout->addStretch( 1 );

  QPushButton *button = new QPushButton( i18n("Grab Picture") );
  pictureSelectorLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( grabPicture() ) );

  connect( m_model, SIGNAL( identityInserted( const Identity & ) ),
    SLOT( showIdentity( const Identity & ) ) );
}

void PersonView::showIdentity( const Identity &identity )
{
  m_identity = identity;

  Picture::List pictures = identity.pictures().pictureList();

  if ( !pictures.isEmpty() ) {
    m_pictureSelector->setPictures( pictures );

    KUrl u( pictures.first().url() );
    connect( ImageLoader::load(u), SIGNAL( loaded(const QPixmap &) ),
      SLOT( setImage( const QPixmap & ) ) );
  }

  HtmlRenderer renderer;

  QString html = renderer.renderPerson( identity );

  qDebug() << html;

  m_webView->setHtml( html );
}

void PersonView::showIdentity()
{
  showIdentity( m_identity );
}

void PersonView::setImage( const QPixmap &pixmap )
{
  m_titleLabel->setPixmap( pixmap );
}

void PersonView::grabPicture()
{
  m_regionGrabber = new RegionGrabber();
  connect( m_regionGrabber, SIGNAL( regionGrabbed( const QPixmap & ) ),
    SLOT( slotRegionGrabbed( const QPixmap & ) ) );
}

void PersonView::slotRegionGrabbed( const QPixmap &pixmap )
{
  delete m_regionGrabber;
  m_regionGrabber = 0;

  setImage( pixmap );

  m_model->importPicture( pixmap, m_identity );
}

void PersonView::slotLinkClicked( const QUrl &url )
{
  qDebug() << "CLICKED" << url;

  if ( url.scheme() == "polka" ) {
    QStringList path = url.path().split("/");
    QString action = path.first();
    qDebug() << "ACTION" << action;
    
    if ( action == "editName" ) editName();
    
    else if ( action == "addEmail" ) addEmail();
    else if ( action == "editEmail" ) editEmail( path.value( 1 ) );
    else if ( action == "removeEmail" ) removeEmail( path.value( 1 ) );

    else if ( action == "addPhone" ) addPhone();
    else if ( action == "editPhone" ) editPhone( path.value( 1 ) );
    else if ( action == "removePhone" ) removePhone( path.value( 1 ) );

    else if ( action == "addLink" ) addLink();
    else if ( action == "editLink" ) editLink( path.value( 1 ) );
    else if ( action == "removeLink" ) removeLink( path.value( 1 ) );

    else if ( action == "addComment" ) addComment();
    else if ( action == "editComment" ) editComment( path.value( 1 ) );
    else if ( action == "removeComment" ) removeComment( path.value( 1 ) );

    else qDebug() << "unknown action" << action;
  } else {
    new KRun( KUrl( url ), this );
  }
}

void PersonView::addEmail()
{
  bool ok;
  QString email = KInputDialog::getText( i18n("Add email"),
    i18n("Enter new email address"), QString(), &ok );
  if ( ok ) {
    Email e;
    e.setId( KRandom::randomString( 10 ) );
    e.setText( email );
    Emails es = m_identity.emails();
    es.addEmail( e );
    m_identity.setEmails( es );

    m_model->insert( m_identity );

//    showIdentity( m_identity );
  }
}

void PersonView::editEmail( const QString &id )
{
  Email e = m_identity.emails().findEmail( id );

  bool ok;
  QString email = KInputDialog::getText( i18n("Add email"),
    i18n("Enter new email address"), e.text(), &ok );
  if ( ok ) {
    Emails es = m_identity.emails();
    e.setText( email );
    es.insert( e );
    m_identity.setEmails( es );
    
    m_model->insert( m_identity );
  }
}

void PersonView::removeEmail( const QString &id )
{
  Emails es = m_identity.emails();
  Email e = es.findEmail( id );
  es.remove( e );
  m_identity.setEmails( es );
  
  m_model->insert( m_identity );
}

void PersonView::addComment()
{
  CommentEditor *editor = new CommentEditor( this );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Comments cs = m_identity.comments();
    Comment c;
    c.setId( KRandom::randomString( 10 ) );
    c.setText( editor->comment() );
    cs.insert( c );
    m_identity.setComments( cs );
    
    m_model->insert( m_identity );
  }
}

void PersonView::editComment( const QString &id )
{
  Comment comment = m_identity.comments().findComment( id );

  CommentEditor *editor = new CommentEditor( this );
  editor->setComment( comment.text() );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Comments cs = m_identity.comments();
    comment.setText( editor->comment() );
    cs.insert( comment );
    m_identity.setComments( cs );
    
    m_model->insert( m_identity );
  }
}

void PersonView::removeComment( const QString &id )
{
  Comments cs = m_identity.comments();
  Comment c = cs.findComment( id );
  cs.remove( c );
  m_identity.setComments( cs );
  
  m_model->insert( m_identity );
}

void PersonView::addPhone()
{
  PhoneEditor *editor = new PhoneEditor( this );
  if ( editor->exec() == PhoneEditor::Accepted ) {
    Phones cs = m_identity.phones();
    Phone c = editor->phone();
    c.setId( KRandom::randomString( 10 ) );
    cs.insert( c );
    m_identity.setPhones( cs );
    
    m_model->insert( m_identity );
  }
}

void PersonView::editPhone( const QString &id )
{
  Phone phone = m_identity.phones().findPhone( id );

  PhoneEditor *editor = new PhoneEditor( this );
  editor->setPhone( phone );
  if ( editor->exec() == PhoneEditor::Accepted ) {
    Phones cs = m_identity.phones();
    phone = editor->phone();
    cs.insert( phone );
    m_identity.setPhones( cs );
    
    m_model->insert( m_identity );
  }
}

void PersonView::removePhone( const QString &id )
{
  Phones cs = m_identity.phones();
  Phone c = cs.findPhone( id );
  cs.remove( c );
  m_identity.setPhones( cs );
  
  m_model->insert( m_identity );
}


void PersonView::addLink()
{
  LinkEditor *editor = new LinkEditor( this );
  if ( editor->exec() == LinkEditor::Accepted ) {
    Links cs = m_identity.links();
    Link c = editor->link();
    c.setId( KRandom::randomString( 10 ) );
    cs.insert( c );
    m_identity.setLinks( cs );
    
    m_model->insert( m_identity );
  }
}

void PersonView::editLink( const QString &id )
{
  Link link = m_identity.links().findLink( id );

  LinkEditor *editor = new LinkEditor( this );
  editor->setLink( link );
  if ( editor->exec() == LinkEditor::Accepted ) {
    Links cs = m_identity.links();
    link = editor->link();
    cs.insert( link );
    m_identity.setLinks( cs );
    
    m_model->insert( m_identity );
  }
}

void PersonView::removeLink( const QString &id )
{
  Links cs = m_identity.links();
  Link c = cs.findLink( id );
  cs.remove( c );
  m_identity.setLinks( cs );
  
  m_model->insert( m_identity );
}

void PersonView::editName()
{
  Name name = m_identity.name();

  bool ok;
  QString nameString = KInputDialog::getText( i18n("Edit name"),
    QString(), name.text(), &ok );
  if ( ok ) {
    name.setText( nameString );
    m_identity.setName( name );
    
    m_model->insert( m_identity );
  }  
}
