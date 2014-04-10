#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <libmocap/marker-set-factory.hh>

int main ()
{
  libmocap::MarkerSetFactory factory;
  std::string file = LIBMOCAP_DATA_PATH "human.mars";
  try
    {
  libmocap::MarkerSet markerSet = factory.load (file);
    }
  catch (const std::exception& e)
    {
      std::cerr << e.what () << std::endl;
      return 1;
    }
  std::cout << "file loaded with success!" << std::endl;
  return 0;
}
