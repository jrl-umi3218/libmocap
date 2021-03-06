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
#include <libmocap/link.hh>

namespace libmocap
{
  Link::Link ()
    : name_ (),
      color_ (),
      type_ (),
      marker1_ (),
      marker2_ (),
      minLength_ (),
      maxLength_ (),
      extraStretch_ ()
  {}

  Link::Link (const Link& rhs)
    : name_ (rhs.name_),
      color_ (rhs.color_),
      type_ (rhs.type_),
      marker1_ (rhs.marker1_),
      marker2_ (rhs.marker2_),
      minLength_ (rhs.minLength_),
      maxLength_ (rhs.maxLength_),
      extraStretch_ (rhs.extraStretch_)
  {}

  Link::~Link ()
  {}

  Link& Link::operator= (const Link& rhs)
  {
    if (&rhs == this)
      return *this;
    name_ = rhs.name_;
    color_ = rhs.color_;
    type_ = rhs.type_;
    marker1_ = rhs.marker1_;
    marker2_ = rhs.marker2_;
    minLength_ = rhs.minLength_;
    maxLength_ = rhs.maxLength_;
    extraStretch_ = rhs.extraStretch_;
    return *this;
  }

  std::ostream&
  Link::print (std::ostream& stream) const
  {
    stream
      << "link:\n"
      << "name: " << name () << '\n'
      << "color: " << color () << '\n'
      << "type: " << type () << '\n'
      << "marker 1: " << marker1 () << '\n'
      << "marker 2: " << marker2 () << '\n'
      << "min length: " << minLength () << '\n'
      << "max length: " << maxLength () << '\n'
      << "extra stretch: " << extraStretch ();
    return stream;
  }

  std::ostream&
  operator<< (std::ostream& o, const Link& markerSet)
  {
    return markerSet.print (o);
  }

} // end of namespace libmocap.
