#include "particle.hh"

void particles::add(double x, double y, double z,
		    double sx, double sy, double sz,
		    double vx, double vy, double vz,
		    double t, double p,
		    uint32_t userflag){
  _x.push_back(x);
  _y.push_back(y);
  _z.push_back(z);
  _sx.push_back(sx);
  _sy.push_back(sy);
  _sz.push_back(sz);
  _vx.push_back(vx);
  _vy.push_back(vy);
  _vz.push_back(vz);
  _time.push_back(t);
  _weight.push_back(p);
  _userflag.push_back(userflag);
  
}
