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
#ifndef PERSONVIEW_H
#define PERSONVIEW_H

#include "polka/polka.h"

#include <QtGui>
#include <QWebView>

class PictureSelector;
class PictureSelectorControls;
class RegionGrabber;
class PolkaModel;
class KDirWatch;

class PersonView : public QWidget
{
    Q_OBJECT
  public:
    PersonView( PolkaModel *, QWidget *parent = 0 );

    Polka::Identity identity() const;

  public slots:
    void showIdentity( const Polka::Identity & );

  signals:
    void closeRequested();
  
  protected slots:
    void showIdentity();
  
    void setImage( const QPixmap & );

    void grabPicture();
    void slotRegionGrabbed( const QPixmap & );

    void slotLinkClicked( const QUrl & );

    void debugHtml();
    void reloadDebugHtml();

  protected:
    void editName();

    void addEmail();
    void editEmail( const QString &id );
    void removeEmail( const QString &id );
    void commentEmail( const QString &id );

    void addDetail();
    void editDetail( const QString &id );
    void removeDetail( const QString &id );
    void commentDetail( const QString &id );

    void addPhone();
    void editPhone( const QString &id );
    void removePhone( const QString &id );
    void commentPhone( const QString &id );

    void addAddress();
    void editAddress( const QString &id );
    void removeAddress( const QString &id );
    void commentAddress( const QString &id );

    void addLink();
    void editLink( const QString &id );
    void removeLink( const QString &id );
    void commentLink( const QString &id );

    void addComment();
    void editComment( const QString &id );
    void removeComment( const QString &id );

    void requestClose();
  
  private:
    PolkaModel *m_model;
    Polka::Identity m_identity;
  
    QLabel *m_titleLabel;
    QWebView *m_webView;
    PictureSelector *m_pictureSelector;
    PictureSelectorControls *m_pictureSelectorControls;

    RegionGrabber *m_regionGrabber;

    KDirWatch *m_dirWatch;
};

#endif
