#ifndef PARTICLE_CLASS_HH
#define PARTICLE_CLASS_HH

#include <array>
#include <cmath>
#include <vector>

#ifndef V2SE
#define VS2E 5.22703725e-6 /* Convert (v[m/s])**2 to E[meV] */
#endif


    /** \class particle 
     * \brief storing the particle properties
     */

class particle {

public:
  particle(double x, double y, double z, double sx, double sy, double sz,
           double vx, double vy, double vz, double t, double p,
           uint32_t userflag = 0)
      : _userflag(userflag) {
    setPosition(x, y, z);
    setPolarisation(sx, sy, sz);
    setDirection(vx, vy, vz);
    setTime(t);
    setWeight(p);
  }

  /// \brief set the neutron position in cm
  void setPosition(double x, double y, double z) {
    position[0] = x * 100;
    position[1] = y * 100;
    position[2] = z * 100;
  }
  /// set the neutron polarization
  void setPolarisation(double sx, double sy, double sz) {
    polarisation[0] = sx;
    polarisation[1] = sy;
    polarisation[2] = sz;
  }
  /// \brief set the neutron direction
  void setDirection(double vx, double vy, double vz) {
    abs_v = sqrt(vx * vx + vy * vy + vz * vz);
    direction[0] = vx / abs_v;
    direction[1] = vy / abs_v;
    direction[2] = vz / abs_v;
    setEKin();
  }
  /// \brief set the neutron time
  void setTime(double t) {
    /*time in ms:*/
    time = t * 1e3;
  }
  ///\brief set the weight assigned to the neutron in the simulation
  void setWeight(double p) {
    /*weight in unspecified units:*/
    weight = p;
  }

  void userflag(uint32_t userflag) { _userflag = std::move(userflag); }; // set
  const uint32_t &userflag() const { return _userflag; };                // get

private:
  std::array<float, 3> position, direction, polarisation;
  double abs_v, time, weight, ekin;
  uint32_t _userflag;
  /*ekin is in MeV*/
  void setEKin() { ekin = VS2E * abs_v * abs_v / 1e9; }

  // /*if specified also add the userflags*/
  // if(userflagenabled){
  //     particle->userflags = (uint32_t) userflag;
  // }
};


class particles{
private:
  std::vector<float> _x, _y, _z, // position
    _vx, _vy, _vz, // direction (vx^2+vy^2+vz^2) = 1
    _sx, _sy, _sz, // polarization
    _abs_v, _time, _weight, _ekin; //velocity, time, weight, kinetic energy
  std::vector<uint32_t> _userflag;

public:
  particles(){};

  void add(double x, double y, double z,
	   double sx, double sy, double sz,
           double vx, double vy, double vz,
	   double t, double p,
           uint32_t userflag = 0);
  size_t size()const{ return _x.size();};

};

#endif
