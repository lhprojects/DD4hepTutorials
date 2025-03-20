project(simplecaloproject)

file(GLOB headers
  ${PROJECT_SOURCE_DIR}/include/*.h
)

dd4hep_add_plugin(simplecalo1 SOURCES src/*.cpp USES
  DD4hep::DDCore
  DD4hep::DDCond
  ROOT::Core
  ROOT::Geom
  ROOT::GenVector
  ROOT::MathCore
)

target_include_directories(simplecalo1 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(simplecalo1 PROPERTIES PUBLIC_HEADER "${headers}")

install(TARGETS simplecalo1
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev
)

install(DIRECTORY compact DESTINATION ${CMAKE_INSTALL_DATADIR})

dd4hep_configure_scripts( simplecalo1 DEFAULT_SETUP WITH_TESTS )
