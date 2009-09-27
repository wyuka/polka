/*
 * polka.h
 *
 * Copyright (C) 2008 Cornelius Schumacher <schumacher@kde.org>
 */
#ifndef POLKA_H
#define POLKA_H


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
class Polka : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    Polka();

    /**
     * Default Destructor
     */
    virtual ~Polka();

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

#endif // _POLKA_H_
