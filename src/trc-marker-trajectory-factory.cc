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
#include <cmath>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "trc-marker-trajectory-factory.hh"
#include "string.hh"

namespace libmocap
{
  struct TrcVariableMapper
  {
    const char* variable;
    void (TrcMarkerTrajectoryFactory::*loader)
    (MarkerTrajectory&, const std::string&);
  };

  static const TrcVariableMapper generalInformationVariableMapper[] = {
    {"DataRate", &TrcMarkerTrajectoryFactory::loadDataRate},
    {"CameraRate", &TrcMarkerTrajectoryFactory::loadCameraRate},
    {"NumFrames", &TrcMarkerTrajectoryFactory::loadNumFrames},
    {"NumMarkers", &TrcMarkerTrajectoryFactory::loadNumMarkers},
    {"Units", &TrcMarkerTrajectoryFactory::loadUnits},
    {"OrigDataRate", &TrcMarkerTrajectoryFactory::loadOrigDataRate},
    {"OrigDataStartFrame", &TrcMarkerTrajectoryFactory::loadOrigDataStartFrame},
    {"OrigNumFrames", &TrcMarkerTrajectoryFactory::loadOrigNumFrames},
    {0, 0}
  };


  TrcMarkerTrajectoryFactory::TrcMarkerTrajectoryFactory ()
  {
  }

  TrcMarkerTrajectoryFactory::~TrcMarkerTrajectoryFactory ()
  {
  }

  TrcMarkerTrajectoryFactory&
  TrcMarkerTrajectoryFactory::operator= (const TrcMarkerTrajectoryFactory& rhs)
  {
    if (this == &rhs)
      return *this;
    return *this;
  }

  MarkerTrajectory
  TrcMarkerTrajectoryFactory::load (const std::string& filename)
  {
    std::ifstream file (filename.c_str ());
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    MarkerTrajectory trajectory;

    loadHeader (file, trajectory);
    loadData (file, trajectory);

    return trajectory;
  }

  void
  TrcMarkerTrajectoryFactory::loadHeader (std::ifstream& file, MarkerTrajectory& trajectory)
  {
    std::string line;
    std::getline (file, line);
    trimEndOfLine (line);

    std::string tmp;

    // Load first line (prologue)
    {
      std::istringstream stream (line);

      if (!(stream >> tmp) && (tmp != "PathFileType"))
	throw std::runtime_error ("failed to read header");

      if (!(stream >> tmp) && (tmp != "4"))
	throw std::runtime_error ("failed to read header");

      if (!(stream >> tmp) && (tmp != "(X/Y/Z)"))
	throw std::runtime_error ("failed to read header");

      if (!(stream >> tmp))
	throw std::runtime_error ("failed to read header");
      trajectory.filename () = tmp;
    }

    // Load metadata labels
    std::vector<std::string> metaDataHeaders;

    std::getline (file, line);
    trimEndOfLine (line);

    {
      std::istringstream stream (line);
      while (stream >> tmp)
	metaDataHeaders.push_back (tmp);
    }

    // Load metadata values
    std::vector<std::string> metaDataValues;

    std::getline (file, line);
    trimEndOfLine (line);

    {
      std::istringstream stream (line);
      while (stream >> tmp)
	metaDataValues.push_back (tmp);
    }

    if (metaDataHeaders.size () != metaDataValues.size ())
      {
	std::cerr
	  << "warning: metadata header and value lines are inconsistent\n";
	metaDataValues.resize (metaDataHeaders.size ());
      }

    std::vector<std::string>::const_iterator itHeader;
    std::vector<std::string>::const_iterator itValue;
    const TrcVariableMapper* vMapper;
    for (itHeader = metaDataHeaders.begin (), itValue = metaDataValues.begin ();
	 itHeader != metaDataHeaders.end ();
	 ++itHeader, ++itValue)
      {
	vMapper = generalInformationVariableMapper;
	while (!!vMapper)
	  {
	    if (*itHeader == vMapper->variable)
	      {
		if (vMapper->loader)
		  (this->*(vMapper->loader)) (trajectory, *itValue);
		break;
	      }
	    vMapper++;
	  }
      }
  }

  static bool isBlank (char c)
  {
    return c == ' '
      || c == '\t'
      || c == '\r'
      || c == '\b';
  }

