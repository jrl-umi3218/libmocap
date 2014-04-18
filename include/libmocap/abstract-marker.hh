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

#ifndef LIBMOCAP_ABSTRACT_MARKER_HH
# define LIBMOCAP_ABSTRACT_MARKER_HH
# include <iosfwd>
# include <string>

# include <libmocap/config.hh>
# include <libmocap/color.hh>
# include <libmocap/util.hh>

namespace libmocap
{
  class MarkerSet;
  class MarkerTrajectory;

  class LIBMOCAP_DLLEXPORT AbstractMarker
  {
  public:
    AbstractMarker ();
    AbstractMarker (const AbstractMarker&);
    virtual ~AbstractMarker ();
    AbstractMarker& operator= (const AbstractMarker& rhs);

    LIBMOCAP_ACCESSOR (id, int);
    LIBMOCAP_ACCESSOR (name, std::string);
    LIBMOCAP_ACCESSOR (color, Color);
    LIBMOCAP_ACCESSOR (physicalColor, Color);
    LIBMOCAP_ACCESSOR (size, double);
    LIBMOCAP_ACCESSOR (optional, bool);

    virtual std::ostream& print (std::ostream& o) const;

    virtual AbstractMarker* clone () const = 0;

    virtual void position
      (double position[3],
       const MarkerSet& markerSet,
       const MarkerTrajectory& trajectory,
       int frameId) const = 0;

  private:
    int id_;
    std::string name_;
    Color color_;
    Color physicalColor_;
    double size_;
    bool optional_;
  };

  LIBMOCAP_DLLEXPORT std::ostream&
  operator<< (std::ostream& o, const AbstractMarker& markerSet);

} // end of namespace libmocap.

#endif //! LIBMOCAP_ABSTRACT_MARKER_HH
