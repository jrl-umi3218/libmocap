INCLUDE(../../cmake/ros.cmake)

FIND_PACKAGE(Curses)

INCLUDE_DIRECTORIES(${CURSES_INCLUDE_DIR})

ADD_REQUIRED_DEPENDENCY(rosconsole)
ADD_REQUIRED_DEPENDENCY(roscpp)

ADD_EXECUTABLE(ros-viewer
  ros-viewer.cc

  marker-set-link-view.cc
  marker-set-name-view.cc
  marker-trajectory-view.cc
  view.cc)

PKG_CONFIG_USE_DEPENDENCY(ros-viewer rosconsole)
PKG_CONFIG_USE_DEPENDENCY(ros-viewer roscpp)
TARGET_LINK_LIBRARIES(ros-viewer
  mocap ${CURSES_LIBRARIES}
  )
