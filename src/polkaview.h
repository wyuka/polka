/*
 * polkaview.h
 *
 * Copyright (C) 2009 Cornelius Schumacher <schumacher@kde.org>
 */
#ifndef POLKAVIEW_H
#define POLKAVIEW_H

#include "polka/polka.h"

#include <QtGui>

class QPainter;
class KUrl;
class PolkaModel;
class IdentityListView;
class GroupListView;
class IdentityGraphicsView;
class PersonView;

/**
 * This is the main view class for Polka.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Cornelius Schumacher <schumacher@kde.org>
 * @version 0.1
 */

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

    void newGroup();
    void cloneGroup( const Polka::Identity &group );
    void removeGroup( const Polka::Identity &group );

    void showGroupList();
    void showView();
    void showGroupView( const Polka::Identity & );
    void showPerson( const Polka::Identity & );
    void removePerson( const Polka::Identity &person,
      const Polka::Identity &group );

  signals:
    void dataWritten();
  
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

  protected slots:
    void slotSyncingCheckChanged();

  private:
    PolkaModel *m_model;

    Polka::Identity m_group;

    QCheckBox *m_graphicsModeCheck;
    QCheckBox *m_syncingCheck;
    QStackedLayout *m_listLayout;
    GroupListView *m_groupListView;
    IdentityListView *m_groupView;
    IdentityGraphicsView *m_groupGraphicsView;
    PersonView *m_personView;
};

#endif
