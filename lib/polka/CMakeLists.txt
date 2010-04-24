set(polka_LIB_SRCS
  htmlrenderer.cpp
  polka.cpp
  htmlcreator.cpp
)

kde4_add_library( polka SHARED ${polka_LIB_SRCS} )

target_link_libraries( polka ${KDE4_KDEUI_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} )

set_target_properties( polka PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install( TARGETS polka ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES
  htmlrenderer.h
  polka.h
  htmlcreator.h
  polka_export.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/polka
)
