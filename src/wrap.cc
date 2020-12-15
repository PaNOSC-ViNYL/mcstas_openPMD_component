#include <cstdlib>
///\file
#include <openPMDraytrace/openPMD_io.hh>

#include "wrap.h"
constexpr double V2W = 3.956034e-07;
class mcstas_neutron : public raytracing::Ray {

public:
	mcstas_neutron(raytracing::Ray r): Ray(r){};
	mcstas_neutron(): Ray(){};

	void
	set_position(double x, double y, double z) {
		Ray::set_position(x, y, z);
	};
	void
	set_velocity(double x, double y, double z) {
		double abs_v = sqrt(x * x + y * y + z * z);
		set_direction(x, y, z, 1. / abs_v);
		set_wavelength(V2W/abs_v);
	}
	void
	get_velocity(double* x, double* y, double* z) {
		double abs_v = V2W / get_wavelength();
		get_direction(x, y, z, abs_v);
	}
};

#ifdef __cplusplus
extern "C" {
#endif

openPMD_io*
openPMD_create(const char* filename, const char* mc_code_name, const char* mc_code_version,
              const char* instrument_name, const char* name_current_component, int mpi_node_rank) {
	if(mpi_node_rank==1){
		raytracing::openPMD_io* io = new raytracing::openPMD_io(filename, mc_code_name, mc_code_version,
		                                  instrument_name, name_current_component);
		return io;
	}
	//	else return openPMD_append(filename);
}
        /*
openPMD_io*
openPMD_read(const char* filename, const char* mc_code_name, const char* mc_code_version,
             const char* instrument_name, const char* name_current_component, int repeat) {
	return new openPMD_io(filename, mc_code_name, mc_code_version,
	                      instrument_name, name_current_component, repeat);
}
	
openPMD_io*
openPMD_append(const char* name) {
	return new openPMD_io(name, openPMD::Access::READ_WRITE);
}
	*/
void
init_write(openPMD_io* op,// enum openPMD_output_format_t extension,
           unsigned long long int n_neutrons, unsigned int iter) {
	op->init_write("2112", n_neutrons, raytracing::AUTO, iter);
}

unsigned long long int
init_read(openPMD_io* op, //enum openPMD_output_format_t extension,
	  unsigned long long int n_neutrons, unsigned int iter, int repeat
) {
	return op->init_read("2112", iter, n_neutrons, repeat); 
}

void
trace_write(openPMD_io* op, double x, double y, double z, double sx, double sy, double sz,
            double vx, double vy, double vz, double t, double p) {
	mcstas_neutron r;
	r.set_position(x, y, z);
	r.set_velocity(vx, vy, vz);
	r.set_polarization(sx, sy, sz);
	r.set_time(t);
	r.set_weight(p);
	op->trace_write(r);
};

void
trace_read(openPMD_io* op, double* x, double* y, double* z, double* sx, double* sy, double* sz,
           double* vx, double* vy, double* vz, double* t, double* p) {

	mcstas_neutron r = op->trace_read();
	
	r.get_position(x, y, z);
	r.get_polarization(sx, sy, sz);
	r.get_velocity(vx, vy, vz);
	*t  = r.get_time();
	*p  = r.get_weight();
};

void
save_write(openPMD_io* op) {
	op->save_write();
}

#ifdef __cplusplus
}
#endif
