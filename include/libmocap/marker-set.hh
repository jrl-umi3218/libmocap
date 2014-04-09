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

#ifndef LIBMOCAP_MARKER_SET_HH
# define LIBMOCAP_MARKER_SET_HH
# include <string>
# include <vector>

# include <libmocap/config.hh>
# include <libmocap/abstract-marker.hh>
# include <libmocap/link.hh>
# include <libmocap/pose.hh>
# include <libmocap/segment.hh>
# include <libmocap/util.hh>

namespace libmocap
{
  /// \brief A marker set stores data about a subset of the markers
  /// sharing geometric constraints.
  ///
  /// A marker set often represents an object whose motion is
  /// captured. It possesses:
  ///
  /// - a name (#name_) which is used to identify it marker data,
  /// - markers (#markers_) describing how to compute the marker
  ///   position from the marker parameters,
  /// - links (#links_) expressing the geometric relationships between
  ///   the markers in the subset. It is used to recognize the marker
  ///   set in a scene.
  /// - segments (#segments_) defining zero, one or more frames whose
  ///   position is given by the position of some markers. Each
  ///   segment can have a parent allowing the definition of a tree
  ///   (i.e. skeleton).
  /// - poses (#poses_) a particular set of marker positions for this
  ///   marker set.
  ///
  /// Using marker set information, objects or characters can be
  /// tracker in raw motion capture data. Indeed, any subset of
  /// markers matching the constraints of the marker set will be
  /// recognized as the tracked object. These constraints are
  /// expressed by Link objects.
  ///
  /// The constraints expressed by the Link objects concern the
  /// markers relative positions. Most of them will be physical
  /// markers but some virtual markers can also be defined. A virtual
  /// marker is a marker whose position is a function of the position
  /// of other (physical) markers.
  ///
  /// Segments are frames attached to the object. Using three markers,
  /// one can define a 6D frame (position and orientation). Notably,
  /// frames can be used to track reference frames attached to bodies
  /// of an articulated system.
  class LIBMOCAP_DLLEXPORT MarkerSet
  {
  public:
    MarkerSet ();
    ~MarkerSet ();
    MarkerSet& operator= (const MarkerSet& rhs);

    LIBMOCAP_ACCESSOR (name, std::string);
    LIBMOCAP_ACCESSOR (markers, std::vector<AbstractMarker*>);
    LIBMOCAP_ACCESSOR (links, std::vector<Link>);
    LIBMOCAP_ACCESSOR (segments, std::vector<Segment>);
    LIBMOCAP_ACCESSOR (poses, std::vector<Pose>);
  private:
    std::string name_;
    std::vector<AbstractMarker*> markers_;
    std::vector<Link> links_;
    std::vector<Segment> segments_;
    std::vector<Pose> poses_;
  };
} // end of namespace libmocap.

#endif //! LIBMOCAP_MARKER_SET_HH
