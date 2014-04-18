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
#include <libmocap/abstract-marker.hh>
#include <libmocap/marker.hh>
#include <libmocap/marker-trajectory.hh>

namespace libmocap
{
  Marker::Marker ()
    : AbstractMarker ()
  {}

  Marker::Marker (const Marker& rhs)
    : AbstractMarker (rhs)
  {}

  Marker::~Marker ()
  {}

  Marker&
  Marker::operator= (const Marker& rhs)
  {
    if (this == &rhs)
      return *this;
    AbstractMarker::operator= (rhs);
    return *this;
  }

  std::ostream&
  Marker::print (std::ostream& stream) const
  {
    AbstractMarker::print (stream);
    return stream;
  }

  AbstractMarker*
  Marker::clone () const
  {
    return new Marker (*this);
  }

  void
  Marker::position
  (double position[3],
   const MarkerSet&,
   const MarkerTrajectory& trajectory,
   int frameId) const
  {
    if (frameId < 0)
      throw std::runtime_error ("negative frame id");
    if (frameId >= static_cast<int> (trajectory.positions ().size ()))
      throw std::runtime_error ("frame id is too large");
    if (id () < 0
	|| id () >= static_cast<int> (trajectory.positions ()[frameId].size ()))
      throw std::runtime_error ("marker id is inconsistent");

    position[0] = trajectory.positions ()[frameId][1 + id () * 3 + 0];
    position[1] = trajectory.positions ()[frameId][1 + id () * 3 + 1];
    position[2] = trajectory.positions ()[frameId][1 + id () * 3 + 2];
  }

  std::ostream&
  operator<< (std::ostream& o, const Marker& marker)
  {
    return marker.print (o);
  }

} // end of namespace libmocap.
