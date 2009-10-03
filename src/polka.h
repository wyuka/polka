// This file is generated by kxml_compiler from polka.xml.
// All changes you do to this file will be lost.
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
#ifndef POLKA_H
#define POLKA_H

#include <QString>
#include <QDomElement>
#include <QList>

class Identity
{
  public:
    typedef QList<Identity> List;

  public:
    void setIs_group( const QString &v );
    QString isGroup() const;
    void setId( const QString &v );
    QString id() const;
    void setName( const QString &v );
    QString name() const;
    /**
      Parse XML object from DOM element.
     */
    static Identity parseElement( const QDomElement &element, bool *ok );
    QString writeElement();

  private:
    QString mIs_group;
    QString mId;
    QString mName;
};

class Polka
{
  public:
    void addIdentity( const Identity &v );
    void setIdentityList( const Identity::List &v );
    Identity::List identityList() const;
    /**
      Parse XML object from DOM element.
     */
    static Polka parseElement( const QDomElement &element, bool *ok );
    QString writeElement();
    static Polka parseFile( const QString &filename, bool *ok );
    bool writeFile( const QString &filename );

  private:
    Identity::List mIdentityList;
};

#endif
