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

#ifndef MAKE_IT_MEEGO
#include "imageloader.h"
#endif

#include "pictureselector.h"
#include "regiongrabber.h"
#include "polkamodel.h"
#include "commenteditor.h"
#include "polka/htmlrenderer.h"
#include "phoneeditor.h"
#include "linkeditor.h"
#include "addresseditor.h"
#include "detaileditor.h"
#include "pictureselectorcontrols.h"
#include "settings.h"

#include <KLocale>
#include <KUrl>
#include <KInputDialog>
#include <KRandom>
#include <KRun>
#include <KDirWatch>
#include <KProcess>

PersonView::PersonView( PolkaModel *model, QWidget *parent )
  : QWidget( parent ), m_model( model ), m_dirWatch( 0 )
{
  QBoxLayout *topLayout = new QVBoxLayout( this );

  m_webView = new QWebView;
  topLayout->addWidget( m_webView );
  m_webView->page()->setLinkDelegationPolicy( QWebPage::DelegateAllLinks );
  connect( m_webView->page(), SIGNAL( linkClicked( const QUrl & ) ),
    SLOT( slotLinkClicked( const QUrl & ) ) );

  m_pictureSelectorControls = new PictureSelectorControls( m_model );
  topLayout->addWidget( m_pictureSelectorControls );
  m_pictureSelectorControls->hide();

  QHBoxLayout *pictureSelectorLayout = new QHBoxLayout;
  topLayout->addLayout( pictureSelectorLayout );

  m_pictureSelector = new PictureSelector( m_model );
  pictureSelectorLayout->addWidget( m_pictureSelector );
  connect( m_pictureSelector, SIGNAL( grabPicture() ), SLOT( grabPicture() ) );

  connect( m_pictureSelector,
    SIGNAL( pictureSelected( const Polka::Picture & ) ),
    m_pictureSelectorControls, SLOT( setPicture( const Polka::Picture & ) ) );

  pictureSelectorLayout->addStretch( 1 );

  connect( m_model, SIGNAL( identityChanged( const Polka::Identity & ) ),
    SLOT( showIdentity( const Polka::Identity & ) ) );
}

void PersonView::showIdentity( const Polka::Identity &identity )
{
  m_identity = identity;

  Polka::Pictures pictures = identity.pictures();

  m_pictureSelectorControls->hide();
  m_pictureSelectorControls->setIdentity( identity );
  m_pictureSelector->setPictures( pictures );

#ifndef MAKE_IT_MEEGO
  if ( !pictures.pictureList().isEmpty() ) {
    KUrl u( pictures.pictureList().first().url() );
    connect( ImageLoader::load(u), SIGNAL( loaded(const QPixmap &) ),
      SLOT( setImage( const QPixmap & ) ) );
  }
#endif

  Polka::HtmlRenderer renderer;

  QString html = renderer.personEditor( identity,
    m_model->pixmapPath( identity ), Settings::enableMagic() );

//  qDebug() << html;

  m_webView->setHtml( html );
}

void PersonView::showIdentity()
{
  showIdentity( m_identity );
}

Polka::Identity PersonView::identity() const
{
  return m_identity;
}

void PersonView::setImage( const QPixmap &pixmap )
{
  Q_UNUSED( pixmap )
  
  // FIXME: Update HTML view
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

  if ( !pixmap.isNull() ) {
    setImage( pixmap );

    m_model->importPicture( pixmap, m_identity );
  }
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
    else if ( action == "commentEmail" ) commentEmail( path.value( 1 ) );

    else if ( action == "addDetail" ) addDetail();
    else if ( action == "editDetail" ) editDetail( path.value( 1 ) );
    else if ( action == "removeDetail" ) removeDetail( path.value( 1 ) );
    else if ( action == "commentDetail" ) commentDetail( path.value( 1 ) );

    else if ( action == "addPhone" ) addPhone();
    else if ( action == "editPhone" ) editPhone( path.value( 1 ) );
    else if ( action == "removePhone" ) removePhone( path.value( 1 ) );
    else if ( action == "commentPhone" ) commentPhone( path.value( 1 ) );

    else if ( action == "addLink" ) addLink();
    else if ( action == "editLink" ) editLink( path.value( 1 ) );
    else if ( action == "removeLink" ) removeLink( path.value( 1 ) );
    else if ( action == "commentLink" ) commentLink( path.value( 1 ) );

    else if ( action == "addAddress" ) addAddress();
    else if ( action == "editAddress" ) editAddress( path.value( 1 ) );
    else if ( action == "removeAddress" ) removeAddress( path.value( 1 ) );
    else if ( action == "commentAddress" ) commentAddress( path.value( 1 ) );

    else if ( action == "addComment" ) addComment();
    else if ( action == "editComment" ) editComment( path.value( 1 ) );
    else if ( action == "removeComment" ) removeComment( path.value( 1 ) );

    else if ( action == "close" ) requestClose();
    else if ( action == "magic" ) debugHtml();

    else qDebug() << "unknown action" << action;
  } else {
    new KRun( KUrl( url ), this );
  }
}

