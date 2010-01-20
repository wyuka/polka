/*
 * polkaview.h
 *
 * Copyright (C) 2009 Cornelius Schumacher <schumacher@kde.org>
 */
#ifndef POLKAVIEW_H
#define POLKAVIEW_H

#include "polka.h"

#include <QtGui>

class QPainter;
class KUrl;
class PolkaModel;
class IdentityListView;
class GroupListView;
class IdentityGraphicsView;

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
    void writeData();

  public slots:
    void newPerson();
    void newGroup();

    void showGroupList();
    void showView();
    void showGroupView( const Identity & );
    void showPerson( const Identity & );
    void removePerson( const Identity &person, const Identity &group );

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

    Identity m_group;

    QCheckBox *m_graphicsModeCheck;
    QCheckBox *m_syncingCheck;
    QStackedLayout *m_viewLayout;
    GroupListView *m_groupListView;
    IdentityListView *m_groupView;
    IdentityGraphicsView *m_groupGraphicsView;
};

#endif
