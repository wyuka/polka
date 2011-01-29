/*
    This file is part of KDE.

    Copyright (C) 2009-2011 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef POLKAVIEW_H
#define POLKAVIEW_H

#include "polka/polka.h"

#include <QtGui>

class QPainter;
class KUrl;
class PolkaModel;
class GroupListView;
class IdentityGraphicsView;
class PersonView;
class SettingsWidget;
class PolkaView : public QWidget
{
    Q_OBJECT
  public:
    PolkaView(QWidget *parent);
    virtual ~PolkaView();

    void readConfig();
    void writeConfig();

    void readData();
    void writeData( const QString &msg );

  public slots:
    void newPerson();
    void newSubGroup();

    void cloneGroup( const Polka::Identity &group );
    void removeGroup( const Polka::Identity &group );

    void showRoot();
    void showView();
    void goBack();
    void showGroup( const Polka::Identity & );
    void showIdentity( const Polka::Identity & );
    void showPerson( const Polka::Identity & );
    void removeIdentity( const Polka::Identity &person,
      const Polka::Identity &group );

    void showSettings();

  signals:
    void dataWritten();
  
  protected slots:
    void finishShowPerson();
    void closePersonView();
    void continueShowGroup();

  private:
    PolkaModel *m_model;

    Polka::Identity m_group;

    QStringList m_history;

    QPushButton *m_backButton;
    QLabel *m_groupNameLabel;  
    SettingsWidget *m_settingsWidget;
    QWidget *m_groupWidget;
    QStackedLayout *m_listLayout;
    GroupListView *m_groupView;
    IdentityGraphicsView *m_groupGraphicsView;
    PersonView *m_personView;
};

#endif