void PersonView::requestClose()
{
  emit closeRequested();
}

void PersonView::addEmail()
{
  bool ok;
  QString email = KInputDialog::getText( i18n("Add email"),
    i18n("Enter new email address"), QString(), &ok );
  if ( ok ) {
    Polka::Email e;
    e.setId( KRandom::randomString( 10 ) );
    e.setEmailAddress( email );
    Polka::Emails es = m_identity.emails();
    es.addEmail( e );
    m_identity.setEmails( es );

    m_model->insert( m_identity, i18n("Add email address %1 to %2")
      .arg( email ).arg( m_identity.name().value() ) );

//    showIdentity( m_identity );
  }
}

void PersonView::editEmail( const QString &id )
{
  Polka::Email e = m_identity.emails().findEmail( id );

  bool ok;
  QString email = KInputDialog::getText( i18n("Add email"),
    i18n("Enter new email address"), e.emailAddress(), &ok );
  if ( ok ) {
    Polka::Emails es = m_identity.emails();
    e.setEmailAddress( email );
    es.insert( e );
    m_identity.setEmails( es );
    
    m_model->insert( m_identity, i18n("Edit email address %1 of %2")
      .arg( email ).arg( m_identity.name().value() ) );
  }
}

void PersonView::commentEmail( const QString &id )
{
  Polka::Email e = m_identity.emails().findEmail( id );

  Polka::Comment comment = e.comment();

  CommentEditor *editor = new CommentEditor( this );
  editor->setComment( comment.value() );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Polka::Emails es = m_identity.emails();
    comment.setValue( editor->comment() );
    e.setComment( comment );
    es.insert( e );
    m_identity.setEmails( es );
        
    m_model->insert( m_identity, i18n("Edit comment of email %2")
      .arg( e.emailAddress() ) );
  }
}

void PersonView::removeEmail( const QString &id )
{
  Polka::Emails es = m_identity.emails();
  Polka::Email e = es.findEmail( id );
  es.remove( e );
  m_identity.setEmails( es );
  
  m_model->insert( m_identity, i18n("Remove email address %1 from %2")
    .arg( e.emailAddress() ).arg( m_identity.name().value() ) );
}

void PersonView::addDetail()
{
  DetailEditor *editor = new DetailEditor( this );
  if ( editor->exec() == DetailEditor::Accepted ) {
    Polka::Detail d;
    d.setId( KRandom::randomString( 10 ) );
    d.setDetailName( editor->detailName() );
    d.setDetailValue( editor->detailValue() );
    Polka::Details ds = m_identity.details();
    ds.addDetail( d );
    m_identity.setDetails( ds );

    m_model->insert( m_identity, i18n("Add detail %1 to %2")
      .arg( d.detailName() ).arg( m_identity.name().value() ) );
  }
}

void PersonView::editDetail( const QString &id )
{
  Polka::Detail d = m_identity.details().findDetail( id );
  DetailEditor *editor = new DetailEditor( this );
  editor->setDetailName( d.detailName() );
  editor->setDetailValue( d.detailValue() );
  if ( editor->exec() == AddressEditor::Accepted ) {
    Polka::Details ds = m_identity.details();
    d.setDetailName( editor->detailName() );
    d.setDetailValue( editor->detailValue() );
    ds.insert( d );
    m_identity.setDetails( ds );
    
    m_model->insert( m_identity, i18n("Edit detail %1 of %2")
      .arg( d.detailName() ).arg( m_identity.name().value() ) );
  }
}

void PersonView::commentDetail( const QString &id )
{
  Polka::Detail d = m_identity.details().findDetail( id );

  Polka::Comment comment = d.comment();

  CommentEditor *editor = new CommentEditor( this );
  editor->setComment( comment.value() );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Polka::Details ds = m_identity.details();
    comment.setValue( editor->comment() );
    d.setComment( comment );
    ds.insert( d );
    m_identity.setDetails( ds );

    m_model->insert( m_identity, i18n("Edit comment of detail %1")
      .arg( d.detailName() ) );
  }
}

