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

void HtmlElement::attribute( const QString &key, const QString &value )
{
  m_attributes.insert( key, value );
}

void HtmlElement::attribute( const QString &key, bool value )
{
  m_attributes.insert( key, value ? "true" : "false" );
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

  writeElements( xml, childElements() );

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
