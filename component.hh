#ifndef MCSTAS_COMPONENT_BASE_HH
#define MCSTAS_COMPONENT_BASE_HH

#include "particle.hh"
#include <openPMD/openPMD.hpp>
#include <string>

/** \class component
 * \brief base class for any component
 */
class component {
public:
  explicit component(const std::string &name);

private:
  std::string _name;
};

class openPMD_io : private component {
public:
  explicit openPMD_io(const std::string &name);

  /** initializes the "series" object from the openPMD API */
  void init(const char *extension, unsigned int iter, unsigned long long int n_neutrons);

  /** save neutron properties in a vector and dump it to the file in chunks */
  void trace(double x, double y, double z, double sx, double sy, double sz,
             double vx, double vy, double vz, double t, double p);

  /** flushes the output to file before closing it */
  void save(void);

private:
  std::unique_ptr<openPMD::Series> _series;
  //std::vector<particle> _neutrons;
  particles _neutrons;
  /** declare the neutron particle species in the file */
  void init_neutrons(unsigned int iter, unsigned long long int n_neutrons);
};

#endif
