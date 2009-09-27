set(polka_SRCS
   mainwindow.cpp
   main.cpp
   polkaview.cpp
 )

kde4_add_ui_files(polka_SRCS polkaview_base.ui prefs_base.ui)

kde4_add_kcfg_files(polka_SRCS settings.kcfgc )

kde4_add_executable(polka ${polka_SRCS})

target_link_libraries(polka ${KDE4_KDEUI_LIBS} )

install(TARGETS polka ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES polka.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES polka.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES polkaui.rc  DESTINATION  ${DATA_INSTALL_DIR}/polka )
