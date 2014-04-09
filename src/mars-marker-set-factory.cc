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
#include <algorithm>
#include <stdexcept>
#include "mars-marker-set-factory.hh"

namespace libmocap
{
  static std::string extractExtension (const std::string& filename)
  {
    std::string::size_type idx = filename.rfind ('.');
    if (idx != std::string::npos)
      return filename.substr (idx + 1);
    else
      return filename;
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
  MarsMarkerSetFactory::load (const std::string& /*filename*/)
  {
    MarkerSet result;
    return result;
  }

  bool
  MarsMarkerSetFactory::canLoad (const std::string& filename)
  {
    std::string extension = extractExtension (filename);
    std::transform (extension.begin (), extension.end(), extension.begin(), ::tolower);
    return extension == "mars";
  }
} // end of namespace libmocap.
