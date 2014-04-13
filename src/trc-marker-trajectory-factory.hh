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

#ifndef LIBMOCAP_TRC_MARKER_TRAJECTORY_FACTORY_HH
# define LIBMOCAP_TRC_MARKER_TRAJECTORY_FACTORY_HH
# include <iosfwd>
# include <string>

# include <libmocap/marker-trajectory.hh>

namespace libmocap
{
  class VariableMapper;

  class TrcMarkerTrajectoryFactory
  {
  public:
    TrcMarkerTrajectoryFactory ();
    ~TrcMarkerTrajectoryFactory ();
    TrcMarkerTrajectoryFactory& operator= (const TrcMarkerTrajectoryFactory& rhs);

    MarkerTrajectory load (const std::string& filename);

    static bool canLoad (const std::string& filename);


    void
    loadDataRate
    (MarkerTrajectory& trajectory, const std::string& value);
    void
    loadCameraRate
    (MarkerTrajectory& trajectory, const std::string& value);
    void
    loadNumFrames
    (MarkerTrajectory& trajectory, const std::string& value);
    void
    loadNumMarkers
    (MarkerTrajectory& trajectory, const std::string& value);
    void
    loadUnits
    (MarkerTrajectory& trajectory, const std::string& value);
    void
    loadOrigDataRate
    (MarkerTrajectory& trajectory, const std::string& value);
    void
    loadOrigDataStartFrame
    (MarkerTrajectory& trajectory, const std::string& value);
    void
    loadOrigNumFrames
    (MarkerTrajectory& trajectory, const std::string& value);

  private:
    void loadHeader (std::ifstream& file, MarkerTrajectory& trajectory);
    void loadData (std::ifstream& file, MarkerTrajectory& trajectory);

  };
} // end of namespace libmocap.

#endif //! LIBMOCAP_TRC_MARKER_TRAJECTORY_HH
