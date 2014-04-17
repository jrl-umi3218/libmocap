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
    std::string markerLabel;
    for (int i = 0; i < static_cast<int> (markerSet.links ().size ()); ++i)
      {
	msg_.colors[i * 2].r = static_cast<float> (markerSet.links ()[i].color ().red () / 256.);
	msg_.colors[i * 2].g = static_cast<float> (markerSet.links ()[i].color ().green () / 256.);
	msg_.colors[i * 2].b = static_cast<float> (markerSet.links ()[i].color ().blue () / 256.);
	msg_.colors[i * 2].a = 1.;

	msg_.colors[i * 2 + 1].r = msg_.colors[i * 2].r;
	msg_.colors[i * 2 + 1].g = msg_.colors[i * 2].g;
	msg_.colors[i * 2 + 1].b = msg_.colors[i * 2].b;
	msg_.colors[i * 2 + 1].a = msg_.colors[i * 2].a;
      }
  }

  MarkerSetLinkView::~MarkerSetLinkView ()
  {}

  void
  MarkerSetLinkView::updateMessage (int frameId, visualization_msgs::Marker& msg)
  {
    int marker1;
    int marker2;
    int missingData = 0;

    msg_.action = visualization_msgs::Marker::ADD;

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

    msg.points.resize (markerSet_.links ().size () * 2);
    for (int i = 0; i < static_cast<int> (markerSet_.links ().size ()); ++i)
      {
	marker1 = markerSet_.links ()[i].marker1 ();
	marker2 = markerSet_.links ()[i].marker2 ();

	if (marker1 * 3  + 2 >= static_cast<int> (trajectory_.positions ()[frameId].size ())
	    || marker2 * 3 + 2 >= static_cast<int> (trajectory_.positions ()[frameId].size ()))
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

	int id = (i - missingData) * 2;
	msg.points[id].x = trajectory_.positions ()[frameId][1 + marker1 * 3 + 0];
	msg.points[id].y = trajectory_.positions ()[frameId][1 + marker1 * 3 + 1];
	msg.points[id].z = trajectory_.positions ()[frameId][1 + marker1 * 3 + 2];

	msg.points[id + 1].x = trajectory_.positions ()[frameId][1 + marker2 * 3 + 0];
	msg.points[id + 1].y = trajectory_.positions ()[frameId][1 + marker2 * 3 + 1];
	msg.points[id + 1].z = trajectory_.positions ()[frameId][1 + marker2 * 3 + 2];

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

    ++msg.header.seq;
    msg.header.stamp = ros::Time::now ();
  }

} // end of namespace libmocap