void PersonView::removeDetail( const QString &id )
{
  Polka::Details ds = m_identity.details();
  Polka::Detail d = ds.findDetail( id );
  ds.remove( d );
  m_identity.setDetails( ds );
  
  m_model->insert( m_identity, i18n("Remove detail %1 from %2")
     .arg( d.detailName() ).arg( m_identity.name().value() ) );
}

void PersonView::addAddress()
{
  AddressEditor *editor = new AddressEditor( this );
  if ( editor->exec() == AddressEditor::Accepted ) {
    Polka::Addresses cs = m_identity.addresses();
    Polka::Address c;
    c.setId( KRandom::randomString( 10 ) );
    c.setLabel( editor->address() );
    cs.insert( c );
    m_identity.setAddresses( cs );
    
    m_model->insert( m_identity, i18n("Add address to %2")
      .arg( m_identity.name().value() ) );
  }
}

void PersonView::editAddress( const QString &id )
{
  Polka::Address address = m_identity.addresses().findAddress( id );

  AddressEditor *editor = new AddressEditor( this );
  editor->setAddress( address.label() );
  if ( editor->exec() == AddressEditor::Accepted ) {
    Polka::Addresses cs = m_identity.addresses();
    address.setLabel( editor->address() );
    cs.insert( address );
    m_identity.setAddresses( cs );
    
    m_model->insert( m_identity, i18n("Edit address of %2")
      .arg( m_identity.name().value() ) );
  }
}

void PersonView::removeAddress( const QString &id )
{
  Polka::Addresses cs = m_identity.addresses();
  Polka::Address c = cs.findAddress( id );
  cs.remove( c );
  m_identity.setAddresses( cs );
  
  m_model->insert( m_identity, i18n("Remove address from %2")
    .arg( m_identity.name().value() ) );
}

void PersonView::commentAddress( const QString &id )
{
  Polka::Address a = m_identity.addresses().findAddress( id );

  Polka::Comment comment = a.comment();

  CommentEditor *editor = new CommentEditor( this );
  editor->setComment( comment.value() );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Polka::Addresses as = m_identity.addresses();
    comment.setValue( editor->comment() );
    a.setComment( comment );
    as.insert( a );
    m_identity.setAddresses( as );
        
    m_model->insert( m_identity, i18n("Edit comment of address of %2")
      .arg( m_identity.name().value() ) );
  }
}


void PersonView::addComment()
{
  CommentEditor *editor = new CommentEditor( this );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Polka::Comments cs = m_identity.comments();
    Polka::Comment c;
    c.setId( KRandom::randomString( 10 ) );
    c.setValue( editor->comment() );
    cs.insert( c );
    m_identity.setComments( cs );
    
    m_model->insert( m_identity, i18n("Add comment to %2")
      .arg( m_identity.name().value() ) );
  }
}

void PersonView::editComment( const QString &id )
{
  Polka::Comment comment = m_identity.comments().findComment( id );

  CommentEditor *editor = new CommentEditor( this );
  editor->setComment( comment.value() );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Polka::Comments cs = m_identity.comments();
    comment.setValue( editor->comment() );
    cs.insert( comment );
    m_identity.setComments( cs );
    
    m_model->insert( m_identity, i18n("Edit comment of %2")
      .arg( m_identity.name().value() ) );
  }
}

void PersonView::removeComment( const QString &id )
{
  Polka::Comments cs = m_identity.comments();
  Polka::Comment c = cs.findComment( id );
  cs.remove( c );
  m_identity.setComments( cs );
  
  m_model->insert( m_identity, i18n("Remove comment from %2")
    .arg( m_identity.name().value() ) );
}

void PersonView::addPhone()
{
  PhoneEditor *editor = new PhoneEditor( this );
  if ( editor->exec() == PhoneEditor::Accepted ) {
    Polka::Phones cs = m_identity.phones();
    Polka::Phone c = editor->phone();
    c.setId( KRandom::randomString( 10 ) );
    cs.insert( c );
    m_identity.setPhones( cs );
    
    m_model->insert( m_identity, i18n("Add phone %1 to %2")
      .arg( c.phoneNumber() ).arg( m_identity.name().value() ) );
  }
}

void PersonView::editPhone( const QString &id )
{
  Polka::Phone phone = m_identity.phones().findPhone( id );

  PhoneEditor *editor = new PhoneEditor( this );
  editor->setPhone( phone );
  if ( editor->exec() == PhoneEditor::Accepted ) {
    Polka::Phones cs = m_identity.phones();
    phone = editor->phone();
    cs.insert( phone );
    m_identity.setPhones( cs );
    
    m_model->insert( m_identity, i18n("Edit phone %1 of %2")
      .arg( phone.phoneNumber() ).arg( m_identity.name().value() ) );
  }
}

