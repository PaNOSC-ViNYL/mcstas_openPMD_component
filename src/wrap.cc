#include <cstdlib>
///\file
#include "openPMD_io.hh"
#include "wrap.h"

#ifdef __cplusplus
extern "C" {
#endif

openPMD_io*
openPMD_write(const char* filename, const char* mc_code_name, const char* mc_code_version,
	      const char* instrument_name, const char* name_current_component) {
	return new openPMD_io(filename, openPMD::Access::CREATE, mc_code_name, mc_code_version,
			      instrument_name, name_current_component);
}

openPMD_io*
openPMD_read(const char* filename, const char* mc_code_name, const char* mc_code_version,
          const char* instrument_name, const char* name_current_component) {
    return new openPMD_io(filename, openPMD::Access::READ_ONLY, mc_code_name, mc_code_version,
                  instrument_name, name_current_component);
}

openPMD_io*
openPMD_append(const char* name) {
	return new openPMD_io(name, openPMD::Access::READ_WRITE);
}

void
init_write(openPMD_io* op, enum openPMD_output_format_t extension, unsigned long long int n_neutrons
     //, unsigned int iter
) {
	op->init_write(extension, n_neutrons); //, iter);
}

void
init_read(openPMD_io* op, enum openPMD_output_format_t extension, unsigned long long int n_neutrons
     //, unsigned int iter
) {
    op->init_read(extension, n_neutrons); //, iter);
}

void
trace_write(openPMD_io* op, double x, double y, double z, double sx, double sy, double sz, double vx, double vy,
      double vz, double t, double p) {
	op->trace_write(x, y, z, sx, sy, sz, vx, vy, vz, t, p); //, userflag);
};

void
trace_read(openPMD_io* op, double *x, double *y, double *z, double *sx, double *sy, double *sz, double *vx, double *vy,
      double *vz, double *t, double *p) {
    op->trace_read(x, y, z, sx, sy, sz, vx, vy, vz, t, p); //, userflag);
};

void
save_write(openPMD_io* op) {
	op->save_write();
}

#ifdef __cplusplus
}
#endif
