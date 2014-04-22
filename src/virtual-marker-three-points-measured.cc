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
#include <stdexcept>

#include <libmocap/abstract-virtual-marker.hh>
#include <libmocap/marker-trajectory.hh>
#include <libmocap/marker-set.hh>
#include <libmocap/virtual-marker-three-points-measured.hh>

#include "math.hh"

namespace libmocap
{
  VirtualMarkerThreePointsMeasured::VirtualMarkerThreePointsMeasured
  (const double& offsetX, const double& offsetY, const double& offsetZ)
    : AbstractVirtualMarker (),
      offset_ (3)
  {
    offset_[0] = offsetX;
    offset_[1] = offsetY;
    offset_[2] = offsetZ;
  }

  VirtualMarkerThreePointsMeasured::VirtualMarkerThreePointsMeasured
  (const VirtualMarkerThreePointsMeasured& rhs)
    : AbstractVirtualMarker (),
      offset_ (rhs.offset_)
  {}

  VirtualMarkerThreePointsMeasured::~VirtualMarkerThreePointsMeasured ()
  {}

  VirtualMarkerThreePointsMeasured&
  VirtualMarkerThreePointsMeasured::operator= (const VirtualMarkerThreePointsMeasured& rhs)
  {
    if (&rhs == this)
      return *this;
    AbstractVirtualMarker::operator= (rhs);
    offset_ = rhs.offset_;
    return *this;
  }

  std::ostream&
  VirtualMarkerThreePointsMeasured::print (std::ostream& stream) const
  {
    AbstractVirtualMarker::print (stream);
    return stream;
  }

  AbstractMarker*
  VirtualMarkerThreePointsMeasured::clone () const
  {
    return new VirtualMarkerThreePointsMeasured (*this);
  }

  void
  VirtualMarkerThreePointsMeasured::position
  (double position[3], const MarkerSet& markerSet,
   const MarkerTrajectory& trajectory, int frameId) const
  {
    if (frameId < 0)
      throw std::runtime_error ("negative frame id");
    if (frameId >= static_cast<int> (trajectory.positions ().size ()))
      throw std::runtime_error ("frame id is too large");
    if (offset ().size () != 3)
      throw std::runtime_error ("offset vector too large");
    if (originMarker () < 0)
      throw std::runtime_error ("negative origin marker");
    if (originMarker () >= static_cast<int> (markerSet.markers ().size ()))
      throw std::runtime_error ("origin marker id too large");
    if (!markerSet.markers ()[originMarker ()])
      throw std::runtime_error ("null long axis marker");
    if (longAxisMarker () < 0)
      throw std::runtime_error ("negative long axis marker");
    if (longAxisMarker () >= static_cast<int> (markerSet.markers ().size ()))
      throw std::runtime_error ("long axis marker id too large");
    if (!markerSet.markers ()[longAxisMarker ()])
      throw std::runtime_error ("null long axis marker");
    if (planeAxisMarker () < 0)
      throw std::runtime_error ("negative plane axis marker");
    if (planeAxisMarker () >= static_cast<int> (markerSet.markers ().size ()))
      throw std::runtime_error ("plane axis marker id too large");
    if (!markerSet.markers ()[planeAxisMarker ()])
      throw std::runtime_error ("null plane axis axis marker");

    double originPos[3];
    double longAxisMarkerPos[3];
    double planeAxisMarkerPos[3];

    markerSet.markers ()[originMarker ()]->position
      (originPos, markerSet, trajectory, frameId);
    markerSet.markers ()[longAxisMarker ()]->position
      (longAxisMarkerPos, markerSet, trajectory, frameId);
    markerSet.markers ()[planeAxisMarker ()]->position
      (longAxisMarkerPos, markerSet, trajectory, frameId);


    double ox[3];
    double oy[3];
    double oz[3];

    for (std::size_t i = 0; i < 3; ++i)
      ox[i] = longAxisMarkerPos[i] - originPos[i];
    for (std::size_t i = 0; i < 3; ++i)
      oy[i] = planeAxisMarkerPos[i] - originPos[i];
    cross (oz, ox, oy);

    double proj_ox_oy[3];
    double proj_ox_oz[3];
    double proj_oy_oz[3];

    proj (proj_ox_oy, ox, oy);
    proj (proj_ox_oz, ox, oz);
    proj (proj_oy_oz, oy, oz);

    normalize (ox);
    normalize (oy);
    normalize (oz);

    for (std::size_t i = 0; i < 3; ++i)
      {
	oy[i] = oy[i] - proj_ox_oy[i];
	oz[i] = oz[i] - proj_ox_oz[i] - proj_oy_oz[i];
      }


    for (std::size_t i = 0; i < 3; ++i)
      position[i] =
	originPos[i]
	+ offset_[0] * ox[i]
	+ offset_[1] * oy[i]
	+ offset_[2] * oz[i];
  }

  std::ostream&
  operator<< (std::ostream& o, const VirtualMarkerThreePointsMeasured& markerSet)
  {
    return markerSet.print (o);
  }

} // end of namespace libmocap.
