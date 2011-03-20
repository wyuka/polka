QT       += core gui xml webkit

TARGET = Polka
CONFIG += meegotouch
TEMPLATE = app
target.path=/usr/local/bin
INSTALLS=target
DEFINES += MAKE_IT_MEEGO
INCLUDEPATH += ../lib ../microkde meego

SOURCES += main_polka.cpp \
    polkaview.cpp \
    polkamodel.cpp \
    ../microkde/KLocale.cpp \
    overview.cpp \
    grouplistview.cpp \
    groupgraphicsview.cpp \
    personview.cpp \
    historyview.cpp \
    settingswidget.cpp \
    meego/settings.cpp \
    ../microkde/KMessageBox.cpp \
    gitremote.cpp \
    newgroupdialog.cpp \
    ../lib/polka/polka.cpp \
    ../microkde/KInputDialog.cpp \
    newpersondialog.cpp \
    groupview.cpp \
    gitdir.cpp \
    polkaallitemmodel.cpp \
    polkapersonsitemmodel.cpp \
    polkaitemmodel.cpp \
    ../microkde/KRandom.cpp \
    gitcommand.cpp \
    localpicture.cpp \
    trackinggraphicsview.cpp \
    identityitem.cpp \
    magicmenuitem.cpp \
    mainmenuitem.cpp \
    groupadderitem.cpp \
    labelitem.cpp \
    fanmenuitem.cpp \
    fanmenuelement.cpp \
    fanmenu.cpp \
    pictureselectorcontrols.cpp \
    pictureselectorbutton.cpp \
    pictureselector.cpp \
    ../lib/polka/htmlrenderer.cpp \
    ../lib/polka/htmlcreator.cpp \
    roundedrectitem.cpp \
    regiongrabber.cpp \
    phoneeditor.cpp \
    linkeditor.cpp \
    commenteditor.cpp \
    buttonitem.cpp \
    addresseditor.cpp \
    ../microkde/KDialog.cpp \
    ../microkde/KPushButton.cpp \
    ../microkde/KStandardDirs.cpp

HEADERS += \
    ../microkde/kdemacros.h \
    polkaview.h \
    meego/settings.h \
    ../microkde/KInputDialog \
    ../microkde/KMessageBox \
    ../microkde/KLocale \
    ../microkde/KGlobal \
    ../microkde/KConfig \
    polkamodel.h \
    ../microkde/KRandom \
    overview.h \
    grouplistview.h \
    groupview.h \
    groupgraphicsview.h \
    personview.h \
    regiongrabber.h \
    trackinggraphicsview.h \
    settingswidget.h \
    roundedrectitem.h \
    polkapersonsitemmodel.h \
    polkaitemmodel.h \
    polkaallitemmodel.h \
    pictureselectorcontrols.h \
    pictureselectorbutton.h \
    pictureselector.h \
    phoneeditor.h \
    newpersondialog.h \
    newgroupdialog.h \
    mainmenuitem.h \
    magicmenuitem.h \
    localpicture.h \
    linkeditor.h \
    labelitem.h \
    identityitem.h \
    groupadderitem.h \
    gitremote.h \
    gitdir.h \
    gitcommand.h \
    historyview.h \
    fanmenuitem.h \
    fanmenuelement.h \
    fanmenu.h \
    commenteditor.h \
    addresseditor.h \
    buttonitem.h \
    ../microkde/KPushButton \
    ../microkde/KDialog.h \
    ../microkde/KStandardDirs.h

RESOURCES += \
    ../polka.qrc
