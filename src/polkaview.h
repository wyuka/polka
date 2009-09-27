/*
 * polkaview.h
 *
 * Copyright (C) 2007 Cornelius Schumacher <schumacher@kde.org>
 */
#ifndef POLKAVIEW_H
#define POLKAVIEW_H

#include <QtGui/QWidget>

#include "ui_polkaview_base.h"

class QPainter;
class KUrl;

/**
 * This is the main view class for Polka.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Cornelius Schumacher <schumacher@kde.org>
 * @version 0.1
 */

class PolkaView : public QWidget, public Ui::polkaview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    PolkaView(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~PolkaView();

private:
    Ui::polkaview_base ui_polkaview_base;

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

private slots:
    void switchColors();
    void settingsChanged();
};

#endif // PolkaVIEW_H
