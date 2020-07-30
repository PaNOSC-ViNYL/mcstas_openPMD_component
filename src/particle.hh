#ifndef PARTICLE_CLASS_HH
#define PARTICLE_CLASS_HH

#include <array>
#include <cmath>
#include <memory> // shared_ptr
#include <vector>

class particles {
private:
	std::vector<float> _x, _y, _z,     // position
	    _sx, _sy, _sz,                 // polarization
	    _vx, _vy, _vz,                 // direction (vx^2+vy^2+vz^2) = 1
	    _abs_v, _time, _weight, _ekin; // velocity, time, weight, kinetic energy
	std::vector<uint32_t> _userflag;
	size_t _size;

public:
	particles();

	void add(double x, double y, double z,    //
		 double sx, double sy, double sz, //
		 double vx, double vy, double vz, //
		 double t, double p, uint32_t userflag = 0);

	void clear(void){
		_size = 0;
		_x.clear();
		_y.clear();
		_z.clear();
		_vx.clear();
		_vy.clear();
		_vz.clear();
		_sx.clear();
		_sy.clear();
		_sz.clear();
		_time.clear();
		_ekin.clear();
		_weight.clear();
	};
	size_t size() const { return _size; };

	std::vector<float> x(void) const { return _x; };
	std::vector<float> y(void) const { return _y; };
	std::vector<float> z(void) const { return _z; };
	std::vector<float> vx(void) const { return _vx; };
	std::vector<float> vy(void) const { return _vy; };
	std::vector<float> vz(void) const { return _vz; };
	std::vector<float> sx(void) const { return _sx; };
	std::vector<float> sy(void) const { return _sy; };
	std::vector<float> sz(void) const { return _sz; };
	std::vector<float> time(void) const { return _time; };
	std::vector<float> ekin(void) const { return _ekin; };
	std::vector<float> weight(void) const { return _weight; };
};

#endif
