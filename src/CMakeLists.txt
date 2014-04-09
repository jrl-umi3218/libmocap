ADD_LIBRARY(
  mocap SHARED
  abstract-marker.cc
  abstract-virtual-marker.cc
  link.cc
  marker-set-factory.cc
  marker-set.cc
  marker.cc
  pose.cc
  segment.cc
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
