#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <libmocap/marker-trajectory-factory.hh>

int main ()
{
  libmocap::MarkerTrajectoryFactory factory;

  std::string humanMars = LIBMOCAP_DATA_PATH "human.trc";
  std::string boxMars = LIBMOCAP_DATA_PATH "box.trc";
  try
    {
      libmocap::MarkerTrajectory humanMarkerTrajectory = factory.load (humanMars);
      std::cout << humanMarkerTrajectory << std::endl;
      libmocap::MarkerTrajectory boxMarkerTrajectory = factory.load (boxMars);
      std::cout << boxMarkerTrajectory << std::endl;
    }
  catch (const std::exception& e)
    {
      std::cerr << e.what () << std::endl;
      return 1;
    }
  std::cout << "file loaded with success!" << std::endl;
  return 0;
}