void PersonView::removePhone( const QString &id )
{
  Polka::Phones cs = m_identity.phones();
  Polka::Phone c = cs.findPhone( id );
  cs.remove( c );
  m_identity.setPhones( cs );
  
  m_model->insert( m_identity, i18n("Remove phone %1 from %2")
    .arg( c.phoneNumber() ).arg( m_identity.name().value() ) );
}

void PersonView::commentPhone( const QString &id )
{
  Polka::Phone p = m_identity.phones().findPhone( id );

  Polka::Comment comment = p.comment();

  CommentEditor *editor = new CommentEditor( this );
  editor->setComment( comment.value() );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Polka::Phones ps = m_identity.phones();
    comment.setValue( editor->comment() );
    p.setComment( comment );
    ps.insert( p );
    m_identity.setPhones( ps );
        
    m_model->insert( m_identity, i18n("Edit comment of phone %2")
      .arg( p.phoneNumber() ) );
  }
}


void PersonView::addLink()
{
  LinkEditor *editor = new LinkEditor( this );
  if ( editor->exec() == LinkEditor::Accepted ) {
    Polka::Links cs = m_identity.links();
    Polka::Link c = editor->link();
    c.setId( KRandom::randomString( 10 ) );
    cs.insert( c );
    m_identity.setLinks( cs );
    
    m_model->insert( m_identity, i18n("Add link %1 to %2")
      .arg( c.url() ).arg( m_identity.name().value() ) );
  }
}

void PersonView::editLink( const QString &id )
{
  Polka::Link link = m_identity.links().findLink( id );

  LinkEditor *editor = new LinkEditor( this );
  editor->setLink( link );
  if ( editor->exec() == LinkEditor::Accepted ) {
    Polka::Links cs = m_identity.links();
    link = editor->link();
    cs.insert( link );
    m_identity.setLinks( cs );
    
    m_model->insert( m_identity, i18n("Edit link %1 of %2")
      .arg( link.url() ).arg( m_identity.name().value() ) );
  }
}

void PersonView::removeLink( const QString &id )
{
  Polka::Links cs = m_identity.links();
  Polka::Link c = cs.findLink( id );
  cs.remove( c );
  m_identity.setLinks( cs );
  
  m_model->insert( m_identity, i18n("Remove link %1 from %2")
    .arg( c.url() ).arg( m_identity.name().value() ) );
}

void PersonView::commentLink( const QString &id )
{
  Polka::Link l = m_identity.links().findLink( id );

  Polka::Comment comment = l.comment();

  CommentEditor *editor = new CommentEditor( this );
  editor->setComment( comment.value() );
  if ( editor->exec() == CommentEditor::Accepted ) {
    Polka::Links ls = m_identity.links();
    comment.setValue( editor->comment() );
    l.setComment( comment );
    ls.insert( l );
    m_identity.setLinks( ls );
        
    m_model->insert( m_identity, i18n("Edit comment of link %2")
      .arg( l.url() ) );
  }
}


void PersonView::editName()
{
  Polka::Name name = m_identity.name();
  QString oldNameString = name.value();

  bool ok;
  QString nameString = KInputDialog::getText( i18n("Edit name"),
    QString(), oldNameString, &ok );
  if ( ok ) {
    name.setValue( nameString );
    m_identity.setName( name );
    
    m_model->insert( m_identity, i18n("Changed name from %1 to %2")
      .arg( oldNameString ).arg( nameString ) );
  }  
}

void PersonView::debugHtml()
{
  Polka::HtmlRenderer renderer;

  QString html = renderer.personEditor( m_identity,
    m_model->pixmapPath( m_identity ) );
  
  QFile file("polka.html");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug() << "Error opening html debug file";
    return;
  }
  
  QTextStream out(&file);
  out << html;
  
  file.close();

  if ( !m_dirWatch ) {
#ifndef MAKE_IT_MEEGO
    m_dirWatch = new KDirWatch( this );
    m_dirWatch->addFile( file.fileName() );
    connect( m_dirWatch, SIGNAL( dirty( const QString & ) ),
      SLOT( reloadDebugHtml() ) );  
#endif
}
  
  m_webView->setHtml( html );
  
  QStringList args;
  args << file.fileName();
  
  KProcess::execute( "nc", args );
}

void PersonView::reloadDebugHtml()
{
  qDebug() << "RELOAD DEBUG HTML";

  QFile file("polka.html");
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "Error opening html debug file";
    return;
  }
  
  QTextStream in(&file);
  QString html = in.readAll();
  
  file.close();

  m_webView->setHtml( html );
}
