// Copyright (c) 2014, CNRS-AIST JRL/UMI3218
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <iostream>
#include "marker-trajectory-view.hh"

namespace libmocap
{
  MarkerTrajectoryView::MarkerTrajectoryView
  (const MarkerTrajectory& trajectory)
    : View (),
      trajectory_ (trajectory)
  {
    msg_.points.resize (trajectory.numMarkers ());
  }

  MarkerTrajectoryView::~MarkerTrajectoryView ()
  {}

  void
  MarkerTrajectoryView::updateMessage (int frameId, visualization_msgs::Marker& msg)
  {
    int offset = 1;
    if (frameId >= static_cast<int> (trajectory_.positions ().size ()))
      {
	std::cerr << "size mismatch (trajectory)" << std::endl;
	return;
      }
    if (trajectory_.positions ()[frameId].empty ())
      {
	std::cerr << "size mismatch (frame in trajectory)" << std::endl;
	return;
      }
    for (int i = 0; i < trajectory_.numMarkers (); ++i)
      {
	if (offset + i * 3 + 2
	    >= static_cast<int> (trajectory_.positions ()[frameId].size ()))
	  {
	    std::stringstream stream;
	    stream
	      << "size mismatch (while iterating, size is "
	      << trajectory_.positions ()[frameId].size ()
	      << " but we already are at "
	      << offset + i * 3 + 2 << ")";
	    std::cerr << stream.str () << std::endl;
	    continue;
	  }
	msg.points[i].x = trajectory_.positions ()[frameId][offset + i * 3 + 0];
	msg.points[i].y = trajectory_.positions ()[frameId][offset + i * 3 + 1];
	msg.points[i].z = trajectory_.positions ()[frameId][offset + i * 3 + 2];
      }
    ++msg.header.seq;
    msg.header.stamp = ros::Time::now ();
  }

} // end of namespace libmocap
