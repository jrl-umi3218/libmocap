#include <cstdlib>
#include <stdexcept>
#include <libmocap/marker-set-factory.hh>

int main ()
{
  libmocap::MarkerSetFactory factory;
  std::string file = LIBMOCAP_DATA_PATH "human.mars";
  libmocap::MarkerSet markerSet = factory.load (file);
  return 0;
}
