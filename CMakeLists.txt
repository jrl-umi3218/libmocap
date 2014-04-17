# Copyright 2010, Thomas Moulard, LAAS-CNRS
#
# This file is part of roboptim-core.
# roboptim-core is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# roboptim-core is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Lesser Public License for more details.
# You should have received a copy of the GNU Lesser General Public License
# along with roboptim-core.  If not, see <http://www.gnu.org/licenses/>.

# Requires at least CMake 2.8 to configure the package.
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE(cmake/base.cmake)

SET(PROJECT_NAME libmocap)
SET(PROJECT_DESCRIPTION "libmocap")
SET(PROJECT_URL "http://github.com/jrl-umi3218/libmocap")

SET(HEADERS
  )

SETUP_PROJECT()

# Check compiler flags and enable them if possible
INCLUDE(CheckCCompilerFlag)

CHECK_C_COMPILER_FLAG(-fvisibility=hidden HAS_VISIBILITY)
IF(HAS_VISIBILITY)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
ENDIF()

CHECK_C_COMPILER_FLAG(-Wl,--no-undefined HAS_NO_UNDEFINED)
IF(HAS_NO_UNDEFINED)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-undefined")
ENDIF()
CHECK_C_COMPILER_FLAG(-Wl,--no-allow-shlib-undefined HAS_NO_ALLOW_SHLIB_UNDEFINED)
IF(HAS_NO_ALLOW_SHLIB_UNDEFINED)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-allow-shlib-undefined")
ENDIF()
CHECK_C_COMPILER_FLAG(-Wl,--as-needed HAS_AS_NEEDED)
IF(HAS_AS_NEEDED)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--as-needed")
ENDIF()

# Add main library to pkg-config file.
PKG_CONFIG_APPEND_LIBS(mocap)

# Should we compile the viewer?
# If yes, ncurses and ROS are required.
OPTION(ENABLE_ROS_VIEWER "If false, ros-viewer is built (ROS and ncurses required)" TRUE)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)

SETUP_PROJECT_FINALIZE()
