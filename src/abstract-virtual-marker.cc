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
#include <libmocap/abstract-virtual-marker.hh>

namespace libmocap
{
  AbstractVirtualMarker::AbstractVirtualMarker ()
    : AbstractMarker (),
      originMarker_ (),
      longAxisMarker_ (),
      planeAxisMarker_ ()
  {
  }

  AbstractVirtualMarker::AbstractVirtualMarker
  (const AbstractVirtualMarker& rhs)
    : AbstractMarker (rhs),
      originMarker_ (rhs.originMarker_),
      longAxisMarker_ (rhs.longAxisMarker_),
      planeAxisMarker_ (rhs.planeAxisMarker_)
  {
  }

  AbstractVirtualMarker::~AbstractVirtualMarker ()
  {}

  AbstractVirtualMarker&
  AbstractVirtualMarker::operator= (const AbstractVirtualMarker& rhs)
  {
    if (this == &rhs)
      return *this;
    AbstractMarker::operator= (rhs);
    originMarker_ = rhs.originMarker_;
    longAxisMarker_ = rhs.longAxisMarker_;
    planeAxisMarker_ = rhs.planeAxisMarker_;
    return *this;
  }

  std::ostream&
  AbstractVirtualMarker::print (std::ostream& stream) const
  {
    AbstractMarker::print (stream);
    stream
      << "origin marker: " << originMarker_ << '\n'
      << "long axis marker: " << longAxisMarker_ << '\n'
      << "plane axis marker: " << planeAxisMarker_ << '\n';
    return stream;
  }

  std::ostream&
  operator<< (std::ostream& o, const AbstractVirtualMarker& markerSet)
  {
    return markerSet.print (o);
  }

} // end of namespace libmocap.
