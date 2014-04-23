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
#include <cassert>
#include <cmath>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <libmocap/abstract-virtual-marker.hh>
#include <libmocap/link.hh>
#include <libmocap/marker.hh>
#include <libmocap/virtual-marker-one-point-measured.hh>
#include <libmocap/virtual-marker-three-points-measured.hh>
#include <libmocap/virtual-marker-two-points-measured.hh>
#include <libmocap/virtual-marker-two-points-ratio.hh>
#include <libmocap/virtual-marker-three-points-ratio.hh>

#include "mars-marker-set-factory.hh"
#include "string.hh"

// Please note that eof is checked before and after peek as peek will
// fail is eof is already reached but may also change the stream
// status.
//
// In this case, the following reading attempt will throw an
// exception.

namespace libmocap
{
  struct SectionMapper
  {
    const char* section;
    void (MarsMarkerSetFactory::*loader) (MarkerSet&, std::ifstream&);
  };

  static const SectionMapper sectionMapper[] = {
    {"General Information", &MarsMarkerSetFactory::loadGeneralInformation},
    {"Markers", &MarsMarkerSetFactory::loadMarkers},
    {"VirtualMarkers", &MarsMarkerSetFactory::loadVirtualMarkers},
    {"VMJoinDefs", &MarsMarkerSetFactory::loadVMJoinDefs},
    {"Linkages", &MarsMarkerSetFactory::loadLinkages},
    {"SkeletonType", &MarsMarkerSetFactory::loadSkeletonType},
    {"CalciumModel", &MarsMarkerSetFactory::loadCalciumModel},
    {"HtrExportOptions", &MarsMarkerSetFactory::loadHtrExportOptions},
    {"Segments", &MarsMarkerSetFactory::loadSegments},
    {"ModelPose", &MarsMarkerSetFactory::loadModelPose},
    {"Personal Info", &MarsMarkerSetFactory::loadPersonalInfo},
    {"Mass Model", &MarsMarkerSetFactory::loadMassModel},
    {0, 0},
  };

  struct VariableMapper
  {
    const char* variable;
    void (MarsMarkerSetFactory::*loader) (MarkerSet&, const std::string&);
  };

  static const VariableMapper generalInformationVariableMapper[] = {
    {"Version",  &MarsMarkerSetFactory::loadVersion},
    {"ProjectName",  &MarsMarkerSetFactory::loadProjectName},
    {"ProjectComments1",  0},
    {"ProjectComments2",  0},
    {"ProjectComments3",  0},
    {0, 0},
  };

  static const VariableMapper markersVariableMapper[] = {
    {"Comment", 0},
    {"NumberOf", 0},
    {0, 0},
  };

  static const VariableMapper virtualMarkersVariableMapper[] = {
    {"Comment", 0},
    {"NumberOf", 0},
    {0, 0},
  };

  static const VariableMapper VMJoinDefsVariableMapper[] = {
    {"Comment", 0},
    {"NumberOf", 0},
    {0, 0},
  };

  static const VariableMapper linkagesVariableMapper[] = {
    {"Comment", 0},
    {"NumberOf", 0},
    {0, 0},
  };

  static const VariableMapper skeletonTypeVariableMapper[] = {
    {"Comment", 0},
    {"SkeletonType", 0},
    {"RotationOrder", 0},
    {"BoneAxis", 0},
    {"PlaneAxis", 0},
    {0, 0},
  };

  static const VariableMapper htrExportOptionsVariableMapper[] = {
    {"BasePositionOption", 0},
    {0, 0},
  };

  static const VariableMapper segmentsVariableMapper[] = {
    {"Comment", 0},
    {"NumberOf", 0},
    {0, 0},
  };

  static const VariableMapper modePoseVariableMapper[] = {
    {"NumberOf", 0},
    {0, 0},
  };

  static const VariableMapper personalInfoVariableMapper[] = {
    {"Name", 0},
    {"Height", 0},
    {"Weight", 0},
    {0, 0},
  };

  static const VariableMapper massModelVariableMapper[] = {
    {"Description", 0},
    {"Type", 0},
    {"Gender", 0},
    {"Comment", 0},
    {"NumberOfSegments", 0},
    {0, 0},
  };

  enum MarsMarkerTypes
    {
      TWO_POINTS_RATIO = 2,
      TWO_POINTS_MEASURED = 3,
      THREE_POINTS_RATIO = 4,
      THREE_POINTS_MEASURED = 5,
      EMR_MARKER = 6,
      ONE_POINT_MEASURED = 7,
      RELATIVE_TO_BONE = 8
    };

