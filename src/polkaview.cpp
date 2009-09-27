/*
 * polkaview.cpp
 *
 * Copyright (C) 2008 Cornelius Schumacher <schumacher@kde.org>
 */
#include "polkaview.h"
#include "settings.h"

#include <klocale.h>
#include <QtGui/QLabel>

PolkaView::PolkaView(QWidget *)
{
    ui_polkaview_base.setupUi(this);
    settingsChanged();
    setAutoFillBackground(true);
}

PolkaView::~PolkaView()
{

}

void PolkaView::switchColors()
{
    // switch the foreground/background colors of the label
    QColor color = Settings::col_background();
    Settings::setCol_background( Settings::col_foreground() );
    Settings::setCol_foreground( color );

    settingsChanged();
}

void PolkaView::settingsChanged()
{
    QPalette pal;
    pal.setColor( QPalette::Window, Settings::col_background());
    pal.setColor( QPalette::WindowText, Settings::col_foreground());
    ui_polkaview_base.kcfg_sillyLabel->setPalette( pal );

    // i18n : internationalization
    ui_polkaview_base.kcfg_sillyLabel->setText( i18n("This project is %1 days old",Settings::val_time()) );
    emit signalChangeStatusbar( i18n("Settings changed") );
}

#include "polkaview.moc"
