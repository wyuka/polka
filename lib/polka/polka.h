// This file is generated by kxml_compiler from polka.xml.
// All changes you do to this file will be lost.
/*
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
#ifndef POLKA_POLKA_H
#define POLKA_POLKA_H

#include <polka/polka_export.h>
#include <QString>
#include <QDomElement>
#include <QList>
#include <QDateTime>
#include <QDate>
#include <QtXml/QXmlStreamWriter>

namespace Polka {

class POLKA_EXPORT ViewLabel
{
  public:
    typedef QList<ViewLabel> List;

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void setText( const QString &v );
    QString text() const;
    void setX( int v );
    int x() const;
    void setY( int v );
    int y() const;
    /**
      Parse XML object from DOM element.
     */
    static ViewLabel parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    QString mText;
    int mX;
    int mY;
};

class POLKA_EXPORT IdentityCheck
{
  public:
    typedef QList<IdentityCheck> List;

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void setChecked( const QString &v );
    QString checked() const;
    /**
      Parse XML object from DOM element.
     */
    static IdentityCheck parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    QString mChecked;
};

class POLKA_EXPORT IdentityPosition
{
  public:
    typedef QList<IdentityPosition> List;

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void setX( int v );
    int x() const;
    void setY( int v );
    int y() const;
    /**
      Parse XML object from DOM element.
     */
    static IdentityPosition parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    int mX;
    int mY;
};

class POLKA_EXPORT GroupView
{
  public:
    typedef QList<GroupView> List;

  public:
    enum Flags { None, AutoCreate };

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void addIdentityPosition( const IdentityPosition &v );
    void setIdentityPositionList( const IdentityPosition::List &v );
    IdentityPosition::List identityPositionList() const;
    IdentityPosition findIdentityPosition( const QString &id, Flags flags = None );
    bool insert( const IdentityPosition &v );
    bool remove( const IdentityPosition &v );
    void addIdentityCheck( const IdentityCheck &v );
    void setIdentityCheckList( const IdentityCheck::List &v );
    IdentityCheck::List identityCheckList() const;
    IdentityCheck findIdentityCheck( const QString &id, Flags flags = None );
    bool insert( const IdentityCheck &v );
    bool remove( const IdentityCheck &v );
    void addViewLabel( const ViewLabel &v );
    void setViewLabelList( const ViewLabel::List &v );
    ViewLabel::List viewLabelList() const;
    ViewLabel findViewLabel( const QString &id, Flags flags = None );
    bool insert( const ViewLabel &v );
    bool remove( const ViewLabel &v );
    /**
      Parse XML object from DOM element.
     */
    static GroupView parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    IdentityPosition::List mIdentityPositionList;
    IdentityCheck::List mIdentityCheckList;
    ViewLabel::List mViewLabelList;
};

class POLKA_EXPORT Comment
{
  public:
    typedef QList<Comment> List;

  public:
    Comment();
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setValue( const QString &v );
    QString value() const;
    /**
      Parse XML object from DOM element.
     */
    static Comment parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mValue;
};

class POLKA_EXPORT Comments
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void addComment( const Comment &v );
    void setCommentList( const Comment::List &v );
    Comment::List commentList() const;
    Comment findComment( const QString &id, Flags flags = None );
    bool insert( const Comment &v );
    bool remove( const Comment &v );
    /**
      Parse XML object from DOM element.
     */
    static Comments parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Comment::List mCommentList;
};

class POLKA_EXPORT Attribute
{
  public:
    typedef QList<Attribute> List;

  public:
    Attribute();
    void setType( const QString &v );
    QString type() const;
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setKey( const QString &v );
    QString key() const;
    void setValue( const QString &v );
    QString value() const;
    void setComment( const Comment &v );
    Comment comment() const;
    /**
      Parse XML object from DOM element.
     */
    static Attribute parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mType;
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mKey;
    QString mValue;
    Comment mComment;
};

class POLKA_EXPORT ExtendedAttributes
{
  public:
    void addAttribute( const Attribute &v );
    void setAttributeList( const Attribute::List &v );
    Attribute::List attributeList() const;
    /**
      Parse XML object from DOM element.
     */
    static ExtendedAttributes parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Attribute::List mAttributeList;
};

class POLKA_EXPORT Link
{
  public:
    typedef QList<Link> List;

