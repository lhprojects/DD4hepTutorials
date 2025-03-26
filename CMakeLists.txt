CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
project(DD4hepTutorials)

include(GNUInstallDirs)

find_package(DD4hep REQUIRED)

dd4hep_set_compiler_flags()
dd4hep_configure_output()

dd4hep_add_plugin(${PROJECT_NAME} SOURCES
  ${PROJECT_SOURCE_DIR}/simplecalo*/src/*.cpp USES
  DD4hep::DDCore
  DD4hep::DDCond
  ROOT::Core
  ROOT::Geom
  ROOT::GenVector
  ROOT::MathCore
)

add_subdirectory(sdaction)


