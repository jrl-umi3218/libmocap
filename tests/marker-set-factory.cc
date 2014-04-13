#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <libmocap/marker-set-factory.hh>

int main ()
{
  libmocap::MarkerSetFactory factory;

  std::string humanMars = LIBMOCAP_DATA_PATH "human.mars";
  std::string boxMars = LIBMOCAP_DATA_PATH "box.mars";
  try
    {
      libmocap::MarkerSet humanMarkerSet = factory.load (humanMars);
      std::cout << humanMarkerSet << std::endl;
      libmocap::MarkerSet boxMarkerSet = factory.load (boxMars);
      std::cout << boxMarkerSet << std::endl;
    }
  catch (const std::exception& e)
    {
      std::cerr << e.what () << std::endl;
      return 1;
    }
  std::cout << "file loaded with success!" << std::endl;
  return 0;
}