  public:
    Link();
    bool isValid() const;
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setId( const QString &v );
    QString id() const;
    void setLinkType( const QString &v );
    QString linkType() const;
    void setUrl( const QString &v );
    QString url() const;
    void setComment( const Comment &v );
    Comment comment() const;
    /**
      Parse XML object from DOM element.
     */
    static Link parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mId;
    QString mLinkType;
    QString mUrl;
    Comment mComment;
};

class POLKA_EXPORT Links
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void addLink( const Link &v );
    void setLinkList( const Link::List &v );
    Link::List linkList() const;
    Link findLink( const QString &id, Flags flags = None );
    bool insert( const Link &v );
    bool remove( const Link &v );
    /**
      Parse XML object from DOM element.
     */
    static Links parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Link::List mLinkList;
};

class POLKA_EXPORT Relation
{
  public:
    typedef QList<Relation> List;

  public:
    void setRelationType( const QString &v );
    QString relationType() const;
    void setTarget( const QString &v );
    QString target() const;
    /**
      Parse XML object from DOM element.
     */
    static Relation parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mRelationType;
    QString mTarget;
};

class POLKA_EXPORT Relations
{
  public:
    void addRelation( const Relation &v );
    void setRelationList( const Relation::List &v );
    Relation::List relationList() const;
    /**
      Parse XML object from DOM element.
     */
    static Relations parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Relation::List mRelationList;
};

class POLKA_EXPORT Address
{
  public:
    typedef QList<Address> List;

  public:
    Address();
    bool isValid() const;
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setId( const QString &v );
    QString id() const;
    void setLabel( const QString &v );
    QString label() const;
    void setComment( const Comment &v );
    Comment comment() const;
    /**
      Parse XML object from DOM element.
     */
    static Address parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mId;
    QString mLabel;
    Comment mComment;
};

class POLKA_EXPORT Addresses
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void addAddress( const Address &v );
    void setAddressList( const Address::List &v );
    Address::List addressList() const;
    Address findAddress( const QString &id, Flags flags = None );
    bool insert( const Address &v );
    bool remove( const Address &v );
    /**
      Parse XML object from DOM element.
     */
    static Addresses parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Address::List mAddressList;
};

class POLKA_EXPORT Phone
{
  public:
    typedef QList<Phone> List;

  public:
    Phone();
    bool isValid() const;
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setId( const QString &v );
    QString id() const;
    void setPhoneType( const QString &v );
    QString phoneType() const;
    void setPhoneNumber( const QString &v );
    QString phoneNumber() const;
    void setComment( const Comment &v );
    Comment comment() const;
    /**
      Parse XML object from DOM element.
     */
    static Phone parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mId;
    QString mPhoneType;
    QString mPhoneNumber;
    Comment mComment;
};

class POLKA_EXPORT Phones
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void addPhone( const Phone &v );
    void setPhoneList( const Phone::List &v );
    Phone::List phoneList() const;
    Phone findPhone( const QString &id, Flags flags = None );
    bool insert( const Phone &v );
    bool remove( const Phone &v );
    /**
      Parse XML object from DOM element.
     */
    static Phones parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Phone::List mPhoneList;
};

class POLKA_EXPORT Picture
{
  public:
    typedef QList<Picture> List;

  public:
    Picture();
    bool isValid() const;
    void setPictureType( const QString &v );
    QString pictureType() const;
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setId( const QString &v );
    QString id() const;
    void setUrl( const QString &v );
    QString url() const;
    void setComment( const Comment &v );
    Comment comment() const;
    /**
      Parse XML object from DOM element.
     */
    static Picture parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mPictureType;
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mId;
    QString mUrl;
    Comment mComment;
};

class POLKA_EXPORT Pictures
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void addPicture( const Picture &v );
    void setPictureList( const Picture::List &v );
    Picture::List pictureList() const;
    Picture findPicture( const QString &id, Flags flags = None );
    bool insert( const Picture &v );
    bool remove( const Picture &v );
    /**
      Parse XML object from DOM element.
     */
    static Pictures parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Picture::List mPictureList;
};

class POLKA_EXPORT Email
{
  public:
    typedef QList<Email> List;

  public:
    Email();
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setEmailAddress( const QString &v );
    QString emailAddress() const;
    void setComment( const Comment &v );
    Comment comment() const;
    /**
      Parse XML object from DOM element.
     */
    static Email parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mEmailAddress;
    Comment mComment;
};

