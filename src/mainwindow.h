/*
 * Copyright (C) 2008 Cornelius Schumacher <schumacher@kde.org>
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <kxmlguiwindow.h>

#include "ui_prefs_base.h"

class PolkaView;
class QPrinter;
class KToggleAction;
class KUrl;

/**
 * This class serves as the main window for Polka.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Cornelius Schumacher <schumacher@kde.org>
 * @version 0.1
 */
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
  public:
    MainWindow();
    virtual ~MainWindow();

  private slots:
    void fileNew();
    void optionsPreferences();

  private:
    void setupActions();

  private:
    Ui::prefs_base ui_prefs_base ;
    PolkaView *m_view;

    QPrinter   *m_printer;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif
