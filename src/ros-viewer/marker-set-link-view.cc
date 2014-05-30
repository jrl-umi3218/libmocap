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

#include "marker-set-link-view.hh"

namespace libmocap
{
  MarkerSetLinkView::MarkerSetLinkView
  (const MarkerTrajectory& trajectory,
   const MarkerSet& markerSet)
    : View (),
      trajectory_ (trajectory),
      markerSet_ (markerSet)
  {
    msg_.type = visualization_msgs::Marker::LINE_LIST;
    msg_.ns = "markers/links";
    msg_.id = 1;
    msg_.scale.x = 0.02;
    msg_.scale.y = 0.02;
    msg_.scale.z = 0.02;
    msg_.points.resize (markerSet.links ().size () * 2);
    msg_.colors.resize (markerSet.links ().size () * 2);
  }

  MarkerSetLinkView::~MarkerSetLinkView ()
  {}

  void
  MarkerSetLinkView::updateMessage (int frameId, visualization_msgs::Marker& msg)
  {
    std::size_t marker1;
    std::size_t marker2;
    std::size_t missingData = 0;

    std::size_t frameId_ = static_cast<std::size_t> (frameId);

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

    msg.points.resize (markerSet_.links ().size () * 2);
    msg.colors.resize (markerSet_.links ().size () * 2);
    for (std::size_t i = 0; i < markerSet_.links ().size (); ++i)
      {
	marker1 = static_cast<std::size_t> (markerSet_.links ()[i].marker1 ());
	marker2 = static_cast<std::size_t> (markerSet_.links ()[i].marker2 ());

	if (marker1 >= markerSet_.markers ().size ()
	    || marker2 >= markerSet_.markers ().size ())
	  {
	    std::stringstream stream;
	    stream
	      << "size mismatch (while iterating, marker1 = "
	      << marker1
	      << " or marker 2 = " << marker2
	      << " is too large)";
	    std::cerr << stream.str () << std::endl;
	    continue;
	  }

	double position1[3];
	double position2[3];
	markerSet_.markers ()[marker1]->position
	  (position1, markerSet_, trajectory_, frameId);
	markerSet_.markers ()[marker2]->position
	  (position2, markerSet_, trajectory_, frameId);

	std::size_t id = (i - missingData) * 2;
	msg.points[id].x = position1[0];
	msg.points[id].y = position1[1];
	msg.points[id].z = position1[2];

	msg.points[id + 1].x = position2[0];
	msg.points[id + 1].y = position2[1];
	msg.points[id + 1].z = position2[2];

	msg.colors[id].r =
	  static_cast<float> (markerSet_.links ()[i].color ().red () / 256.);
	msg.colors[id].g =
	  static_cast<float> (markerSet_.links ()[i].color ().green () / 256.);
	msg.colors[id].b =
	  static_cast<float> (markerSet_.links ()[i].color ().blue () / 256.);
	msg.colors[id].a = 1.;
	msg.colors[id + 1].r = msg.colors[id].r;
	msg.colors[id + 1].g = msg.colors[id].g;
	msg.colors[id + 1].b = msg.colors[id].b;
	msg.colors[id + 1].a = msg.colors[id].a;

	if (std::isnan (msg.points[id].x)
	    || std::isnan (msg.points[id].y)
	    || std::isnan (msg.points[id].z)
	    || std::isnan (msg.points[id + 1].x)
	    || std::isnan (msg.points[id + 1].y)
	    || std::isnan (msg.points[id + 1].z))
	  {
	    msg.points[id].x = msg.points[id].y = msg.points[id].z = 0.;
	    msg.points[id + 1].x = msg.points[id + 1].y = msg.points[id + 1].z = 0.;
	    missingData++;
	  }
      }
    msg.points.resize ((markerSet_.links ().size () - missingData) * 2);
    msg.colors.resize ((markerSet_.links ().size () - missingData) * 2);

    ++msg.header.seq;
    msg.header.stamp = ros::Time::now ();
  }

} // end of namespace libmocap
