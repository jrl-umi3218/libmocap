libmocap
========

[![Build Status](https://travis-ci.org/jrl-umi3218/libmocap.svg?branch=master)](https://travis-ci.org/jrl-umi3218/libmocap)
[![Coverage Status](https://coveralls.io/repos/jrl-umi3218/libmocap/badge.png)](https://coveralls.io/r/jrl-umi3218/libmocap)

***This work is distributed under The BSD 3-Clause License.***


libmocap is a C++ library with no dependencies aiming at Motion
Capture Data.

libmocap also includes an *optional* ROS viewer relying on ROS and
ncurses. It can be disabled by setting `ENABLE_ROS_VIEWER` to false.
When the viewer is running, it streams MarkerArray messages which can
then be displayed in rviz.


Supported Formats:

 - Cortex (Motion Analysis) file formats:
   - `*.mars` (Marker Set)
   - `*.trc` (Marker Position / track data)


These loaders have been retro-engineered using the GUI documentation
and there is no guarantee they will work for any file.


### Missing Features

 * Segments Hierarchy not loaded
 * Segments rotation offset not loaded


![rviz displaying motion capture data](doc/libmocap-viewer.png "rviz displaying motion capture data")


### License

Copyright (c) 2014, CNRS-AIST JRL UMI3218
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
