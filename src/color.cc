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
#include <cstdlib>
#include <limits>
#include <iostream>
#include <libmocap/color.hh>

namespace libmocap
{
  uint8_t Color::red () const
  {
    return static_cast<uint8_t> ((data_ & 0xFF000000) >> 24);
  }

  uint8_t Color::green () const
  {
    return static_cast<uint8_t> ((data_ & 0x00FF0000) >> 16);
  }

  uint8_t Color::blue () const
  {
    return static_cast<uint8_t> ((data_ & 0x0000FF00) >> 8);
  }

  uint8_t Color::alpha () const
  {
    return static_cast<uint8_t> (data_ & 0x000000FF);
  }


  std::ostream&
  Color::print (std::ostream& stream) const
  {
    stream
      << "r: " << static_cast<unsigned int> (red ()) << ", "
      << "g: " << static_cast<unsigned int> (green ()) << ", "
      << "b: " << static_cast<unsigned int> (blue ()) << ", "
      << "a: " << static_cast<unsigned int> (alpha ());
    return stream;
  }

  Color randomizeColorRGB ()
  {
    Color color;
    color.data () = rand () % std::numeric_limits<uint32_t>::max ();
    color.data () |= 0x000000FF;
    return color;
  }

  std::ostream&
  operator<< (std::ostream& o, const Color& color)
  {
    return color.print (o);
  }

} // end of namespace libmocap.
