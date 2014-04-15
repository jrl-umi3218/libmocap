#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <libmocap/marker-set-factory.hh>
#include <libmocap/marker-trajectory-factory.hh>

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include "view.hh"
#include "marker-trajectory-view.hh"

namespace libmocap
{
  class MarkerPublisher
  {
  public:
    MarkerPublisher (ros::NodeHandle& n,
		     libmocap::MarkerTrajectory& trajectory,
		     libmocap::MarkerSet& markerSet)
      : n_ (n),
	pub_ (n.advertise<visualization_msgs::MarkerArray>("markers", 1, true)),
	msg_ (),
	views_ ()
    {
      views_.push_back (new MarkerTrajectoryView (trajectory, markerSet));
    }

    ~MarkerPublisher ()
    {
      std::vector<View*>::const_iterator it;
      for (it = views_.begin (); it != views_.end (); ++it)
	delete *it;
    }

    void publish (int frameId)
    {
      updateMessage (frameId);
      pub_.publish (msg_);
    }

  private:
    void updateMessage (int frameId)
    {
      msg_.markers.resize (views_.size ());
      std::vector<View*>::const_iterator it;
      for (it = views_.begin (); it != views_.end (); ++it)
	if (*it)
	  msg_.markers[it - views_.begin ()] = (*it)->markerMessage (frameId);
    }

    ros::NodeHandle& n_;
    ros::Publisher pub_;
    visualization_msgs::MarkerArray msg_;

    std::vector<View*> views_;
  };
} // end of namespace libmocap.

int mainSafe (int argc, char* argv[])
{
  if (argc != 2 && argc != 3)
    {
      std::cout <<
	"usage: ros-viewer TRC_FILE [MARS_FILE]\n"
	"\n"
	"TRC_FILE must be a valid TRC files\n"
	"i.e. a file containing markers trajectories\n"
	"\n"
	"an optional marker set file (MARS_FILE)\n"
	"can be given to enhance data display\n"
	;
      return 1;
    }

  std::string filename (argv[1]);
  libmocap::MarkerTrajectoryFactory factory;
  libmocap::MarkerTrajectory trajectory = factory.load (filename);
  trajectory.normalize ();

  libmocap::MarkerSet markerSet;
  if (argc == 3)
    {
      std::string filenameMars = argv[2];
      libmocap::MarkerSetFactory factoryMarkerSet;
      markerSet = factoryMarkerSet.load (filenameMars);
      std::cout << markerSet << std::endl;
    }

  ros::init (argc, argv, "libmocap");
  ros::NodeHandle n;
  libmocap::MarkerPublisher markerPublisher (n, trajectory, markerSet);
  ros::Rate r (trajectory.dataRate ());
  for (int frameId = 0; frameId < trajectory.numFrames (); ++frameId)
    {
      std::cout << "Frame: " << frameId << '\n';
      markerPublisher.publish (frameId);
      r.sleep ();
      if (!ros::ok ())
	return 0;
    }
  return 0;
}

int main (int argc, char* argv[])
{
  try
    {
      return mainSafe (argc, argv);
    }
  catch (const std::exception& e)
    {
      std::cerr << e.what () << std::endl;
      return 1;
    }
}
