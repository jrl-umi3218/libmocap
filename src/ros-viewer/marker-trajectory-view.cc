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

#include <std_msgs/ColorRGBA.h>

#include "marker-trajectory-view.hh"

namespace libmocap
{
  MarkerTrajectoryView::MarkerTrajectoryView
  (const MarkerTrajectory& trajectory,
   const MarkerSet& markerSet)
    : View (),
      trajectory_ (trajectory),
      markerSet_ (markerSet)
  {
    msg_.points.resize (markerSet.markers ().size ());
    msg_.colors.resize (markerSet.markers ().size ());
  }

  MarkerTrajectoryView::~MarkerTrajectoryView ()
  {}

  void
  MarkerTrajectoryView::updateMessage (int frameId, visualization_msgs::Marker& msg)
  {
    std::size_t frameId_ =static_cast<std::size_t> (frameId);

    msg_.action = visualization_msgs::Marker::ADD;

    if (frameId >= static_cast<int> (trajectory_.positions ().size ()))
      {
	std::cerr << "size mismatch (trajectory)" << std::endl;
	return;
      }
    if (trajectory_.positions ()[frameId_].empty ())
      {
	std::cerr << "size mismatch (frame in trajectory)" << std::endl;
	return;
      }

    std::size_t missingData = 0;
    msg.points.resize (markerSet_.markers ().size ());
    msg.colors.resize (markerSet_.markers ().size ());
    for (std::size_t i = 0; i < markerSet_.markers ().size (); ++i)
      {
	if (!markerSet_.markers ()[i])
	  {
	    std::cerr << "null marker in marker set" << std::endl;
	    continue;
	  }

	const AbstractMarker& marker = *markerSet_.markers ()[i];
	double position[3];
	marker.position (position, markerSet_, trajectory_, frameId);

	std::size_t id = i - missingData;
	msg.points[id].x = position[0];
	msg.points[id].y = position[1];
	msg.points[id].z = position[2];

	msg.colors[id].r = static_cast<float> (marker.color ().red () / 256.);
	msg.colors[id].g = static_cast<float> (marker.color ().green () / 256.);
	msg.colors[id].b = static_cast<float> (marker.color ().blue () / 256.);
	msg.colors[id].a = 1.;

	if (std::isnan (msg.points[id].x)
	    || std::isnan (msg.points[id].y)
	    || std::isnan (msg.points[id].z))
	  {
	    msg.points[id].x = msg.points[id].y = msg.points[id].z = 0.;
	    missingData++;
	  }
      }

    msg.points.resize (markerSet_.markers ().size () - missingData);
    msg.colors.resize (markerSet_.markers ().size () - missingData);

    ++msg.header.seq;
    msg.header.stamp = ros::Time::now ();
  }

} // end of namespace libmocap
