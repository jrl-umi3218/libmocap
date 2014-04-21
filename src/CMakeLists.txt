ADD_LIBRARY(
  mocap SHARED

  abstract-marker.cc
  abstract-virtual-marker.cc
  color.cc
  link.cc
  marker-set-factory.cc
  marker-set.cc
  marker-trajectory-factory.cc
  marker-trajectory.cc
  marker.cc
  mars-marker-set-factory.cc
  math.cc
  pose.cc
  segment.cc
  string.cc
  trc-marker-trajectory-factory.cc
  virtual-marker-one-point-measured.cc
  virtual-marker-relative-to-bone.cc
  virtual-marker-three-points-measured.cc
  virtual-marker-three-points-ratio.cc
  virtual-marker-two-points-measured.cc
  virtual-marker-two-points-ratio.cc
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

