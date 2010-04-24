project(polka)

find_package(KDE4 REQUIRED)
include (KDE4Defaults)

include_directories(
  ${KDE4_INCLUDES} ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

add_subdirectory( doc )
add_subdirectory( lib/polka )
add_subdirectory( src )
add_subdirectory( icons )
add_subdirectory( pics )
