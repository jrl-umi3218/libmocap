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
#include <sstream>

#include <std_msgs/ColorRGBA.h>

#include "marker-set-segment-view.hh"

namespace libmocap
{
  MarkerSetSegmentView::MarkerSetSegmentView
  (const MarkerTrajectory& trajectory,
   const MarkerSet& markerSet,
   std::size_t segmentId)
    : View (),
      trajectory_ (trajectory),
      markerSet_ (markerSet),
      segmentId_ (segmentId)
  {
    if (segmentId >= markerSet.segments ().size ())
      throw std::runtime_error ("segment id too high");

    msg_.type = visualization_msgs::Marker::LINE_LIST;

    msg_.ns = "markers/segments/";
    if (markerSet.segments ()[segmentId].name ().empty ())
      {
	std::stringstream stream;
	stream << segmentId;
	msg_.ns += stream.str ();
      }
    else
      msg_.ns += markerSet.segments ()[segmentId].name ();

    msg_.id = 100 + static_cast<int> (segmentId);
    msg_.scale.x = 0.02;
    msg_.scale.y = 0.02;
    msg_.scale.z = 0.02;
    msg_.points.resize (3 * 2);

    msg_.colors.resize (3 * 2);

    msg_.colors[0].r = msg_.colors[1].r = 1.;
    msg_.colors[0].g = msg_.colors[1].g = 0.;
    msg_.colors[0].b = msg_.colors[1].b = 0.;
    msg_.colors[0].a = msg_.colors[1].a = 1.;

    msg_.colors[2].r = msg_.colors[3].r = 0.;
    msg_.colors[2].g = msg_.colors[3].g = 1.;
    msg_.colors[2].b = msg_.colors[3].b = 0.;
    msg_.colors[2].a = msg_.colors[3].a = 1.;

    msg_.colors[4].r = msg_.colors[5].r = 0.;
    msg_.colors[4].g = msg_.colors[5].g = 0.;
    msg_.colors[4].b = msg_.colors[5].b = 1.;
    msg_.colors[4].a = msg_.colors[5].a = 1.;
  }

  MarkerSetSegmentView::~MarkerSetSegmentView ()
  {}

  static void cross (double result[3], const double lhs[3], const double rhs[3])
  {
    result[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    result[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    result[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
  }

  static void dot_prod (double& result, const double lhs[3], const double rhs[3])
  {
    result = lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
  }

  static void proj (double result[3], const double lhs[3], const double rhs[3])
  {
    double cross_uu;
    double cross_uv;

    dot_prod (cross_uu, lhs, lhs);
    dot_prod (cross_uv, lhs, rhs);

    for (std::size_t i = 0; i < 3; ++i)
      result[i] = cross_uv / cross_uu * lhs[i];
  }

  void
  MarkerSetSegmentView::updateMessage (int frameId, visualization_msgs::Marker& msg)
  {
    msg_.action = visualization_msgs::Marker::ADD;

    std::size_t originMarkerId;
    std::size_t longAxisMarkerId;
    std::size_t planeAxisMarkerId;

    originMarkerId = static_cast<std::size_t>
      (markerSet_.segments ()[segmentId_].originMarker ());
    longAxisMarkerId = static_cast<std::size_t>
      (markerSet_.segments ()[segmentId_].longAxisMarker ());
    planeAxisMarkerId = static_cast<std::size_t>
      (markerSet_.segments ()[segmentId_].planeAxisMarker ());

    double originMarkerPos[3];
    double longAxisMarkerPos[3];
    double planeAxisMarkerPos[3];

    markerSet_.markers ()[originMarkerId]->position
      (originMarkerPos, markerSet_, trajectory_, frameId);
    markerSet_.markers ()[longAxisMarkerId]->position
      (longAxisMarkerPos, markerSet_, trajectory_, frameId);
    markerSet_.markers ()[planeAxisMarkerId]->position
      (planeAxisMarkerPos, markerSet_, trajectory_, frameId);

    for (std::size_t i = 0; i < 3; ++i)
      if (std::isnan (originMarkerPos[i])
	  || std::isnan (longAxisMarkerPos[i])
	  || std::isnan (planeAxisMarkerPos[i]))
	{
	  msg_.action = visualization_msgs::Marker::DELETE;
	  return;
	}

    double ox[3];
    double oy[3];
    double oz[3];

    for (std::size_t i = 0; i < 3; ++i)
      {
	ox[i] = longAxisMarkerPos[i] - originMarkerPos[i];
	oy[i] = planeAxisMarkerPos[i] - originMarkerPos[i];
      }

    double proj_ox_oy[3];
    proj (proj_ox_oy, ox, oy);
    for (std::size_t i = 0; i < 3; ++i)
      oy[i] -= proj_ox_oy[i];

    cross (oz, ox, oy);


    // draw ox
    msg_.points[0].x = originMarkerPos[0];
    msg_.points[0].y = originMarkerPos[1];
    msg_.points[0].z = originMarkerPos[2];

    msg_.points[1].x = originMarkerPos[0] + ox[0];
    msg_.points[1].y = originMarkerPos[1] + ox[1];
    msg_.points[1].z = originMarkerPos[2] + ox[2];
    // draw oy
    msg_.points[2 + 0].x = originMarkerPos[0];
    msg_.points[2 + 0].y = originMarkerPos[1];
    msg_.points[2 + 0].z = originMarkerPos[2];

    msg_.points[2 + 1].x = originMarkerPos[0] + oy[0];
    msg_.points[2 + 1].y = originMarkerPos[1] + oy[1];
    msg_.points[2 + 1].z = originMarkerPos[2] + oy[2];

    // draw oz
    msg_.points[4 + 0].x = originMarkerPos[0];
    msg_.points[4 + 0].y = originMarkerPos[1];
    msg_.points[4 + 0].z = originMarkerPos[2];

    msg_.points[4 + 1].x = originMarkerPos[0] + oz[0];
    msg_.points[4 + 1].y = originMarkerPos[1] + oz[1];
    msg_.points[4 + 1].z = originMarkerPos[2] + oz[2];

    ++msg.header.seq;
    msg.header.stamp = ros::Time::now ();
  }

} // end of namespace libmocap