  void
  TrcMarkerTrajectoryFactory::loadData (std::ifstream& file, MarkerTrajectory& trajectory)
  {
    std::string line;
    int frameId;

    // Read columns
    std::getline (file, line);

    std::istringstream stream (line);
    std::string value;
    stream >> value;
    if (value != "Frame#")
      throw std::runtime_error ("failed to read columns titles");
    stream >> value;
    if (value != "Time")
      throw std::runtime_error ("failed to read columns titles");
    while (stream >> value)
      trajectory.markers ().push_back (value);

    // The other two lines are discarded as they contain no
    // interesting information.
    std::getline (file, line);
    std::getline (file, line);

    trajectory.positions ().clear ();
    trajectory.positions ().resize
      (static_cast<std::size_t> (trajectory.numFrames ()),
       std::vector<double>
       (1 + static_cast<std::size_t> (trajectory.numMarkers ()) * 3));

    while (!file.eof ())
      {
	try
	  {
	    std::getline (file, line);
	  }
	catch (const std::ifstream::failure& e)
	  {
	    // if eof is reached here, return, if this is another
	    // error, re-throw.
	    if (file.eof ())
	      return;
	    else
	      throw;
	  }
	trimEndOfLine (line);

	std::size_t start = 0;
	std::size_t end = 0;

	std::vector<std::string> data;
	while (start < line.size () && end < line.size ())
	  {
	    if (isBlank (line[start]))
	      start++;

	    // no more to read
	    if (start >= line.size ())
	      break;

	    end = start;
	    while (!isBlank (line[end]))
	      end++;
	    data.push_back (line.substr (start, end - start));
	    start = end;
	  }

	// skip empty lines
	if (data.empty ())
	  continue;

	frameId = convert<int> (data[0]) - 1;
	std::size_t frameId_ = static_cast<std::size_t> (frameId);

	if (frameId < 0
	    || frameId >= static_cast<int> (trajectory.positions ().size ()))
	  {
	    std::stringstream error;
	    error << "invalid frame id (number of frames is "
		  << trajectory.numFrames ()
		  << " but data size is "
		  << frameId
		  << ")";
	    throw std::runtime_error (error.str ());
	  }

	std::size_t markerId = 0;
	std::vector<std::string>::const_iterator it = data.begin ();
	for (++it; it != data.end (); ++it)
	  {
	    if (markerId >=
		1 + static_cast<std::size_t> (trajectory.numMarkers ()) * 3)
	      {
		std::stringstream stream;
		stream
		  << "size data mismatch, expected size is "
		  << 1 + trajectory.numMarkers () * 3
		  << ", but "
		  << markerId
		  << " have already been read";
		std::cerr << stream.str () << std::endl;
		continue;
	      }

	    // Missing marker, put NaN to signal it.
	    if (it->empty ())
	      {
		trajectory.positions ()[frameId_][markerId++] = nan ("");
	      }
	    else
	      trajectory.positions ()[frameId_][markerId++] = convert<double> (*it);
	  }
	if (markerId !=
	    1 + static_cast<std::size_t> (trajectory.numMarkers ()) * 3)
	  {
	    std::stringstream stream;
	    stream
	      << "size data mismatch, expected size is "
	      << 1 + trajectory.numMarkers () * 3
	      << ", but only "
	      << markerId
	      << " have been read";
	    std::cerr << stream.str () << std::endl;

	    while (markerId <
		   1 + static_cast<std::size_t> (trajectory.numMarkers ()) * 3)
	      trajectory.positions ()[frameId_][markerId++] = 0.;
	  }
      }
  }

  bool
  TrcMarkerTrajectoryFactory::canLoad (const std::string& filename)
  {
    std::string extension = extractExtension (filename);
    std::transform (extension.begin (),
		    extension.end(),
		    extension.begin(), ::tolower);
    return extension == "trc";
  }

  void
  TrcMarkerTrajectoryFactory::loadDataRate
  (MarkerTrajectory& trajectory, const std::string& value)
  {
    trajectory.dataRate () = convert<double> (value);
  }

  void
  TrcMarkerTrajectoryFactory::loadCameraRate
  (MarkerTrajectory& trajectory, const std::string& value)
  {
    trajectory.cameraRate () = convert<double> (value);
  }

  void
  TrcMarkerTrajectoryFactory::loadNumFrames
  (MarkerTrajectory& trajectory, const std::string& value)
  {
    trajectory.numFrames () = convert<int> (value);
  }

  void
  TrcMarkerTrajectoryFactory::loadNumMarkers
  (MarkerTrajectory& trajectory, const std::string& value)
  {
    trajectory.numMarkers () = convert<int> (value);
  }

  void
  TrcMarkerTrajectoryFactory::loadUnits (MarkerTrajectory& trajectory, const std::string& value)
  {
    trajectory.units () = value;
  }

  void
  TrcMarkerTrajectoryFactory::loadOrigDataRate
  (MarkerTrajectory& trajectory, const std::string& value)
  {
    trajectory.origDataRate () = convert<double> (value);
  }

  void
  TrcMarkerTrajectoryFactory::loadOrigDataStartFrame
  (MarkerTrajectory& trajectory, const std::string& value)
  {
    trajectory.origDataStartFrame () = convert<int> (value);
  }

  void
  TrcMarkerTrajectoryFactory::loadOrigNumFrames
  (MarkerTrajectory& trajectory, const std::string& value)
  {
    trajectory.origNumFrames () = convert<int> (value);
  }

} // end of namespace libmocap.
