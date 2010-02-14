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

#include "polka.h"

#include <QtDebug>
#include <QFile>
#include <QDomDocument>
#include <QtCore/QTextStream>
#include <QtCore/QtDebug>
#include <QtCore/QFile>

QString indent( int n = 0 )
{
  static int i = 0;
  i += n;
  QString space;
  return space.fill( ' ', i );
}

void IdentityPosition::setId( const QString &v )
{
  mId = v;
}

QString IdentityPosition::id() const
{
  return mId;
}

void IdentityPosition::setX( int v )
{
  mX = v;
}

int IdentityPosition::x() const
{
  return mX;
}

void IdentityPosition::setY( int v )
{
  mY = v;
}

int IdentityPosition::y() const
{
  return mY;
}

IdentityPosition IdentityPosition::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "identity_position" ) {
    qCritical() << "Expected 'identity_position', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return IdentityPosition();
  }

  IdentityPosition result = IdentityPosition();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
    else if ( e.tagName() == "x" ) {
      result.setX( e.text().toInt() );
    }
    else if ( e.tagName() == "y" ) {
      result.setY( e.text().toInt() );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString IdentityPosition::writeElement()
{
  QString xml;
  xml += indent() + "<identity_position>\n";
  indent( 2 );
  if ( !id().isEmpty() ) {
    xml += indent() + "<id>" + id() + "</id>\n";
  }
  xml += indent() + "<x>" + QString::number( x() ) + "</x>\n";
  xml += indent() + "<y>" + QString::number( y() ) + "</y>\n";
  indent( -2 );
  xml += indent() + "</identity_position>\n";
  return xml;
}


void GroupView::setId( const QString &v )
{
  mId = v;
}

QString GroupView::id() const
{
  return mId;
}

void GroupView::addIdentityPosition( const IdentityPosition &v )
{
  mIdentityPositionList.append( v );
}

void GroupView::setIdentityPositionList( const IdentityPosition::List &v )
{
  mIdentityPositionList = v;
}

IdentityPosition::List GroupView::identityPositionList() const
{
  return mIdentityPositionList;
}

IdentityPosition GroupView::findIdentityPosition( const QString &id )
{
  foreach( IdentityPosition v, mIdentityPositionList ) {
    if ( v.id() == id ) return v;
  }
  IdentityPosition v;
  v.setId( id );
  return v;
}

bool GroupView::insert( const IdentityPosition &v )
{
  int i = 0;
  for( ; i < mIdentityPositionList.size(); ++i ) {
    if ( mIdentityPositionList[i].id() == v.id() ) {
      mIdentityPositionList[i] = v;
      return true;
    }
  }
  if ( i == mIdentityPositionList.size() ) {
    addIdentityPosition( v );
  }
  return true;
}

GroupView GroupView::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "group_view" ) {
    qCritical() << "Expected 'group_view', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return GroupView();
  }

  GroupView result = GroupView();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
    else if ( e.tagName() == "identity_position" ) {
      bool ok;
      IdentityPosition o = IdentityPosition::parseElement( e, &ok );
      if ( ok ) result.addIdentityPosition( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString GroupView::writeElement()
{
  QString xml;
  xml += indent() + "<group_view>\n";
  indent( 2 );
  if ( !id().isEmpty() ) {
    xml += indent() + "<id>" + id() + "</id>\n";
  }
  foreach( IdentityPosition e, identityPositionList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</group_view>\n";
  return xml;
}


void Comment::setId( const QString &v )
{
  mId = v;
}

QString Comment::id() const
{
  return mId;
}

void Comment::setText( const QString &v )
{
  mText = v;
}

QString Comment::text() const
{
  return mText;
}

Comment Comment::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "comment" ) {
    qCritical() << "Expected 'comment', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Comment();
  }

  Comment result = Comment();

  result.setText( element.text() );
  result.setId( element.attribute( "id" ) );

  if ( ok ) *ok = true;
  return result;
}

QString Comment::writeElement()
{
  QString xml;
  if ( !text().isEmpty() ) {
    xml += indent() + "<comment id=\"" + id() + "\">" + text() + "</comment>\n";
  }
  return xml;
}


void Comments::addComment( const Comment &v )
{
  mCommentList.append( v );
}

void Comments::setCommentList( const Comment::List &v )
{
  mCommentList = v;
}

Comment::List Comments::commentList() const
{
  return mCommentList;
}

Comments Comments::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "comments" ) {
    qCritical() << "Expected 'comments', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Comments();
  }

  Comments result = Comments();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "comment" ) {
      bool ok;
      Comment o = Comment::parseElement( e, &ok );
      if ( ok ) result.addComment( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Comments::writeElement()
{
  QString xml;
  xml += indent() + "<comments>\n";
  indent( 2 );
  foreach( Comment e, commentList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</comments>\n";
  return xml;
}


void Attribute::setType( const QString &v )
{
  mType = v;
}

QString Attribute::type() const
{
  return mType;
}

void Attribute::setKey( const QString &v )
{
  mKey = v;
}

QString Attribute::key() const
{
  return mKey;
}

void Attribute::setValue( const QString &v )
{
  mValue = v;
}

QString Attribute::value() const
{
  return mValue;
}

Attribute Attribute::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "attribute" ) {
    qCritical() << "Expected 'attribute', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Attribute();
  }

  Attribute result = Attribute();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "key" ) {
      result.setKey( e.text() );
    }
    else if ( e.tagName() == "value" ) {
      result.setValue( e.text() );
    }
  }

  result.setType( element.attribute( "type" ) );

  if ( ok ) *ok = true;
  return result;
}

