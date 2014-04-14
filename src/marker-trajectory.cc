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
#include <iterator>
#include <stdexcept>
#include <libmocap/marker-trajectory.hh>

namespace libmocap
{
  MarkerTrajectory::MarkerTrajectory ()
    : filename_ (),
      dataRate_ (),
      cameraRate_ (),
      numFrames_ (),
      numMarkers_ (),
      units_ (),
      origDataRate_ (),
      origDataStartFrame_ (),
      origNumFrames_ (),
      markers_ (),
      positions_ ()
  {
  }

  MarkerTrajectory::~MarkerTrajectory ()
  {
  }

  MarkerTrajectory&
  MarkerTrajectory::operator= (const MarkerTrajectory& rhs)
  {
    if (this == &rhs)
      return *this;
    return *this;
  }

  void
  MarkerTrajectory::normalize ()
  {
    double scalingFactor = 1.;
    if (units () == "m")
      return;
    else if (units () == "mm")
      scalingFactor = 1e-3;
    else
      throw std::runtime_error ("unit not supported");

    std::vector<std::vector<double> >::iterator itFrame;
    std::vector<double>::iterator itValue;
    for (itFrame = positions ().begin ();
	 itFrame != positions ().end (); ++itFrame)
      {
	// pass first elements which does not have to be scaled (time)
	itValue = itFrame->begin ();
	if (itValue == itFrame->end ())
	  continue;
	for (++itValue; itValue != itFrame->end (); ++itValue)
	  *itValue = *itValue * scalingFactor;
      }

    units () = "m";
  }

  std::ostream&
  MarkerTrajectory::print (std::ostream& o) const
  {
    o << "market trajectory:\n"
      << "filename: " << filename () << '\n'
      << "data rate: " << dataRate () << '\n'
      << "camera rate: " << cameraRate () << '\n'
      << "num frames: " << numFrames () << '\n'
      << "num markers: " << numMarkers () << '\n'
      << "units: " << units () << '\n'
      << "origDataRate: " << origDataRate () << '\n'
      << "origDataStartFrame: " << origDataStartFrame () << '\n'
      << "origNumFrames: " << origNumFrames () << '\n'
      << "markers: ";
    std::copy
      (markers ().begin (), markers ().end (),
       std::ostream_iterator<std::string>(o, "\n"));
    o << '\n'
      << "positions: \n";
    for (std::size_t frame = 0; frame < positions ().size (); ++frame)
      {
	if (positions ()[frame].empty ())
	  o << "(empty vector)";
	else
	  std::copy
	    (positions ()[frame].begin (), positions ()[frame].end (),
	     std::ostream_iterator<double>(o, ", "));
	o << '\n';
      }
    return o;
  }

  LIBMOCAP_DLLEXPORT std::ostream&
  operator<< (std::ostream& o, const MarkerTrajectory& trajectory)
  {
    return trajectory.print (o);
  }
} // end of namespace libmocap.
