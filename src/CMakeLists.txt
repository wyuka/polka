set(polka_bin_SRCS
  matchlist.cpp
  searchresultview.cpp
  searchedit.cpp
  fanmenuitem.cpp
  buttonitem.cpp
  groupadderitem.cpp
  historyview.cpp
  overview.cpp
  groupview.cpp
  pictureselectorcontrols.cpp
  roundedrectitem.cpp
  settingswidget.cpp
  magicmenuitem.cpp
  mainmenuitem.cpp
  newgroupdialog.cpp
  labelitem.cpp
  addresseditor.cpp
  linkeditor.cpp
  phoneeditor.cpp
  commenteditor.cpp
  detaileditor.cpp
  fanmenuelement.cpp
  fanmenu.cpp
  gitremote.cpp
  identityitem.cpp
  groupgraphicsview.cpp
  newpersondialog.cpp
  grouplistview.cpp
  mainwindow.cpp
  main.cpp
  polkaview.cpp
  polkamodel.cpp
  gitdir.cpp
  gitcommand.cpp
  polkaitemmodel.cpp
  polkaallitemmodel.cpp
  polkapersonsitemmodel.cpp
  personview.cpp
  imageloader.cpp
  pictureselector.cpp
  pictureselectorbutton.cpp
  localpicture.cpp
  regiongrabber.cpp
  trackinggraphicsview.cpp
)

kde4_add_kcfg_files(polka_bin_SRCS settings.kcfgc )

kde4_add_executable(polka_bin ${polka_bin_SRCS})
set_target_properties( polka_bin PROPERTIES OUTPUT_NAME polka )

target_link_libraries(polka_bin polka ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_QTWEBKIT_LIBRARY})

install(TARGETS polka_bin ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES polka.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES polka.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES polkaui.rc  DESTINATION  ${DATA_INSTALL_DIR}/polka )