  MarsMarkerSetFactory::MarsMarkerSetFactory ()
    : palette_ ()
  {
    // http://www.colourlovers.com/palette/3320274/Paper_Straws
    palette_.resize (5);
    palette_[0].data () = 0x467D7700;
    palette_[1].data () = 0x82924900;
    palette_[2].data () = 0xEFD07500;
    palette_[3].data () = 0xF9AECF00;
    palette_[4].data () = 0xECC6B300;
  }

  MarsMarkerSetFactory::~MarsMarkerSetFactory ()
  {}

  MarsMarkerSetFactory&
  MarsMarkerSetFactory::operator= (const MarsMarkerSetFactory& rhs)
  {
    if (this == &rhs)
      return *this;
    return *this;
  }

  MarkerSet
  MarsMarkerSetFactory::load (const std::string& filename)
  {
    std::ifstream file (filename.c_str ());
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    MarkerSet result;

    std::string line;
    while (!file.eof ())
      {
	std::getline (file, line);
	trimEndOfLine (line);

	if (line[0] != '[')
	  {
	    std::string error = "failed to load `"
	      + filename
	      + "': invalid format (a section was expected but `"
	      + line[0] + "' was found)";
	    throw std::runtime_error (error);
	  }
	line = line.substr (1, line.size () - 2);

	const SectionMapper* mapper = sectionMapper;
	while (mapper && mapper->section != 0)
	  {
	    if (line == mapper->section)
	      {
		if (mapper->loader)
		  (this->*(mapper->loader)) (result, file);
		break;
	      }
	    ++mapper;
	  }
	if (mapper && mapper->section == 0)
	  {
	    std::string error =
	      "failed to load file `"
	      + filename
	      + "': unknown section `"
	      + line + "'";
	    throw std::runtime_error (error);
	  }
      }
    return result;
  }

  void
  MarsMarkerSetFactory::loadSection
  (const VariableMapper* variableMapper, MarkerSet& markerSet,
   std::ifstream& file)
  {
    std::string line;
    std::string variable;
    std::string value;
    std::string::size_type idx;
    std::streamoff filepos;

    assert (!!variableMapper);

    while (!file.eof () && file.peek () != '[' && !file.eof ())
      {
	filepos = file.tellg ();
	std::getline (file, line);
	trimEndOfLine (line);

	idx = line.find ('=');
	if (idx == std::string::npos)
	  {
	    // restore file position
	    file.seekg (filepos ,std::ios_base::beg);

	    throw std::runtime_error
	      ("invalid syntax while parsing file on line ("
	       + line.substr(0, line.size () - 1)
	       + ")");
	  }

	variable = line.substr (0, idx);
	value = line.substr (idx + 1);

	const VariableMapper* mapper = variableMapper;
	while (mapper && mapper->variable != 0)
	  {
	    if (variable == mapper->variable)
	      {
		if (mapper->loader)
		  (this->*(mapper->loader)) (markerSet, value);
		break;
	      }
	    ++mapper;
	  }
	if (mapper && mapper->variable == 0)
	  {
	    std::string error =
	      "unknown variable `"
	      + variable + "'";
	    std::cerr << "warning: " << error << std::endl;
	  }
      }
  }

  void
  MarsMarkerSetFactory::loadGeneralInformation
  (MarkerSet& markerSet, std::ifstream& file)
  {
    loadSection (generalInformationVariableMapper, markerSet, file);
  }

  std::vector<std::vector<std::string> >
  MarsMarkerSetFactory::loadUnformattedData
  (std::ifstream& file)
  {
    std::string line;
    std::vector<std::vector<std::string> > values;
    std::string::size_type idx;

    // Here we make the assumption the unformatted data are at the end
    // of each section.
    while (!file.eof () && file.peek () != '[' && !file.eof ())
      {
	if (file.eof ())
	  break;

	std::getline (file, line);
	trimEndOfLine (line);

	values.push_back (std::vector<std::string> ());

	idx = line.find (',');
	while (idx != std::string::npos)
	  {
	    values[values.size () - 1].push_back (line.substr (0, idx));
	    line = line.substr (idx + 1);
	    idx = line.find (',');
	  }

	if (!line.empty ())
	  values[values.size () - 1].push_back (line);
      }
    return values;
  }

