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
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include "mars-marker-set-factory.hh"

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
    {"SkeletonTypes", &MarsMarkerSetFactory::loadSkeletonType},
    {"HtrExportOptions", &MarsMarkerSetFactory::loadHtrExportOptions},
    {"Segments", &MarsMarkerSetFactory::loadSegments},
    {"ModelPose", &MarsMarkerSetFactory::loadModelPose},
    {"Personal Info", &MarsMarkerSetFactory::loadPersonalInfo},
    {0, 0}
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
  };



  static std::string extractExtension (const std::string& filename)
  {
    std::string::size_type idx = filename.rfind ('.');
    if (idx != std::string::npos)
      return filename.substr (idx + 1);
    else
      return filename;
  }

  static void trimEndOfLine (std::string& s)
  {
    s.erase (s.find_last_not_of ("\n\r") + 1);
  }

  MarsMarkerSetFactory::MarsMarkerSetFactory ()
  {}

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

    assert (!!variableMapper);

    while (!file.eof () && file.peek () != '[')
      {
	std::getline (file, line);
	trimEndOfLine (line);

	idx = line.find ('=');
	if (idx == std::string::npos)
	  throw std::runtime_error
	    ("invalid syntax while parsing file on line ("
	     + line.substr(0, line.size () - 1)
	     + ")");

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
	    throw std::runtime_error (error);
	  }
      }
  }

  void
  MarsMarkerSetFactory::loadGeneralInformation
  (MarkerSet& markerSet, std::ifstream& file)
  {
    loadSection (generalInformationVariableMapper, markerSet, file);
  }

  void
  MarsMarkerSetFactory::loadMarkers
  (MarkerSet& markerSet, std::ifstream& file)
  {
    loadSection (markersVariableMapper, markerSet, file);
  }

  void
  MarsMarkerSetFactory::loadVirtualMarkers
  (MarkerSet& /*markerSet*/, std::ifstream& /*file*/)
  {
  }

  void
  MarsMarkerSetFactory::loadVMJoinDefs
  (MarkerSet& /*markerSet*/, std::ifstream& /*file*/)
  {
  }

  void
  MarsMarkerSetFactory::loadLinkages
  (MarkerSet& /*markerSet*/, std::ifstream& /*file*/)
  {
  }

  void
  MarsMarkerSetFactory::loadSkeletonType
  (MarkerSet& /*markerSet*/, std::ifstream& /*file*/)
  {
  }

  void
  MarsMarkerSetFactory::loadHtrExportOptions
  (MarkerSet& /*markerSet*/, std::ifstream& /*file*/)
  {
  }

  void
  MarsMarkerSetFactory::loadSegments
  (MarkerSet& /*markerSet*/, std::ifstream& /*file*/)
  {
  }

  void
  MarsMarkerSetFactory::loadModelPose
  (MarkerSet& /*markerSet*/, std::ifstream& /*file*/)
  {
  }

  void
  MarsMarkerSetFactory::loadPersonalInfo
  (MarkerSet& /*markerSet*/, std::ifstream& /*file*/)
  {
  }


  void
  MarsMarkerSetFactory::loadVersion
  (MarkerSet& /*markerSet*/, const std::string& value)
  {
    if (value != "5")
      throw std::runtime_error
	("Mars file loader only support version 5,"
	 " but file format version is |" + value +"|");
  }

  void
  MarsMarkerSetFactory::loadProjectName
  (MarkerSet& markerSet, const std::string& value)
  {
    markerSet.name () = value;
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