QString Attribute::writeElement()
{
  QString xml;
  xml += indent() + "<attribute type=\"" + type() + "\">\n";
  indent( 2 );
  if ( !key().isEmpty() ) {
    xml += indent() + "<key>" + key() + "</key>\n";
  }
  if ( !value().isEmpty() ) {
    xml += indent() + "<value>" + value() + "</value>\n";
  }
  indent( -2 );
  xml += indent() + "</attribute>\n";
  return xml;
}


void ExtendedAttributes::addAttribute( const Attribute &v )
{
  mAttributeList.append( v );
}

void ExtendedAttributes::setAttributeList( const Attribute::List &v )
{
  mAttributeList = v;
}

Attribute::List ExtendedAttributes::attributeList() const
{
  return mAttributeList;
}

ExtendedAttributes ExtendedAttributes::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "extended_attributes" ) {
    qCritical() << "Expected 'extended_attributes', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return ExtendedAttributes();
  }

  ExtendedAttributes result = ExtendedAttributes();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "attribute" ) {
      bool ok;
      Attribute o = Attribute::parseElement( e, &ok );
      if ( ok ) result.addAttribute( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString ExtendedAttributes::writeElement()
{
  QString xml;
  xml += indent() + "<extended_attributes>\n";
  indent( 2 );
  foreach( Attribute e, attributeList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</extended_attributes>\n";
  return xml;
}


void Profile::setProfileType( const QString &v )
{
  mProfileType = v;
}

QString Profile::profileType() const
{
  return mProfileType;
}

void Profile::setUrl( const QString &v )
{
  mUrl = v;
}

QString Profile::url() const
{
  return mUrl;
}

Profile Profile::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "profile" ) {
    qCritical() << "Expected 'profile', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Profile();
  }

  Profile result = Profile();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "profile_type" ) {
      result.setProfileType( e.text() );
    }
    else if ( e.tagName() == "url" ) {
      result.setUrl( e.text() );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Profile::writeElement()
{
  QString xml;
  xml += indent() + "<profile>\n";
  indent( 2 );
  if ( !profileType().isEmpty() ) {
    xml += indent() + "<profile_type>" + profileType() + "</profile_type>\n";
  }
  if ( !url().isEmpty() ) {
    xml += indent() + "<url>" + url() + "</url>\n";
  }
  indent( -2 );
  xml += indent() + "</profile>\n";
  return xml;
}


void Profiles::addProfile( const Profile &v )
{
  mProfileList.append( v );
}

void Profiles::setProfileList( const Profile::List &v )
{
  mProfileList = v;
}

Profile::List Profiles::profileList() const
{
  return mProfileList;
}

Profiles Profiles::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "profiles" ) {
    qCritical() << "Expected 'profiles', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Profiles();
  }

  Profiles result = Profiles();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "profile" ) {
      bool ok;
      Profile o = Profile::parseElement( e, &ok );
      if ( ok ) result.addProfile( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Profiles::writeElement()
{
  QString xml;
  xml += indent() + "<profiles>\n";
  indent( 2 );
  foreach( Profile e, profileList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</profiles>\n";
  return xml;
}


void Note::setId( const QString &v )
{
  mId = v;
}

QString Note::id() const
{
  return mId;
}

void Note::setCreatedAt( const QString &v )
{
  mCreatedAt = v;
}

QString Note::createdAt() const
{
  return mCreatedAt;
}

void Note::setUpdatedAt( const QString &v )
{
  mUpdatedAt = v;
}

QString Note::updatedAt() const
{
  return mUpdatedAt;
}

void Note::setText( const QString &v )
{
  mText = v;
}

QString Note::text() const
{
  return mText;
}

Note Note::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "note" ) {
    qCritical() << "Expected 'note', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Note();
  }

  Note result = Note();

  result.setText( element.text() );
  result.setId( element.attribute( "id" ) );
  result.setCreatedAt( element.attribute( "created_at" ) );
  result.setUpdatedAt( element.attribute( "updated_at" ) );

  if ( ok ) *ok = true;
  return result;
}

QString Note::writeElement()
{
  QString xml;
  if ( !text().isEmpty() ) {
    xml += indent() + "<note id=\"" + id() + "\" created_at=\"" + createdAt() + "\" updated_at=\"" + updatedAt() + "\">" + text() + "</note>\n";
  }
  return xml;
}


void Notes::addNote( const Note &v )
{
  mNoteList.append( v );
}

void Notes::setNoteList( const Note::List &v )
{
  mNoteList = v;
}

Note::List Notes::noteList() const
{
  return mNoteList;
}

Notes Notes::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "notes" ) {
    qCritical() << "Expected 'notes', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Notes();
  }

  Notes result = Notes();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "note" ) {
      bool ok;
      Note o = Note::parseElement( e, &ok );
      if ( ok ) result.addNote( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Notes::writeElement()
{
  QString xml;
  xml += indent() + "<notes>\n";
  indent( 2 );
  foreach( Note e, noteList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</notes>\n";
  return xml;
}


void Relation::setRelationType( const QString &v )
{
  mRelationType = v;
}

QString Relation::relationType() const
{
  return mRelationType;
}

void Relation::setTarget( const QString &v )
{
  mTarget = v;
}

QString Relation::target() const
{
  return mTarget;
}

Relation Relation::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "relation" ) {
    qCritical() << "Expected 'relation', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Relation();
  }

  Relation result = Relation();

  result.setRelationType( element.attribute( "relation_type" ) );
  result.setTarget( element.attribute( "target" ) );

  if ( ok ) *ok = true;
  return result;
}

QString Relation::writeElement()
{
  QString xml;
  xml += indent() + "<relation relation_type=\"" + relationType() + "\" target=\"" + target() + "\"/>\n";
  return xml;
}


void Relations::addRelation( const Relation &v )
{
  mRelationList.append( v );
}

void Relations::setRelationList( const Relation::List &v )
{
  mRelationList = v;
}

Relation::List Relations::relationList() const
{
  return mRelationList;
}

Relations Relations::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "relations" ) {
    qCritical() << "Expected 'relations', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Relations();
  }

  Relations result = Relations();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "relation" ) {
      bool ok;
      Relation o = Relation::parseElement( e, &ok );
      if ( ok ) result.addRelation( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Relations::writeElement()
{
  QString xml;
  xml += indent() + "<relations>\n";
  indent( 2 );
  foreach( Relation e, relationList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</relations>\n";
  return xml;
}


void Phone::setComment( const QString &v )
{
  mComment = v;
}

QString Phone::comment() const
{
  return mComment;
}

void Phone::setPhoneType( const QString &v )
{
  mPhoneType = v;
}

QString Phone::phoneType() const
{
  return mPhoneType;
}

void Phone::setPhoneNumber( const QString &v )
{
  mPhoneNumber = v;
}

QString Phone::phoneNumber() const
{
  return mPhoneNumber;
}

Phone Phone::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "phone" ) {
    qCritical() << "Expected 'phone', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Phone();
  }

  Phone result = Phone();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "phone_type" ) {
      result.setPhoneType( e.text() );
    }
    else if ( e.tagName() == "phone_number" ) {
      result.setPhoneNumber( e.text() );
    }
  }

  result.setComment( element.attribute( "comment" ) );

  if ( ok ) *ok = true;
  return result;
}

