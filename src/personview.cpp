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

#include "htmlcreator.h"
#include "imageloader.h"
#include "pictureselector.h"
#include "regiongrabber.h"
#include "polkamodel.h"
#include "commenteditor.h"

#include <klocale.h>
#include <KUrl>
#include <KInputDialog>

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

  button = new QPushButton( i18n("Close") );
  topLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( close() ) );

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

  HtmlDoc doc;

  CssSheet css;

  CssRule &rule = css.rule( ".editbar" );

  rule.add( "background-color", "#80CCFF" );
  
  QString radius = "6px 6px";
  
  rule.add( "padding", radius );

  // FIXME: Make it work with more recent webkit version (seems to work in
  // Chromium, but not in QWebKit 4.5.
/*
  rule.add( "border-bottom-left-radius", radius );
  rule.add( "border-bottom-right-radius", radius );
  rule.add( "border-top-left-radius", radius );
  rule.add( "border-top-right-radius", radius );
*/

  rule.add( "-webkit-border-radius", "6px" );

  // Show edit link on hover
  css.addRule( ".edit-link", "font-size", "60%" );
  css.addRule( ".trigger span.edit-link", "display", "none" );
  css.addRule( ".trigger:hover span.edit-link", "display", "block" );

  doc.setCss( css );

  doc.element("h1").text(identity.displayName());
  if ( !identity.name().text().isEmpty() ) {
    doc.element("p").text(identity.name().text());
  }

  HtmlElement &div = doc.element("div");
//  div.attribute("contentEditable", true);
  foreach( Email email, identity.emails().emailList() ) {
    HtmlElement &a = div.element("div").element("a");
    a.attribute("href","mailto:" + email.text());
    a.text(email.text());
  }

  HtmlElement &commentsDiv = doc.element("div");
  commentsDiv.element("h2").text(i18n("Comments"));
  
  foreach( Comment comment, identity.comments().commentList() ) {
    HtmlElement &commentDiv = commentsDiv.element( "div" );
    commentDiv.attribute( "class", "trigger" );

    commentDiv.element("p").text( comment.text() );

    HtmlElement &span = commentDiv.element("span");
    span.attribute("class","edit-link");
    HtmlElement &a = span.element("a");
    a.attribute("href","polka:editComment/" + comment.id() );
    a.text("Edit");
  }

  HtmlElement &editBar = doc.element("div");
  editBar.attribute( "class", "editbar" );

  HtmlElement &addEmail = editBar.element("span").element("a");
  addEmail.attribute("href","polka:addEmail");
  addEmail.text("Add email");

  HtmlElement &addComment = editBar.element("span").element("a");
  addComment.attribute("href","polka:addComment");
  addComment.text("Add comment");

  qDebug() << doc.html();

  m_webView->setHtml( doc.html() );
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
    QString action = url.path();
    qDebug() << "ACTION" << action;
    if ( action == "addEmail" ) addEmail();
    else if ( action == "addComment" ) addComment();
    else qDebug() << "unknown action" << action;
  }
}

void PersonView::addEmail()
{
  bool ok;
  QString email = KInputDialog::getText( i18n("Add email"),
    i18n("Enter new email address"), QString(), &ok );
  if ( ok ) {
    Email e;
    e.setText( email );
    Emails es = m_identity.emails();
    es.addEmail( e );
    m_identity.setEmails( es );

    m_model->insert( m_identity );

//    showIdentity( m_identity );
  }  
}

void PersonView::addComment()
{
  CommentEditor *editor = new CommentEditor( this );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Comment c;
    c.setText( editor->comment() );
    Comments cs = m_identity.comments();
    cs.addComment( c );
    m_identity.setComments( cs );
    
    m_model->insert( m_identity );
  }
}
