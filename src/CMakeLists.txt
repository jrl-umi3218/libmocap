ADD_LIBRARY(
  mocap SHARED
  abstract-marker.cc
  abstract-virtual-marker.cc
  color.cc
  link.cc
  mars-marker-set-factory.cc
  marker-set-factory.cc
  marker-set.cc
  marker-trajectory.cc
  marker-trajectory-factory.cc
  trc-marker-trajectory-factory.cc
  marker.cc
  pose.cc
  segment.cc
  string.cc
  virtual-marker-measured.cc
  virtual-marker-ratio.cc
  virtual-marker-relative-to-bone.cc
  )
SET_TARGET_PROPERTIES(
  mocap PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0.0.0
  )
INSTALL(TARGETS mocap DESTINATION ${CMAKE_INSTALL_LIBDIR})