QString Phone::writeElement()
{
  QString xml;
  xml += indent() + "<phone comment=\"" + comment() + "\">\n";
  indent( 2 );
  if ( !phoneType().isEmpty() ) {
    xml += indent() + "<phone_type>" + phoneType() + "</phone_type>\n";
  }
  if ( !phoneNumber().isEmpty() ) {
    xml += indent() + "<phone_number>" + phoneNumber() + "</phone_number>\n";
  }
  indent( -2 );
  xml += indent() + "</phone>\n";
  return xml;
}


void Phones::addPhone( const Phone &v )
{
  mPhoneList.append( v );
}

void Phones::setPhoneList( const Phone::List &v )
{
  mPhoneList = v;
}

Phone::List Phones::phoneList() const
{
  return mPhoneList;
}

Phones Phones::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "phones" ) {
    qCritical() << "Expected 'phones', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Phones();
  }

  Phones result = Phones();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "phone" ) {
      bool ok;
      Phone o = Phone::parseElement( e, &ok );
      if ( ok ) result.addPhone( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Phones::writeElement()
{
  QString xml;
  xml += indent() + "<phones>\n";
  indent( 2 );
  foreach( Phone e, phoneList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</phones>\n";
  return xml;
}


void Picture::setUpdatedAt( const QString &v )
{
  mUpdatedAt = v;
}

QString Picture::updatedAt() const
{
  return mUpdatedAt;
}

void Picture::setPictureType( const QString &v )
{
  mPictureType = v;
}

QString Picture::pictureType() const
{
  return mPictureType;
}

void Picture::setId( const QString &v )
{
  mId = v;
}

QString Picture::id() const
{
  return mId;
}

void Picture::setUrl( const QString &v )
{
  mUrl = v;
}

QString Picture::url() const
{
  return mUrl;
}

Picture Picture::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "picture" ) {
    qCritical() << "Expected 'picture', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Picture();
  }

  Picture result = Picture();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
    else if ( e.tagName() == "url" ) {
      result.setUrl( e.text() );
    }
  }

  result.setUpdatedAt( element.attribute( "updated_at" ) );
  result.setPictureType( element.attribute( "picture_type" ) );

  if ( ok ) *ok = true;
  return result;
}

