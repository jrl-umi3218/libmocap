#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <libmocap/marker-set-factory.hh>
#include <libmocap/marker-trajectory-factory.hh>

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <visualization_msgs/Marker.h>

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


  ros::init(argc, argv, "libmocap");

  ros::NodeHandle n;
  ros::Rate r (trajectory.dataRate ());
  ros::Publisher markerPub =
    n.advertise<visualization_msgs::Marker>("markers", 1, true);

  visualization_msgs::Marker marker;
  marker.header.frame_id = "/world";
  marker.header.stamp = ros::Time::now ();
  marker.ns = "markers";
  marker.id = 0;
  marker.type = visualization_msgs::Marker::SPHERE_LIST;

  // Set the marker action.  Options are ADD and DELETE
  marker.action = visualization_msgs::Marker::ADD;

  // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
  marker.pose.position.x = 0;
  marker.pose.position.y = 0;
  marker.pose.position.z = 0;
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;

  // Set the scale of the marker -- 0.05x0.05x0.05 here means 5cm radius
  marker.scale.x = 0.05;
  marker.scale.y = 0.05;
  marker.scale.z = 0.05;

  // Set the color -- be sure to set alpha to something non-zero!
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

  marker.lifetime = ros::Duration ();
  marker.points.resize (trajectory.numMarkers ());

  int offset = 1;
  //int markerId;

  for (int frameId = 0; frameId < trajectory.numFrames (); ++frameId)
    {
      std::cout << "Frame: " << frameId << '\n';

      // Fill marker information
      if (frameId >= static_cast<int> (trajectory.positions ().size ()))
	{
	  std::cerr << "size mismatch (trajectory)" << std::endl;
	  continue;
	}
      if (trajectory.positions ()[frameId].empty ())
	{
	  std::cerr << "size mismatch (frame in trajectory)" << std::endl;
	  continue;
	}
      for (int i = 0; i < trajectory.numMarkers (); ++i)
      	{
	  if (offset + i * 3 + 2
	      >= static_cast<int> (trajectory.positions ()[frameId].size ()))
	    {
	      std::cerr << "size mismatch (while iterating)" << std::endl;
	      continue;
	    }
      	  marker.points[i].x = trajectory.positions ()[frameId][offset + i * 3 + 0];
      	  marker.points[i].y = trajectory.positions ()[frameId][offset + i * 3 + 1];
      	  marker.points[i].z = trajectory.positions ()[frameId][offset + i * 3 + 2];
      	}
      std::cout << "publishing frame " << frameId << std::endl;

      ++marker.header.seq;
      marker.header.stamp = ros::Time::now();
      markerPub.publish (marker);
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
