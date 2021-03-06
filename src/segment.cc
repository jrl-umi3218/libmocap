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
#include <libmocap/segment.hh>

namespace libmocap
{
  Segment::Segment ()
    : id_ (),
      name_ (),
      children_ (),
      originMarker_ (),
      longAxisMarker_ (),
      planeAxisMarker_ (),
      rotationOffset_ ()
  {}

  Segment::Segment (const Segment& rhs)
    : id_ (rhs.id_),
      name_ (rhs.name_),
      children_ (rhs.children_),
      originMarker_ (rhs.originMarker_),
      longAxisMarker_ (rhs.longAxisMarker_),
      planeAxisMarker_ (rhs.planeAxisMarker_),
      rotationOffset_ (rhs.rotationOffset_)
  {}

  Segment::~Segment ()
  {}

  Segment&
  Segment::operator= (const Segment& rhs)
  {
    if (this == &rhs)
      return *this;
    id_ = rhs.id_;
    name_ = rhs.name_;
    children_ = rhs.children_;
    originMarker_ = rhs.originMarker_;
    longAxisMarker_ = rhs.longAxisMarker_;
    planeAxisMarker_ = rhs.planeAxisMarker_;
    rotationOffset_ = rhs.rotationOffset_;
    return *this;
  }

  std::ostream&
  Segment::print (std::ostream& stream) const
  {
    stream
      << "segment:\n"
      << "id: " << id () << '\n'
      << "name: " << name () << '\n'
      << "name: " << name () << '\n'
      << "number of children: " << children ().size () << '\n'
      << "origin marker: " << originMarker () << '\n'
      << "long axis marker: " << longAxisMarker () << '\n'
      << "plane axis marker: " << planeAxisMarker () << '\n'
      << "rotation offset: "
      << rotationOffset ().roll () << " / "
      << rotationOffset ().pitch () << " / "
      << rotationOffset ().yaw ();
    return stream;
  }

  std::ostream&
  operator<< (std::ostream& o, const Segment& markerSet)
  {
    return markerSet.print (o);
  }

} // end of namespace libmocap.
