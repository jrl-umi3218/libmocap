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
  virtual-marker-one-point-measured.cc
  virtual-marker-two-points-measured.cc
  virtual-marker-three-points-measured.cc
  virtual-marker-two-points-ratio.cc
  virtual-marker-three-points-ratio.cc
  virtual-marker-relative-to-bone.cc
  )
SET_TARGET_PROPERTIES(
  mocap PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0.0.0
  )
INSTALL(TARGETS mocap DESTINATION ${CMAKE_INSTALL_LIBDIR})

IF(ENABLE_ROS_VIEWER)
  ADD_SUBDIRECTORY(ros-viewer)
ENDIF()

