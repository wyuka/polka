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

#include <polka/polka_export.h>

#include <QList>
#include <QString>
#include <QMap>
#include <QXmlStreamWriter>

class POLKA_EXPORT CssDeclaration
{
  public:
    typedef QList<CssDeclaration> List;
  
    CssDeclaration( const QString &property, const QString &value );
  
    QString property() const;
    QString value() const;
  
  private:
    QString m_property;
    QString m_value;
};

class POLKA_EXPORT CssRule
{
  public:
    CssRule();
    CssRule( const QString &selector );
  
    QString selector() const;

    CssDeclaration::List declarations() const;
    void addDeclaration( const CssDeclaration & );

    void add( const QString &property, const QString &value );
  
    void dump();
  
  private:
    QString m_selector;
    CssDeclaration::List m_declarations;
};

class POLKA_EXPORT CssSheet
{
  public:
    void addRule( const QString &selector, const QString &property,
      const QString &value );
    void addRule( const CssRule & );

    CssRule &rule( const QString &selector );
  
    QString render();

  private:
    QMap <QString,CssRule> m_rules;
};

class POLKA_EXPORT HtmlElement
{
  public:
    typedef QList<HtmlElement> List;
  
    HtmlElement();
    
    HtmlElement &element( const QString &name );

    HtmlElement &attribute( const QString &key, const QString &value );
    HtmlElement &attribute( const QString &key, const char * );
    HtmlElement &attribute( const QString &key, bool );

    /**
      Class attribute. Short function name as "class" is a reserved word.
    */
    HtmlElement &c( const QString &classValue );

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

class POLKA_EXPORT HtmlDoc : public HtmlElement
{
  public:
    HtmlDoc();

    void setCss( const CssSheet & );
    
    QString html();

  protected:
    void writeElements( QXmlStreamWriter &xml, HtmlElement::List &elements );

  private:
    CssSheet m_css;
};

#endif
