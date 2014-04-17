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

#include "marker-set-name-view.hh"

namespace libmocap
{
  MarkerSetNameView::MarkerSetNameView
  (const MarkerTrajectory& trajectory,
   const MarkerSet& markerSet,
   int id)
    : View (),
      trajectory_ (trajectory),
      markerSet_ (markerSet),
      id_ (id)
  {
    if (id >= static_cast<int> (markerSet_.markers ().size ()))
      throw std::runtime_error ("invalid id");

    msg_.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
    msg_.ns = "markers/names";
    msg_.id = 10 + id;
    msg_.scale.z = 0.05;
    if (markerSet_.markers ()[id])
      msg_.text = markerSet_.markers ()[id]->name ();
    else
      msg_.text = "<unnamed>";
  }

  MarkerSetNameView::~MarkerSetNameView ()
  {}

  void
  MarkerSetNameView::updateMessage (int frameId, visualization_msgs::Marker& msg)
  {
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

    if (id_ * 3  + 2 >= static_cast<int> (trajectory_.positions ()[frameId].size ()))
      {
	std::stringstream stream;
	stream
	  << "size mismatch (while iterating, id = "
	  << id_
	  << " is too large)";
	std::cerr << stream.str () << std::endl;
	return;
      }

    msg.pose.position.x = trajectory_.positions ()[frameId][1 + id_ * 3 + 0] + .5 * msg_.scale.z;
    msg.pose.position.y = trajectory_.positions ()[frameId][1 + id_ * 3 + 1] + .5 * msg_.scale.z;
    msg.pose.position.z = trajectory_.positions ()[frameId][1 + id_ * 3 + 2] + .5 * msg_.scale.z;
    ++msg.header.seq;
    msg.header.stamp = ros::Time::now ();
  }

} // end of namespace libmocap