class POLKA_EXPORT Emails
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void addEmail( const Email &v );
    void setEmailList( const Email::List &v );
    Email::List emailList() const;
    Email findEmail( const QString &id, Flags flags = None );
    bool insert( const Email &v );
    bool remove( const Email &v );
    /**
      Parse XML object from DOM element.
     */
    static Emails parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Email::List mEmailList;
};

class POLKA_EXPORT Group
{
  public:
    typedef QList<Group> List;

  public:
    bool isValid() const;
    void setId( const QString &v );
    QString id() const;
    /**
      Parse XML object from DOM element.
     */
    static Group parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mId;
};

class POLKA_EXPORT Groups
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void addGroup( const Group &v );
    void setGroupList( const Group::List &v );
    Group::List groupList() const;
    Group findGroup( const QString &id, Flags flags = None );
    bool insert( const Group &v );
    bool remove( const Group &v );
    /**
      Parse XML object from DOM element.
     */
    static Groups parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Group::List mGroupList;
};

class POLKA_EXPORT Name
{
  public:
    Name();
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setValue( const QString &v );
    QString value() const;
    /**
      Parse XML object from DOM element.
     */
    static Name parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QString mValue;
};

class POLKA_EXPORT Birthday
{
  public:
    Birthday();
    void setCreatedAt( const QDateTime &v );
    QDateTime createdAt() const;
    void setUpdatedAt( const QDateTime &v );
    QDateTime updatedAt() const;
    void setValue( const QDate &v );
    QDate value() const;
    /**
      Parse XML object from DOM element.
     */
    static Birthday parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    QDate mValue;
};

class POLKA_EXPORT Identity
{
  public:
    typedef QList<Identity> List;

  public:
    bool isValid() const;
    void setType( const QString &v );
    QString type() const;
    void setId( const QString &v );
    QString id() const;
    void setGroups( const Groups &v );
    Groups groups() const;
    void setName( const Name &v );
    Name name() const;
    void setBirthday( const Birthday &v );
    Birthday birthday() const;
    void setEmails( const Emails &v );
    Emails emails() const;
    void setPictures( const Pictures &v );
    Pictures pictures() const;
    void setPhones( const Phones &v );
    Phones phones() const;
    void setAddresses( const Addresses &v );
    Addresses addresses() const;
    void setRelations( const Relations &v );
    Relations relations() const;
    void setLinks( const Links &v );
    Links links() const;
    void setExtendedAttributes( const ExtendedAttributes &v );
    ExtendedAttributes extendedAttributes() const;
    void setComments( const Comments &v );
    Comments comments() const;
    /**
      Parse XML object from DOM element.
     */
    static Identity parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    QString mType;
    QString mId;
    Groups mGroups;
    Name mName;
    Birthday mBirthday;
    Emails mEmails;
    Pictures mPictures;
    Phones mPhones;
    Addresses mAddresses;
    Relations mRelations;
    Links mLinks;
    ExtendedAttributes mExtendedAttributes;
    Comments mComments;
};

class POLKA_EXPORT Root
{
  public:
    void setGroup( const Group &v );
    Group group() const;
    /**
      Parse XML object from DOM element.
     */
    static Root parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );

  private:
    Group mGroup;
};

class POLKA_EXPORT Polka
{
  public:
    enum Flags { None, AutoCreate };

  public:
    void setSchemaVersion( int v );
    int schemaVersion() const;
    void setRoot( const Root &v );
    Root root() const;
    void addIdentity( const Identity &v );
    void setIdentityList( const Identity::List &v );
    Identity::List identityList() const;
    Identity findIdentity( const QString &id, Flags flags = None );
    bool insert( const Identity &v );
    bool remove( const Identity &v );
    void addGroupView( const GroupView &v );
    void setGroupViewList( const GroupView::List &v );
    GroupView::List groupViewList() const;
    GroupView findGroupView( const QString &id, Flags flags = None );
    bool insert( const GroupView &v );
    bool remove( const GroupView &v );
    /**
      Parse XML object from DOM element.
     */
    static Polka parseElement( const QDomElement &element, bool *ok );
    void writeElement( QXmlStreamWriter &xml );
    static Polka parseFile( const QString &filename, bool *ok );
    static Polka parseString( const QString &xml, bool *ok );
    bool writeFile( const QString &filename );

  private:
    int mSchemaVersion;
    Root mRoot;
    Identity::List mIdentityList;
    GroupView::List mGroupViewList;
};

}

#endif
