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

#include "htmlcreator.h"

#include <QDebug>

namespace Polka {

CssDeclaration::CssDeclaration( const QString &property, const QString &value )
  : m_property( property ), m_value( value )
{
}

QString CssDeclaration::property() const
{
  return m_property;
}

QString CssDeclaration::value() const
{
  return m_value;
}


CssRule::CssRule()
{
}

CssRule::CssRule( const QString &selector )
  : m_selector( selector )
{
}

QString CssRule::selector() const
{
  return m_selector;
}

CssDeclaration::List CssRule::declarations() const
{
  return m_declarations;
}

void CssRule::addDeclaration( const CssDeclaration &declaration )
{
  m_declarations.append( declaration );
}

void CssRule::add( const QString &property, const QString &value )
{
  addDeclaration( CssDeclaration( property, value ) );
}

void CssRule::dump()
{
  qDebug() << "RULE FOR" << m_selector;
  foreach( CssDeclaration d, m_declarations ) {
    qDebug() << "  " << d.property() << d.value();
  }
}


void CssSheet::addRule( const CssRule &rule )
{
  m_rules.insert( rule.selector(), rule );
}

void CssSheet::addRule( const QString &selector, const QString &property,
  const QString &value )
{
  CssRule rule( selector );

  CssDeclaration declaration( property, value );

  if ( m_rules.contains( selector ) ) {
    rule = m_rules[ selector ];
  }

  rule.addDeclaration( declaration );

  addRule( rule );
}

CssRule &CssSheet::rule( const QString &selector )
{
  CssRule rule( selector );

  if ( !m_rules.contains( selector ) ) {
    addRule( rule );
  }

  return m_rules[ selector ];
}

QString CssSheet::render()
{
  QString text;

  foreach( CssRule rule, m_rules ) {
    text += rule.selector() + " {\n";
    foreach( CssDeclaration declaration, rule.declarations() ) {
      text += "  " + declaration.property() + ": " + declaration.value() +
        ";\n";
    }
    text += "}\n";
  }
  
  return text;
}


HtmlElement::HtmlElement()
{
}

HtmlElement &HtmlElement::element( const QString &name )
{
  HtmlElement element;
  element.m_name = name;
  
  m_childElements.append( element );

  return m_childElements.last();
}

HtmlElement &HtmlElement::attribute( const QString &key, const QString &value )
{
  m_attributes.insert( key, value );
  return *this;
}

HtmlElement &HtmlElement::attribute( const QString &key, const char *string )
{
  m_attributes.insert( key, QString( string ) );
  return *this;
}

HtmlElement &HtmlElement::attribute( const QString &key, bool value )
{
  m_attributes.insert( key, value ? "true" : "false" );
  return *this;
}

HtmlElement &HtmlElement::c( const QString &value )
{
  return attribute( "class", value );
}

void HtmlElement::text( const QString &text )
{
  m_text = text;
}

HtmlElement::List &HtmlElement::childElements()
{
  return m_childElements;
}


HtmlDoc::HtmlDoc()
{
}
    
QString HtmlDoc::html()
{
  QString html;

  QXmlStreamWriter xml( &html );
  xml.setAutoFormatting( true );
  xml.writeStartDocument();

  xml.writeStartElement( "head" );
  xml.writeStartElement( "style" );
  xml.writeAttribute( "type", "text/css" );
  
  xml.writeComment( "\n" + m_css.render() );
  xml.writeEndElement();

  xml.writeStartElement( "body" );
  writeElements( xml, childElements() );
  xml.writeEndElement();

  xml.writeEndDocument();
  
  return html;
}

void HtmlDoc::writeElements( QXmlStreamWriter &xml,
  HtmlElement::List &elements )
{
  foreach( HtmlElement element, elements ) {
    xml.writeStartElement( element.name() );
    foreach( QString key, element.attributes().keys() ) {
      xml.writeAttribute( key, element.attributes().value( key ) );
    }
    if ( !element.text().isEmpty() ) {
      xml.writeCharacters( element.text() ); 
    }
    
    if ( !element.childElements().isEmpty() ) {
      writeElements( xml, element.childElements() ); 
    }
    xml.writeEndElement();
  }
}

void HtmlDoc::setCss( const CssSheet &css )
{
  m_css = css;
}

}
