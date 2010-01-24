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
#ifndef HTMLCREATOR_H
#define HTMLCREATOR_H

#include <QList>
#include <QString>
#include <QMap>
#include <QXmlStreamWriter>

class HtmlElement
{
  public:
    typedef QList<HtmlElement> List;
  
    HtmlElement();
    
    HtmlElement &element( const QString &name );

    void attribute( const QString &key, const QString &value );
    void attribute( const QString &key, bool );

    void text( const QString &text );

    HtmlElement::List &childElements();
    QString name() { return m_name; }
    QString text() { return m_text; }
    QMap<QString,QString> attributes() { return m_attributes; }

  private:
    HtmlElement::List m_childElements;

    QString m_name;
    QString m_text;
    QMap<QString,QString> m_attributes;
};

class HtmlDoc : public HtmlElement
{
  public:
    HtmlDoc();
    
    QString html();

  protected:
    void writeElements( QXmlStreamWriter &xml, HtmlElement::List &elements );
};

#endif