  void
  MarsMarkerSetFactory::loadMarkers
  (MarkerSet& markerSet, std::ifstream& file)
  {
    try
      {
	loadSection (virtualMarkersVariableMapper, markerSet, file);
      }
    catch (const std::runtime_error&)
      {
	std::vector<std::vector<std::string> > values =
	  loadUnformattedData (file);

	std::vector<std::vector<std::string> >::const_iterator itLine;
	for (itLine = values.begin (); itLine != values.end (); ++itLine)
	  {
	    if (itLine->size () != 6)
	      {
		std::ostringstream error;
		error
		  << "unexpected length in marker data"
		  " (6 was expected but length is "
		  << itLine->size () << ")";
		throw std::runtime_error (error.str ());
	      }


	    Marker* marker = new Marker ();
	    try
	      {
		marker->id () = convert<int> ((*itLine)[0]) - 1;
		marker->name () = (*itLine)[1];
		trimWhitespace (marker->name ());
		marker->color () =
		  getColorFromPalette (convert<std::size_t> ((*itLine)[2]));
		marker->physicalColor () =
		  getColorFromPalette (convert<std::size_t> ((*itLine)[3]));
		marker->size () = convert<double> ((*itLine)[4]);
		marker->optional () = convert<int> ((*itLine)[5]);;
		markerSet.markers ().push_back (marker);
	      }
	    catch (...)
	      {
		delete marker;
		throw;
	      }
	  }

	if (!file.eof () && file.peek () != '[' && !file.eof ())
	  loadSection (markersVariableMapper, markerSet, file);
      }
  }

  void
  MarsMarkerSetFactory::loadVirtualMarkers
  (MarkerSet& markerSet, std::ifstream& file)
  {
    try
      {
	loadSection (markersVariableMapper, markerSet, file);
      }
    catch (const std::runtime_error&)
      {
	std::vector<std::vector<std::string> > values =
	  loadUnformattedData (file);

	std::vector<std::vector<std::string> >::const_iterator itLine;
	for (itLine = values.begin (); itLine != values.end (); ++itLine)
	  {
	    if (itLine->size () != 10)
	      {
		std::ostringstream error;
		error
		  << "unexpected length in virtual marker data"
		  " (10 was expected but length is "
		  << itLine->size () << ")";
		throw std::runtime_error (error.str ());
	      }

	    int markerType = convert<int> ((*itLine)[2]);
	    AbstractVirtualMarker* marker;
	    switch (markerType)
	      {
	      case TWO_POINTS_RATIO:
		{
		  marker =
		    new VirtualMarkerTwoPointsRatio
		    (convert<double> ((*itLine)[7]));
		  break;
		}

	      case THREE_POINTS_RATIO:
		{
		  marker =
		    new VirtualMarkerThreePointsRatio
		    (convert<double> ((*itLine)[6]),
		     convert<double> ((*itLine)[7]),
		     convert<double> ((*itLine)[8]));
		  break;
		}

	      case TWO_POINTS_MEASURED:
		{
		  marker =
		    new VirtualMarkerTwoPointsMeasured
		    (convert<double> ((*itLine)[7]) * 1e-3);
		  break;
		}
	      case THREE_POINTS_MEASURED:
		{
		  marker =
		    new VirtualMarkerThreePointsMeasured
		    (convert<double> ((*itLine)[6]) * 1e-3,
		     convert<double> ((*itLine)[7]) * 1e-3,
		     convert<double> ((*itLine)[8]) * 1e-3);
		  break;
		}

	      case ONE_POINT_MEASURED:
		{
		  marker =
		    new VirtualMarkerOnePointMeasured
		    (convert<double> ((*itLine)[6]) * 1e-3,
		     convert<double> ((*itLine)[7]) * 1e-3,
		     convert<double> ((*itLine)[8]) * 1e-3);
		  break;
		}

	       case EMR_MARKER:
	       case RELATIVE_TO_BONE:
	       default:
		 throw std::runtime_error ("unknown marker type");
	       }

	     try
	       {
		 marker->id () = convert<int> ((*itLine)[0]) - 1;
		 marker->name () = (*itLine)[1];
		 trimWhitespace (marker->name ());
		 marker->color () =
		   randomizeColorRGB ();
		 marker->physicalColor () =
		   randomizeColorRGB ();
		 marker->size () = 0.;
		 marker->optional () = true;

		 marker->originMarker () =
		   convert<int> ((*itLine)[3]) - 1;
		 marker->longAxisMarker () =
		   convert<int> ((*itLine)[4]) - 1;
		 marker->planeAxisMarker () =
		   convert<int> ((*itLine)[5]) - 1;

		 // load offset?

		 markerSet.markers ().push_back (marker);
	       }
	     catch (...)
	       {
		 delete marker;
		 throw;
	       }
	   }

	 if (!file.eof () && file.peek () != '[' && !file.eof ())
	   loadSection (virtualMarkersVariableMapper, markerSet, file);
       }
   }

