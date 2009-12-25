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

    void readData();
    void writeData();

  public slots:
    void newPerson();
    void showGroupList();
    void showGroupView( const Identity & );

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

  private:
    PolkaModel *m_model;

    QPushButton *m_backButton;
    QLabel *m_groupNameLabel;
    QStackedLayout *m_viewLayout;
    GroupListView *m_groupListView;
    IdentityListView *m_groupView;
};

#endif