QString Picture::writeElement()
{
  QString xml;
  xml += indent() + "<picture updated_at=\"" + updatedAt() + "\" picture_type=\"" + pictureType() + "\">\n";
  indent( 2 );
  if ( !id().isEmpty() ) {
    xml += indent() + "<id>" + id() + "</id>\n";
  }
  if ( !url().isEmpty() ) {
    xml += indent() + "<url>" + url() + "</url>\n";
  }
  indent( -2 );
  xml += indent() + "</picture>\n";
  return xml;
}


void Pictures::addPicture( const Picture &v )
{
  mPictureList.append( v );
}

void Pictures::setPictureList( const Picture::List &v )
{
  mPictureList = v;
}

Picture::List Pictures::pictureList() const
{
  return mPictureList;
}

Pictures Pictures::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "pictures" ) {
    qCritical() << "Expected 'pictures', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Pictures();
  }

  Pictures result = Pictures();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "picture" ) {
      bool ok;
      Picture o = Picture::parseElement( e, &ok );
      if ( ok ) result.addPicture( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Pictures::writeElement()
{
  QString xml;
  xml += indent() + "<pictures>\n";
  indent( 2 );
  foreach( Picture e, pictureList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</pictures>\n";
  return xml;
}


void Email::setUpdatedAt( const QString &v )
{
  mUpdatedAt = v;
}

QString Email::updatedAt() const
{
  return mUpdatedAt;
}

void Email::setText( const QString &v )
{
  mText = v;
}

QString Email::text() const
{
  return mText;
}

Email Email::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "email" ) {
    qCritical() << "Expected 'email', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Email();
  }

  Email result = Email();

  result.setText( element.text() );
  result.setUpdatedAt( element.attribute( "updated_at" ) );

  if ( ok ) *ok = true;
  return result;
}

QString Email::writeElement()
{
  QString xml;
  if ( !text().isEmpty() ) {
    xml += indent() + "<email updated_at=\"" + updatedAt() + "\">" + text() + "</email>\n";
  }
  return xml;
}


void Emails::addEmail( const Email &v )
{
  mEmailList.append( v );
}

void Emails::setEmailList( const Email::List &v )
{
  mEmailList = v;
}

Email::List Emails::emailList() const
{
  return mEmailList;
}

Emails Emails::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "emails" ) {
    qCritical() << "Expected 'emails', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Emails();
  }

  Emails result = Emails();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "email" ) {
      bool ok;
      Email o = Email::parseElement( e, &ok );
      if ( ok ) result.addEmail( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Emails::writeElement()
{
  QString xml;
  xml += indent() + "<emails>\n";
  indent( 2 );
  foreach( Email e, emailList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</emails>\n";
  return xml;
}


void Group::setId( const QString &v )
{
  mId = v;
}

QString Group::id() const
{
  return mId;
}

Group Group::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "group" ) {
    qCritical() << "Expected 'group', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Group();
  }

  Group result = Group();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Group::writeElement()
{
  QString xml;
  xml += indent() + "<group>\n";
  indent( 2 );
  if ( !id().isEmpty() ) {
    xml += indent() + "<id>" + id() + "</id>\n";
  }
  indent( -2 );
  xml += indent() + "</group>\n";
  return xml;
}


void Groups::addGroup( const Group &v )
{
  mGroupList.append( v );
}

void Groups::setGroupList( const Group::List &v )
{
  mGroupList = v;
}

Group::List Groups::groupList() const
{
  return mGroupList;
}

Groups Groups::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "groups" ) {
    qCritical() << "Expected 'groups', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Groups();
  }

  Groups result = Groups();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "group" ) {
      bool ok;
      Group o = Group::parseElement( e, &ok );
      if ( ok ) result.addGroup( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Groups::writeElement()
{
  QString xml;
  xml += indent() + "<groups>\n";
  indent( 2 );
  foreach( Group e, groupList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</groups>\n";
  return xml;
}


void Name::setUpdatedAt( const QString &v )
{
  mUpdatedAt = v;
}

QString Name::updatedAt() const
{
  return mUpdatedAt;
}

void Name::setText( const QString &v )
{
  mText = v;
}

QString Name::text() const
{
  return mText;
}

Name Name::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "name" ) {
    qCritical() << "Expected 'name', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Name();
  }

  Name result = Name();

  result.setText( element.text() );
  result.setUpdatedAt( element.attribute( "updated_at" ) );

  if ( ok ) *ok = true;
  return result;
}

QString Name::writeElement()
{
  QString xml;
  if ( !text().isEmpty() ) {
    xml += indent() + "<name updated_at=\"" + updatedAt() + "\">" + text() + "</name>\n";
  }
  return xml;
}


void Identity::setId( const QString &v )
{
  mId = v;
}

QString Identity::id() const
{
  return mId;
}

void Identity::setGroups( const Groups &v )
{
  mGroups = v;
}

Groups Identity::groups() const
{
  return mGroups;
}

void Identity::setDisplayName( const QString &v )
{
  mDisplayName = v;
}

QString Identity::displayName() const
{
  return mDisplayName;
}

void Identity::setName( const Name &v )
{
  mName = v;
}

Name Identity::name() const
{
  return mName;
}

void Identity::setBirthname( const QString &v )
{
  mBirthname = v;
}

QString Identity::birthname() const
{
  return mBirthname;
}

void Identity::setBirthday( const QDate &v )
{
  mBirthday = v;
}

QDate Identity::birthday() const
{
  return mBirthday;
}

void Identity::setEmails( const Emails &v )
{
  mEmails = v;
}

Emails Identity::emails() const
{
  return mEmails;
}

void Identity::setPictures( const Pictures &v )
{
  mPictures = v;
}

Pictures Identity::pictures() const
{
  return mPictures;
}

void Identity::setPhones( const Phones &v )
{
  mPhones = v;
}

Phones Identity::phones() const
{
  return mPhones;
}

void Identity::setRelations( const Relations &v )
{
  mRelations = v;
}

Relations Identity::relations() const
{
  return mRelations;
}

void Identity::setNotes( const Notes &v )
{
  mNotes = v;
}

Notes Identity::notes() const
{
  return mNotes;
}

void Identity::setProfiles( const Profiles &v )
{
  mProfiles = v;
}

Profiles Identity::profiles() const
{
  return mProfiles;
}

void Identity::setExtendedAttributes( const ExtendedAttributes &v )
{
  mExtendedAttributes = v;
}

ExtendedAttributes Identity::extendedAttributes() const
{
  return mExtendedAttributes;
}

void Identity::setComments( const Comments &v )
{
  mComments = v;
}

Comments Identity::comments() const
{
  return mComments;
}

Identity Identity::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "identity" ) {
    qCritical() << "Expected 'identity', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Identity();
  }

  Identity result = Identity();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "id" ) {
      result.setId( e.text() );
    }
    else if ( e.tagName() == "groups" ) {
      bool ok;
      Groups o = Groups::parseElement( e, &ok );
      if ( ok ) result.setGroups( o );
    }
    else if ( e.tagName() == "display_name" ) {
      result.setDisplayName( e.text() );
    }
    else if ( e.tagName() == "name" ) {
      bool ok;
      Name o = Name::parseElement( e, &ok );
      if ( ok ) result.setName( o );
    }
    else if ( e.tagName() == "birthname" ) {
      result.setBirthname( e.text() );
    }
    else if ( e.tagName() == "birthday" ) {
      result.setBirthday( QDate::fromString( e.text(), Qt::ISODate ) );
    }
    else if ( e.tagName() == "emails" ) {
      bool ok;
      Emails o = Emails::parseElement( e, &ok );
      if ( ok ) result.setEmails( o );
    }
    else if ( e.tagName() == "pictures" ) {
      bool ok;
      Pictures o = Pictures::parseElement( e, &ok );
      if ( ok ) result.setPictures( o );
    }
    else if ( e.tagName() == "phones" ) {
      bool ok;
      Phones o = Phones::parseElement( e, &ok );
      if ( ok ) result.setPhones( o );
    }
    else if ( e.tagName() == "relations" ) {
      bool ok;
      Relations o = Relations::parseElement( e, &ok );
      if ( ok ) result.setRelations( o );
    }
    else if ( e.tagName() == "notes" ) {
      bool ok;
      Notes o = Notes::parseElement( e, &ok );
      if ( ok ) result.setNotes( o );
    }
    else if ( e.tagName() == "profiles" ) {
      bool ok;
      Profiles o = Profiles::parseElement( e, &ok );
      if ( ok ) result.setProfiles( o );
    }
    else if ( e.tagName() == "extended_attributes" ) {
      bool ok;
      ExtendedAttributes o = ExtendedAttributes::parseElement( e, &ok );
      if ( ok ) result.setExtendedAttributes( o );
    }
    else if ( e.tagName() == "comments" ) {
      bool ok;
      Comments o = Comments::parseElement( e, &ok );
      if ( ok ) result.setComments( o );
    }
  }


  if ( ok ) *ok = true;
  return result;
}