   void
   MarsMarkerSetFactory::loadVMJoinDefs
   (MarkerSet& markerSet, std::ifstream& file)
   {
     loadSection (VMJoinDefsVariableMapper, markerSet, file);
   }

   void
   MarsMarkerSetFactory::loadLinkages
   (MarkerSet& markerSet, std::ifstream& file)
   {
     try
       {
	 loadSection (linkagesVariableMapper, markerSet, file);
       }
     catch (const std::runtime_error&)
       {
	 std::vector<std::vector<std::string> > values =
	   loadUnformattedData (file);

	 std::vector<std::vector<std::string> >::const_iterator itLine;
	 for (itLine = values.begin (); itLine != values.end (); ++itLine)
	   {
	     //FIXME: can be 7 or 8, extra field?!
	     if (itLine->size () != 7 && itLine->size () != 8)
	       {
		 std::ostringstream error;
		 error
		   << "unexpected length in linkage data"
		   " 8 was expected but length is "
		   << itLine->size () << ")";
		 throw std::runtime_error (error.str ());
	       }

	     Link linkage;
	     linkage.name () = (*itLine)[0];
	     trimWhitespace (linkage.name ());
	     linkage.color () =
	       getColorFromPalette (convert<std::size_t> ((*itLine)[1]));
	     linkage.type () = Link::LINK_UNKNOWN;

	     std::istringstream ss ((*itLine)[3]);
	     int marker1, marker2;
	     ss >> marker1 >> marker2;
	     linkage.marker1 () = marker1 - 1;
	     linkage.marker2 () = marker2 - 1;
	     linkage.minLength () = convert<double> ((*itLine)[4]);
	     linkage.maxLength () = convert<double> ((*itLine)[5]);
	     linkage.extraStretch () = convert<double> ((*itLine)[6]);
	     markerSet.links ().push_back (linkage);
	   }


	 if (!file.eof () && file.peek () != '[' && !file.eof ())
	   loadSection (linkagesVariableMapper, markerSet, file);
       }
   }

   void
   MarsMarkerSetFactory::loadSkeletonType
   (MarkerSet& markerSet, std::ifstream& file)
   {
     loadSection (skeletonTypeVariableMapper, markerSet, file);
   }

   void
   MarsMarkerSetFactory::loadCalciumModel
   (MarkerSet& /*markerSet*/, std::ifstream& file)
   {
     std::string line;
     while (!file.eof () && file.peek () != '[' && !file.eof ())
       {
	 std::getline (file, line);
	 trimEndOfLine (line);
       }
   }

   void
   MarsMarkerSetFactory::loadHtrExportOptions
   (MarkerSet& markerSet, std::ifstream& file)
   {
     loadSection (htrExportOptionsVariableMapper, markerSet, file);
   }

   void
   MarsMarkerSetFactory::loadSegments
   (MarkerSet& markerSet, std::ifstream& file)
   {
     try
       {
	 loadSection (segmentsVariableMapper, markerSet, file);
       }
     catch (const std::runtime_error&)
       {
	 std::vector<std::vector<std::string> > values =
	   loadUnformattedData (file);

	 std::vector<std::vector<std::string> >::const_iterator itLine;
	 for (itLine = values.begin (); itLine != values.end (); ++itLine)
	   {
	     if (itLine->size () != 9)
	       {
		 std::ostringstream error;
		 error
		  << "unexpected length in segment data"
		  " (9 was expected but length is "
		  << itLine->size () << ")";
		throw std::runtime_error (error.str ());
	      }

	    Segment segment;
	    segment.id () = convert<int> ((*itLine)[0]) - 1;
	    segment.name () = (*itLine)[1];
	    trimWhitespace (segment.name ());
	    // FIXME: load parent?
	    segment.originMarker () = convert<int> ((*itLine)[3]) - 1;
	    segment.longAxisMarker () = convert<int> ((*itLine)[4]) - 1;
	    segment.planeAxisMarker () = convert<int> ((*itLine)[5]) - 1;
	    segment.rotationOffset ().roll () =
	      convert<double> ((*itLine)[6]) * M_PI / 180.;
	    segment.rotationOffset ().pitch () =
	      convert<double> ((*itLine)[6]) * M_PI / 180.;
	    segment.rotationOffset ().yaw () =
	      convert<double> ((*itLine)[6]) * M_PI / 180.;

	    markerSet.segments ().push_back (segment);
	  }

	if (!file.eof () && file.peek () != '[' && !file.eof ())
	  loadSection (segmentsVariableMapper, markerSet, file);
      }
  }

