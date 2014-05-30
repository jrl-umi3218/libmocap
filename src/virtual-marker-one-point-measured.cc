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
#include <libmocap/virtual-marker-one-point-measured.hh>

namespace libmocap
{
  VirtualMarkerOnePointMeasured::VirtualMarkerOnePointMeasured
  (const double& offsetX,
   const double& offsetY,
   const double& offsetZ)
    : AbstractVirtualMarker (),
      offset_ (3)
  {
    offset_[0] = offsetX;
    offset_[1] = offsetY;
    offset_[2] = offsetZ;
  }

  VirtualMarkerOnePointMeasured::VirtualMarkerOnePointMeasured
  (const VirtualMarkerOnePointMeasured& rhs)
    : AbstractVirtualMarker (),
      offset_ (rhs.offset_)
  {}

  VirtualMarkerOnePointMeasured::~VirtualMarkerOnePointMeasured ()
  {}

  VirtualMarkerOnePointMeasured&
  VirtualMarkerOnePointMeasured::operator= (const VirtualMarkerOnePointMeasured& rhs)
  {
    if (&rhs == this)
      return *this;
    AbstractVirtualMarker::operator= (rhs);
    offset_ = rhs.offset_;
    return *this;
  }

  std::ostream&
  VirtualMarkerOnePointMeasured::print (std::ostream& stream) const
  {
    AbstractVirtualMarker::print (stream);
    return stream;
  }

  AbstractMarker*
  VirtualMarkerOnePointMeasured::clone () const
  {
    return new VirtualMarkerOnePointMeasured (*this);
  }

  void
  VirtualMarkerOnePointMeasured::position
  (double position[3], const MarkerSet& markerSet, const MarkerTrajectory& trajectory, int frameId) const
  {
    if (frameId < 0)
      throw std::runtime_error ("negative frame id");
    if (frameId >= static_cast<int> (trajectory.positions ().size ()))
      throw std::runtime_error ("frame id is too large");
    if (originMarker () < 0)
      throw std::runtime_error ("negative origin marker");
    if (originMarker () >= static_cast<int> (markerSet.markers ().size ()))
      throw std::runtime_error ("origin marker id too large");

    markerSet.markers ()[static_cast<std::size_t> (originMarker ())]->position
      (position, markerSet, trajectory, frameId);
    position[0] += offset ()[0];
    position[1] += offset ()[1];
    position[2] += offset ()[2];
  }

  std::ostream&
  operator<< (std::ostream& o, const VirtualMarkerOnePointMeasured& markerSet)
  {
    return markerSet.print (o);
  }

} // end of namespace libmocap.