QString Identity::writeElement()
{
  QString xml;
  xml += indent() + "<identity>\n";
  indent( 2 );
  if ( !id().isEmpty() ) {
    xml += indent() + "<id>" + id() + "</id>\n";
  }
  xml += groups().writeElement();
  if ( !displayName().isEmpty() ) {
    xml += indent() + "<display_name>" + displayName() + "</display_name>\n";
  }
  xml += name().writeElement();
  if ( !birthname().isEmpty() ) {
    xml += indent() + "<birthname>" + birthname() + "</birthname>\n";
  }
  xml += indent() + "<birthday>" + birthday().toString( Qt::ISODate ) + "</birthday>\n";
  xml += emails().writeElement();
  xml += pictures().writeElement();
  xml += phones().writeElement();
  xml += relations().writeElement();
  xml += notes().writeElement();
  xml += profiles().writeElement();
  xml += extendedAttributes().writeElement();
  xml += comments().writeElement();
  indent( -2 );
  xml += indent() + "</identity>\n";
  return xml;
}


void Polka::setSchemaVersion( const QString &v )
{
  mSchemaVersion = v;
}

QString Polka::schemaVersion() const
{
  return mSchemaVersion;
}

void Polka::addIdentity( const Identity &v )
{
  mIdentityList.append( v );
}

