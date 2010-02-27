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

HtmlRenderer::HtmlRenderer()
{
}

QString HtmlRenderer::renderPerson( const Identity &identity )
{
  HtmlDoc doc;

  CssSheet css;

  CssRule &rule = css.rule( ".editbar" );

  rule.add( "background-color", "#80CCFF" );
  rule.add( "border", "1px solid" );
  
  rule.add( "position", "absolute" );
  rule.add( "bottom", "10px" );
  
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

  css.addRule( ".editbar span", "margin-right", "10px" );

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
  foreach( Email email, identity.emails().emailList() ) {
    HtmlElement &emailDiv = div.element("div").c("trigger");

    HtmlElement &a = emailDiv.element("a");
    a.attribute("href","mailto:" + email.text());
    a.text(email.text());

    HtmlElement &span1 = emailDiv.element("span").c("edit-link");

    HtmlElement &e = span1.element("a");
    e.attribute("href","polka:editEmail/" + email.id());
    e.text("Edit");

    HtmlElement &span = emailDiv.element("span").c("edit-link");

    HtmlElement &r = span.element("a");
    r.attribute("href","polka:removeEmail/" + email.id());
    r.text("Remove");
  }

  HtmlElement &phonesDiv = doc.element("div");
  foreach( Phone phone, identity.phones().phoneList() ) {
    HtmlElement &phoneDiv = phonesDiv.element("div").c("trigger");

    HtmlElement &p = phoneDiv.element("p");
    p.text(phone.phoneNumber());

    HtmlElement &span1 = phoneDiv.element("span").c("edit-link");

    HtmlElement &e = span1.element("a");
    e.attribute("href","polka:editPhone/" + phone.id());
    e.text("Edit");

    HtmlElement &span2 = phoneDiv.element("span").c("edit-link");

    HtmlElement &r = span2.element("a");
    r.attribute("href","polka:removePhone/" + phone.id());
    r.text("Remove");
  }

  HtmlElement &linksDiv = doc.element("div");
  foreach( Link link, identity.links().linkList() ) {
    HtmlElement &linkDiv = linksDiv.element("div").c("trigger");

    HtmlElement &a = linkDiv.element("a");
    a.attribute("href", link.url());
    a.text(link.url());

    HtmlElement &span1 = linkDiv.element("span").c("edit-link");

    HtmlElement &e = span1.element("a");
    e.attribute("href","polka:editLink/" + link.id());
    e.text("Edit");

    HtmlElement &span = linkDiv.element("span").c("edit-link");

    HtmlElement &r = span.element("a");
    r.attribute("href","polka:removeLink/" + link.id());
    r.text("Remove");
  }

  if ( !identity.comments().commentList().isEmpty() ) {
    HtmlElement &commentsDiv = doc.element("div");
    commentsDiv.element("h2").text(i18n("Comments"));

    foreach( Comment comment, identity.comments().commentList() ) {
      HtmlElement &commentDiv = commentsDiv.element( "div" ).c("trigger");

      commentDiv.element("p").text( comment.text() );

      HtmlElement &span = commentDiv.element("span").c("edit-link");

      HtmlElement &a = span.element("a");
      a.attribute("href","polka:editComment/" + comment.id() );
      a.text("Edit");

      HtmlElement &r = span.element("a");
      r.attribute("href","polka:removeComment/" + comment.id() );
      r.text("Remove");
    }
  }

  HtmlElement &editBar = doc.element("div").c("editbar");

  HtmlElement &addEmail = editBar.element("span").element("a");
  addEmail.attribute("href","polka:addEmail");
  addEmail.text("Add email");

  HtmlElement &addPhone = editBar.element("span").element("a");
  addPhone.attribute("href","polka:addPhone");
  addPhone.text("Add phone");

  HtmlElement &addLink = editBar.element("span").element("a");
  addLink.attribute("href","polka:addLink");
  addLink.text("Add link");

  HtmlElement &addComment = editBar.element("span").element("a");
  addComment.attribute("href","polka:addComment");
  addComment.text("Add comment");

  return doc.html();
}
