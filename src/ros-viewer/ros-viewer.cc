#include <ncurses.h>

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
#include "marker-set-link-view.hh"
#include "marker-set-name-view.hh"
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
	pub_ (n.advertise<visualization_msgs::MarkerArray>
	      ("markers", 1, true)),
	msg_ (),
	views_ ()
    {
      views_.push_back (new MarkerTrajectoryView (trajectory, markerSet));
      views_.push_back (new MarkerSetLinkView (trajectory, markerSet));

      for (int i = 0; i < static_cast<int> (markerSet.markers ().size ()); ++i)
	views_.push_back (new MarkerSetNameView (trajectory, markerSet, i));
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

std::string makeProgressBar (int frameId, int nFrames, int len)
{
  std::string result;
  result.reserve (len);

  int progress = frameId * (len - 2) / nFrames;

  result += '|';

  for (int pos = 0; pos < len - 2; ++pos)
    if (pos <= progress)
      result += '#';
    else
      result += ' ';

  result += "|";

  return result;
}

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
    }

  ros::init (argc, argv, "libmocap");
  ros::NodeHandle n;
  libmocap::MarkerPublisher markerPublisher (n, trajectory, markerSet);


  int frameId = 0;
  bool play = true;

  int ch;

  initscr ();
  cbreak ();
  keypad (stdscr, TRUE);
  noecho ();
  curs_set (0);
  nodelay (stdscr, TRUE);

  int row, col;
  int oldrow, oldcol;
  std::string mesg1 = "ROS Motion Capture Data Player";
  std::string mesg2;
  std::string mesg3;

  ros::Rate r (trajectory.dataRate ());
  while (ros::ok ())
    {
      // Input management.
      ch = getch ();
      if (ch == KEY_RIGHT)
	{
	  if (frameId < trajectory.numFrames () - 1)
	    frameId++;
	}
      else if (ch == KEY_LEFT)
	{
	  if (frameId > 0)
	    frameId--;
	}
      else if (ch == KEY_F (2))
	{
	  ros::shutdown ();
	  break;
	}
      else if (ch == ' ')
	play = !play;

      oldrow = row, oldcol = col;
      getmaxyx (stdscr, row, col);
      if (oldrow != row || oldcol != col)
	clear ();
      else
	{
	  move (row / 2 - 1, 0);
	  clrtoeol ();
	  move (row / 2, 0);
	  clrtoeol ();
	  move (row / 2 + 1, 0);
	  clrtoeol ();
	}

      std::stringstream stream;
      stream
	<< "frame " << frameId << " / " << trajectory.numFrames () - 1
	<< " (play = " << (play ? "on" : "off") << ")";
      mesg2 = stream.str ();
      mesg3 = makeProgressBar (frameId, trajectory.numFrames (),
			       static_cast<int> (.75 * col));

      mvprintw (row / 2 - 1, (col - static_cast<int> (mesg1.size ())) / 2, "%s", mesg1.c_str ());
      mvprintw (row / 2, (col - static_cast<int> (mesg2.size ())) / 2, "%s", mesg2.c_str ());
      mvprintw (row / 2 + 1, (col - static_cast<int> (mesg3.size ())) / 2, "%s", mesg3.c_str ());

      refresh ();

      ros::spinOnce ();

      if (play)
	{
	  if (frameId < trajectory.numFrames () - 1)
	    ++frameId;
	  else
	    frameId = 0;
	}
      if (ros::ok ())
	markerPublisher.publish (frameId);

      r.sleep ();
    }
  endwin ();

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