void Polka::setIdentityList( const Identity::List &v )
{
  mIdentityList = v;
}

Identity::List Polka::identityList() const
{
  return mIdentityList;
}

void Polka::addGroupView( const GroupView &v )
{
  mGroupViewList.append( v );
}

void Polka::setGroupViewList( const GroupView::List &v )
{
  mGroupViewList = v;
}

GroupView::List Polka::groupViewList() const
{
  return mGroupViewList;
}

GroupView Polka::findGroupView( const QString &id )
{
  foreach( GroupView v, mGroupViewList ) {
    if ( v.id() == id ) return v;
  }
  GroupView v;
  v.setId( id );
  return v;
}

bool Polka::insert( const GroupView &v )
{
  int i = 0;
  for( ; i < mGroupViewList.size(); ++i ) {
    if ( mGroupViewList[i].id() == v.id() ) {
      mGroupViewList[i] = v;
      return true;
    }
  }
  if ( i == mGroupViewList.size() ) {
    addGroupView( v );
  }
  return true;
}

Polka Polka::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "polka" ) {
    qCritical() << "Expected 'polka', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Polka();
  }

  Polka result = Polka();

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "identity" ) {
      bool ok;
      Identity o = Identity::parseElement( e, &ok );
      if ( ok ) result.addIdentity( o );
    }
    else if ( e.tagName() == "group_view" ) {
      bool ok;
      GroupView o = GroupView::parseElement( e, &ok );
      if ( ok ) result.addGroupView( o );
    }
  }

  result.setSchemaVersion( element.attribute( "schemaVersion" ) );

  if ( ok ) *ok = true;
  return result;
}

