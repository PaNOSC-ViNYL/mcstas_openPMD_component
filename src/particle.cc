#include "particle.hh"
#include <array>
#include <cmath>
#include <iostream> // for debug
///\file
#ifndef V2SE
#define VS2E 5.22703725e-6 /* Convert (v[m/s])**2 to E[meV] */
#endif

particles::particles(): _size(0) {}

void
particles::push_back(double x, double y, double z, double sx, double sy, double sz, double vx, double vy,
                     double vz, double t,
                     double p) { //, uint32_t userflag) {
	double abs_v = sqrt(vx * vx + vy * vy + vz * vz);
	double ekin  = VS2E * abs_v * abs_v / 1e9;

	_x.push_back(x * 100.); //cm
	_y.push_back(y * 100.);
	_z.push_back(z * 100.);

	_sx.push_back(sx);
	_sy.push_back(sy);
	_sz.push_back(sz);

	_vx.push_back(vx / abs_v);
	_vy.push_back(vy / abs_v);
	_vz.push_back(vz / abs_v);

	_time.push_back(t * 1e3);

	_weight.push_back(p);

	_ekin.push_back(ekin);
	//  _userflag.push_back(userflag);
	++_size;
}

void
particles::store(double x, double y, double z, double sx, double sy, double sz, double dx, double dy,
                 double dz, double time, double weight, double ekin) {

	_x.push_back(x);
	_y.push_back(y);
	_z.push_back(z);

	_sx.push_back(sx);
	_sy.push_back(sy);
	_sz.push_back(sz);

	_vx.push_back(dx);
	_vy.push_back(dy);
	_vz.push_back(dz);

	_time.push_back(time);

	_weight.push_back(weight);

	_ekin.push_back(ekin);

	/*
    // The data is bonkers, z is usually largest as it should be, but completely wrong orders of magnitude
    std::cout << "x,y,z:" << x << "," << y << "," << z << std::endl;
    std::cout << "vx,vy,vz:" << dx << "," << dy << "," << dz << std::endl;
    */

	++_size;
}

void
particles::retrieve(double* x, double* y, double* z, double* sx, double* sy, double* sz, double* vx,
                    double* vy, double* vz, double* t,
                    double* p) { //, uint32_t userflag) {

	// Convert position from cm to m
	*x = _x[_read] / 100;
	*y = _y[_read] / 100;
	*z = _z[_read] / 100;

	// Polarization is in the correct format (not written by write component yet)
	*sx = _sx[_read];
	*sy = _sy[_read];
	*sz = _sz[_read];

	// Need energy to calculate magnitude of velocity
	double ekin = _ekin[_read];
	// Calculate velocity from energy
	double abs_v = sqrt(ekin * 1e9 / VS2E);

	// Use magnitude of velocity to convert direction to velocity
	*vx = _vx[_read] * abs_v;
	*vy = _vy[_read] * abs_v;
	*vz = _vz[_read] * abs_v;

	// Time from ms to s
	*t = _time[_read] / 1e3;

	// Weight is in the correct format
	*p = _weight[_read];

	++_read;

	if (_read >= _size)
		_read = 0; // Start over if entire dataset is read
}
