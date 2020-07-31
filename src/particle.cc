#include "particle.hh"
#include <array>
#include <cmath>

#ifndef V2SE
#define VS2E 5.22703725e-6 /* Convert (v[m/s])**2 to E[meV] */
#endif

particles::particles() : _size(0) {}

void particles::push_back(double x, double y, double z, double sx, double sy,
                    double sz, double vx, double vy, double vz, double t,
                    double p){ //, uint32_t userflag) {
  double abs_v = sqrt(vx * vx + vy * vy + vz * vz);
  double ekin = VS2E * abs_v * abs_v / 1e9;

  _x.push_back(x*100.); //cm
  _y.push_back(y*100.);
  _z.push_back(z*100.);
  
  _sx.push_back(sx);
  _sy.push_back(sy);
  _sz.push_back(sz);
  
  _vx.push_back(vx/abs_v);
  _vy.push_back(vy/abs_v);
  _vz.push_back(vz/abs_v);
  
  _time.push_back(t*1e3);
  
  _weight.push_back(p);
  
  _ekin.push_back(ekin);
  //  _userflag.push_back(userflag);
  ++_size;
}