  void
  MarsMarkerSetFactory::loadModelPose
  (MarkerSet& markerSet, std::ifstream& file)
  {
    try
      {
	loadSection (modePoseVariableMapper, markerSet, file);
      }
    catch (const std::runtime_error&)
      {
	std::vector<std::vector<std::string> > values =
	  loadUnformattedData (file);

	std::vector<std::vector<std::string> >::const_iterator itLine;
	Pose pose;
	int id;
	for (itLine = values.begin (); itLine != values.end (); ++itLine)
	  {
	    id = convert<int> ((*itLine)[0]);
	    pose.positions ().resize
	      (std::max (static_cast<std::size_t> (id), pose.positions ().size ()));

	    if (itLine->size () != 4)
	      {
		std::ostringstream error;
		error
		  << "unexpected length in linkage data"
		  " 4 was expected but length is "
		  << itLine->size () << ")";
		throw std::runtime_error (error.str ());
	      }

	    if (id < 1)
	      throw std::runtime_error ("invalid pose id");

	    std::vector<double> position (3);

	    position[0] = convert<double> ((*itLine)[1]);
	    position[1] = convert<double> ((*itLine)[2]);
	    position[2] = convert<double> ((*itLine)[3]);

	    pose.positions ()[id - 1] = position;
	  }
	markerSet.poses ().push_back (pose);

	if (!file.eof () && file.peek () != '[' && !file.eof ())
	  loadSection (modePoseVariableMapper, markerSet, file);
      }

  }

  void
  MarsMarkerSetFactory::loadPersonalInfo
  (MarkerSet& markerSet, std::ifstream& file)
  {
    loadSection (personalInfoVariableMapper, markerSet, file);
  }

  void
  MarsMarkerSetFactory::loadMassModel
  (MarkerSet& markerSet, std::ifstream& file)
  {
    try
      {
	loadSection (massModelVariableMapper, markerSet, file);
      }
    catch (const std::runtime_error&)
      {
	std::vector<std::vector<std::string> > values =
	  loadUnformattedData (file);

	std::vector<std::vector<std::string> >::const_iterator itLine;
	for (itLine = values.begin (); itLine != values.end (); ++itLine)
	  {
	    if (itLine->size () != 8)
	      {
		std::ostringstream error;
		error
		  << "unexpected length in mass model data"
		  " (8 was expected but length is "
		  << itLine->size () << ")";
		throw std::runtime_error (error.str ());
	      }
	  }

	if (!file.eof () && file.peek () != '[' && !file.eof ())
	  loadSection (massModelVariableMapper, markerSet, file);
      }
  }

  void
  MarsMarkerSetFactory::loadVersion
  (MarkerSet& /*markerSet*/, const std::string& value)
  {
    if (value != "5")
      throw std::runtime_error
	("Mars file loader only support version 5,"
	 " but file format version is " + value);
  }

  void
  MarsMarkerSetFactory::loadProjectName
  (MarkerSet& markerSet, const std::string& value)
  {
    markerSet.name () = value;
  }

  const Color&
  MarsMarkerSetFactory::getColorFromPalette (std::size_t id)
  {
    if (id >= palette_.size ())
      {
	std::size_t oldSize  = palette_.size ();
	palette_.resize (id + 1);
	for (id = oldSize; id < palette_.size (); ++id)
	  palette_[id] = randomizeColorRGB ();
	return palette_.back ();
      }
    return palette_[id];
  }

  bool
  MarsMarkerSetFactory::canLoad (const std::string& filename)
  {
    std::string extension = extractExtension (filename);
    std::transform (extension.begin (),
		    extension.end(),
		    extension.begin(), ::tolower);
    return extension == "mars";
  }
} // end of namespace libmocap.
