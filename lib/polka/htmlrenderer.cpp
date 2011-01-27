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

#include "htmlrenderer.h"

#include "htmlcreator.h"

#include <KLocale>
#include <KStandardDirs>

#include <QDebug>

namespace Polka {

HtmlRenderer::HtmlRenderer()
{
}

QString HtmlRenderer::personEditor( const Identity &identity,
  const QString &picturePath )
{
  qDebug() << "PIC PATH" << picturePath;

  HtmlDoc doc;

  CssSheet css;

  CssRule &rule = css.rule( ".editbar" );

  rule.add( "background-color", "#80CCFF" );
  rule.add( "border", "1px solid" );
  
//  rule.add( "position", "absolute" );
  rule.add( "bottom", "10px" );
  
  QString radius = "6px 6px";
  
  rule.add( "padding", radius );
  rule.add( "margin-top", "20px" );

  // FIXME: Make it work with more recent webkit version (seems to work in
  // Chromium, but not in QWebKit 4.5.
/*
  rule.add( "border-bottom-left-radius", radius );
  rule.add( "border-bottom-right-radius", radius );
  rule.add( "border-top-left-radius", radius );
  rule.add( "border-top-right-radius", radius );
*/

  rule.add( "-webkit-border-radius", "6px" );

  css.addRule( ".editbar span", "margin-right", "10px" );

  css.addRule( ".address", "float", "left" );
  css.addRule( ".address", "padding-bottom", "10px" );
  css.addRule( ".address pre", "display", "inline" );

  // Show edit link on hover
  css.addRule( ".edit-link", "font-size", "60%" );
  css.addRule( ".edit-link", "padding-left", "4px" );
  css.addRule( ".edit-link.first", "padding-left", "12px" );
  css.addRule( ".trigger span.edit-link", "visibility", "hidden" );
  css.addRule( ".trigger:hover span.edit-link", "visibility", "visible" );

  css.addRule( ".comment-icon", "margin-left", "6px" );

  CssRule &buttonRule = css.rule( ".global-buttons" );
  buttonRule.add( "position", "absolute" );
  buttonRule.add( "top", "10px" );
  buttonRule.add( "right", "10px" );
  
  css.addRule( ".global-buttons a", "padding-left", "10px" );

  css.addRule( "h1 img", "padding-right", "10px" );

  doc.setCss( css );


  HtmlElement &buttons = doc.element("div");
  buttons.c("global-buttons");
  
  HtmlElement &magicButton = buttons.element("a");
  magicButton.attribute("href","polka:magic");
  magicButton.text("Magic");
  
  HtmlElement &closeButton = buttons.element("a");
  closeButton.attribute("href","polka:close");
  closeButton.text("Close");


  HtmlElement &titleDiv = doc.element("div");
  titleDiv.c("trigger");

  HtmlElement &h1 = titleDiv.element("h1");
  if ( !picturePath.isEmpty() ) {
    HtmlElement &picImg = h1.element("img");
    picImg.attribute("src","file:///" + picturePath);
  }
  h1.text( identity.name().value() );

  HtmlElement &titleEditSpan = h1.element("span").c("edit-link first");

  HtmlElement &titleEdit = titleEditSpan.element("a");
  titleEdit.attribute("href","polka:editName");
  titleEdit.text(i18n("Edit"));

  HtmlElement &m = h1.element("span").c("edit-link");
  m.element("em").text( timeAgo( identity.name().updatedAt() ) );

  HtmlElement &div = doc.element("div");
  foreach( Email email, identity.emails().emailList() ) {
    HtmlElement &emailDiv = div.element("div").c("trigger");

    HtmlElement &a = emailDiv.element("a");
    a.attribute("href","mailto:" + email.emailAddress());
    a.text(email.emailAddress());

    addEditControls( emailDiv, "Email", email.id(), email.updatedAt(),
      email.comment() );
  }

  HtmlElement &phonesDiv = doc.element("div");
  foreach( Phone phone, identity.phones().phoneList() ) {
    HtmlElement &phoneDiv = phonesDiv.element("div").c("trigger");

    HtmlElement &p = phoneDiv.element("p");
    p.text(phone.phoneNumber());

    addEditControls( p, "Phone", phone.id(), phone.updatedAt(),
      phone.comment() );
  }

  HtmlElement &addressesDiv = doc.element("div");
  foreach( Address address, identity.addresses().addressList() ) {
    HtmlElement &addressDiv = addressesDiv.element("div").c("trigger");

    HtmlElement &d = addressDiv.element("div");
    d.c("address");

    HtmlElement &p = d.element("pre");
    p.text(address.label());

    addEditControls( addressDiv, "Address", address.id(), address.updatedAt(),
      address.comment() );

    addressDiv.element("br").attribute("clear","all");
  }

  HtmlElement &linksDiv = doc.element("div");
  foreach( Link link, identity.links().linkList() ) {
    HtmlElement &linkDiv = linksDiv.element("div").c("trigger");

    HtmlElement &a = linkDiv.element("a");
    a.attribute("href", link.url());
    a.text(link.url());

    addEditControls( linkDiv, "Link", link.id(), link.updatedAt(),
      link.comment() );
  }

  if ( !identity.comments().commentList().isEmpty() ) {
    HtmlElement &commentsDiv = doc.element("div");
    commentsDiv.element("h2").text(i18n("Comments"));

    foreach( Comment comment, identity.comments().commentList() ) {
      HtmlElement &commentDiv = commentsDiv.element( "div" ).c("trigger");

      HtmlElement &p = commentDiv.element("p");
      p.text( comment.value() );

      addEditControls( p, "Comment", comment.id(), comment.updatedAt(),
        Comment(), false );
    }
  }

  HtmlElement &editBar = doc.element("div").c("editbar");

  HtmlElement &addEmail = editBar.element("span").element("a");
  addEmail.attribute("href","polka:addEmail");
  addEmail.text("Add email");

  HtmlElement &addPhone = editBar.element("span").element("a");
  addPhone.attribute("href","polka:addPhone");
  addPhone.text("Add phone");

  HtmlElement &addAddress = editBar.element("span").element("a");
  addAddress.attribute("href","polka:addAddress");
  addAddress.text("Add address");

  HtmlElement &addLink = editBar.element("span").element("a");
  addLink.attribute("href","polka:addLink");
  addLink.text("Add link");

  HtmlElement &addComment = editBar.element("span").element("a");
  addComment.attribute("href","polka:addComment");
  addComment.text("Add comment");

  return doc.html();
}

QString HtmlRenderer::personView( const Identity &identity )
{
  HtmlDoc doc;

  HtmlElement &titleDiv = doc.element("div");

  HtmlElement &h1 = titleDiv.element("h1");
  h1.text( identity.name().value() );

  HtmlElement &div = doc.element("div");
  foreach( Email email, identity.emails().emailList() ) {
    HtmlElement &emailDiv = div.element("div").c("trigger");

    HtmlElement &a = emailDiv.element("a");
    a.attribute("href","mailto:" + email.emailAddress());
    a.text(email.emailAddress());
  }

  HtmlElement &phonesDiv = doc.element("div");
  foreach( Phone phone, identity.phones().phoneList() ) {
    HtmlElement &phoneDiv = phonesDiv.element("div").c("trigger");

    HtmlElement &p = phoneDiv.element("p");
    p.text(phone.phoneNumber());
  }

  HtmlElement &addressesDiv = doc.element("div");
  foreach( Address address, identity.addresses().addressList() ) {
    HtmlElement &addressDiv = addressesDiv.element("div").c("trigger");

    HtmlElement &d = addressDiv.element("div");
    d.c("address");

    HtmlElement &p = d.element("pre");
    p.text(address.label());

    addressDiv.element("br").attribute("clear","all");
  }

  HtmlElement &linksDiv = doc.element("div");
  foreach( Link link, identity.links().linkList() ) {
    HtmlElement &linkDiv = linksDiv.element("div").c("trigger");

    HtmlElement &a = linkDiv.element("a");
    a.attribute("href", link.url());
    a.text(link.url());
  }

  if ( !identity.comments().commentList().isEmpty() ) {
    HtmlElement &commentsDiv = doc.element("div");
    commentsDiv.element("h2").text(i18n("Comments"));

    foreach( Comment comment, identity.comments().commentList() ) {
      HtmlElement &commentDiv = commentsDiv.element( "div" ).c("trigger");

      HtmlElement &p = commentDiv.element("p");
      p.text( comment.value() );
    }
  }

  return doc.html();
}

QString HtmlRenderer::personSummary( const Identity &identity )
{
  HtmlDoc doc;

  HtmlElement &div = doc.element("div");
  foreach( Email email, identity.emails().emailList() ) {
    HtmlElement &emailDiv = div.element("div").c("trigger");

    HtmlElement &a = emailDiv.element("a");
    a.attribute("href","mailto:" + email.emailAddress());
    a.text(email.emailAddress());
  }

  HtmlElement &addressesDiv = doc.element("div");
  foreach( Address address, identity.addresses().addressList() ) {
    HtmlElement &addressDiv = addressesDiv.element("div").c("trigger");

    HtmlElement &d = addressDiv.element("div");
    d.c("address");

    HtmlElement &p = d.element("pre");
    p.text(address.label());

    addressDiv.element("br").attribute("clear","all");
  }

  return doc.html();
}

QString HtmlRenderer::timeAgo( const QDateTime &date )
{
  if ( !date.isValid() ) return i18n("some time ago");

  int ago = date.daysTo( QDateTime::currentDateTime() );

  if ( ago < 0 ) return i18n("future");
  if ( ago == 0 ) return i18n("today");
  if ( ago < 7 ) return i18n("%1 days ago").arg( QString::number( ago ) );
  if ( ago < 60 ) return i18n("%1 weeks ago")
    .arg( QString::number( ago / 7 ) );
  if ( ago < 500 ) return i18n("%1 months ago")
    .arg( QString::number( ago / 30 ) );
  return i18n("years ago");
}

void HtmlRenderer::addEditControls( HtmlElement &div, const QString &typeName,
  const QString &id, const QDateTime &updatedAt,
  const Comment &comment, bool commentEnabled )
{
  QString commentSrc = "file://" +
    KStandardDirs::locate( "appdata", "comment.png" );

  if ( !comment.value().isEmpty() ) {
    HtmlElement &aImg = div.element("a");
    aImg.c("comment-icon");
    aImg.attribute("href",
      QString("polka:comment%1/%2").arg( typeName ).arg( id ) );
    aImg.element("img").attribute("src",commentSrc);
  }

  if ( commentEnabled && comment.value().isEmpty() ) {
    HtmlElement &span3 = div.element("span").c("edit-link");

    HtmlElement &aImg = span3.element("a");
    aImg.attribute("href",
      QString("polka:comment%1/%2").arg( typeName ).arg( id ) );
    aImg.element("img").attribute("src",commentSrc);
  }

  HtmlElement &span1 = div.element("span").c("edit-link first");

  HtmlElement &e = span1.element("a");
  e.attribute("href", QString("polka:edit%1/%2").arg( typeName ).arg( id ) );
  e.text("Edit");

  HtmlElement &span2 = div.element("span").c("edit-link");

  HtmlElement &r = span2.element("a");
  r.attribute("href", QString("polka:remove%1/%2").arg( typeName ).arg( id ) );
  r.text("Remove");

  HtmlElement &m = div.element("span").c("edit-link");
  m.element("em").text( timeAgo( updatedAt ) );
}

}
