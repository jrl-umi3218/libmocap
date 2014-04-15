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

#include "view.hh"

namespace libmocap
{
  View::View ()
    : msg_ ()
  {
    msg_.header.frame_id = "/world";
    msg_.header.stamp = ros::Time::now ();
    msg_.ns = "markers";
    msg_.id = 0;
    msg_.type = visualization_msgs::Marker::SPHERE_LIST;

    // Set the msg_ action.  Options are ADD and DELETE
    msg_.action = visualization_msgs::Marker::ADD;

    // Set the pose of the msg_.  This is a full 6DOF pose relative to the frame/time specified in the header
    msg_.pose.position.x = 0;
    msg_.pose.position.y = 0;
    msg_.pose.position.z = 0;
    msg_.pose.orientation.x = 0.0;
    msg_.pose.orientation.y = 0.0;
    msg_.pose.orientation.z = 0.0;
    msg_.pose.orientation.w = 1.0;

    // Set the scale of the msg_ -- 0.05x0.05x0.05 here means 5cm radius
    msg_.scale.x = 0.05;
    msg_.scale.y = 0.05;
    msg_.scale.z = 0.05;

    // Set the color -- be sure to set alpha to something non-zero!
    msg_.color.r = 0.0f;
    msg_.color.g = 1.0f;
    msg_.color.b = 0.0f;
    msg_.color.a = 1.0;

    msg_.lifetime = ros::Duration ();
  }

  View::~View ()
  {}

  const visualization_msgs::Marker&
  View::markerMessage (int frameId)
  {
    updateMessage (frameId, msg_);
    return msg_;
  }

} // end of namespace libmocap