QString Polka::writeElement()
{
  QString xml;
  xml += indent() + "<polka schemaVersion=\"" + schemaVersion() + "\">\n";
  indent( 2 );
  foreach( Identity e, identityList() ) {
    xml += e.writeElement();
  }
  foreach( GroupView e, groupViewList() ) {
    xml += e.writeElement();
  }
  indent( -2 );
  xml += indent() + "</polka>\n";
  return xml;
}

Polka Polka::parseFile( const QString &filename, bool *ok )
{
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    qCritical() << "Unable to open file '" << filename << "'";
    if ( ok ) *ok = false;
    return Polka();
  }

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &file, false, &errorMsg, &errorLine, &errorCol ) ) {
    qCritical() << errorMsg << " at " << errorLine << "," << errorCol;
    if ( ok ) *ok = false;
    return Polka();
  }

  qDebug() << "CONTENT:" << doc.toString();

  bool documentOk;
  Polka c = parseElement( doc.documentElement(), &documentOk );
  if ( ok ) {
    *ok = documentOk;
  }
  return c;
}

bool Polka::writeFile( const QString &filename )
{
  QFile file( filename );
  if ( !file.open( QIODevice::WriteOnly ) ) {
    qCritical() << "Unable to open file '" << filename << "'";
    return false;
  }

  QTextStream ts( &file );
  ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  ts << writeElement();
  file.close();

  return true;
}


