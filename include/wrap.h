#ifndef OPENPMD_IO_WRAP_H
#define OPENPMD_IO_WRAP_H
///\file
#include "openpmd_output_formats.h"
#ifdef __cplusplus
extern "C" {
#endif

/* The openPMD C++ API is used by a class ("openPMD_io") dedicated to perform I/O of McStas
 * simulation in openPMD format.
 * This file provides a wrapper to the openPMD_io class in order to be used in McStas
 * as an external library.
 */
typedef struct openPMD_io openPMD_io;
/** declare an openPMD_io struct, defined from the openPMD_io class, with
 * default parameters to read from openPMD file */

/** declare an openPMD_io struct, with default parameters to write to openPMD
 * file */
openPMD_io*
openPMD_write(const char* filename, const char* mc_code_name, const char* mc_code_version,
	      const char* instrument_name, const char* name_current_component);

/** declare an openPMD_io struct, with default parameters to read from openPMD
 * file */
openPMD_io*
openPMD_read(const char* filename, const char* mc_code_name, const char* mc_code_version,
          const char* instrument_name, const char* name_current_component);

/* openPMD_io *openPMD_append(const char *name); */

/** wrapper of openPMD_io::init_write() method */
void
init_write(openPMD_io* op, enum openPMD_output_format_t extension, unsigned long long int n_neutrons
     //, unsigned int iter
);

/** wrapper of openPMD_io::trace_write() */
void
trace_write(openPMD_io* op, double x, double y, double z, double sx, double sy, double sz, double vx, double vy,
      double vz, double t, double p);
    
/** wrapper of openPMD_io::init_read() method */
void
init_read(openPMD_io* op, enum openPMD_output_format_t extension, unsigned long long int n_neutrons
     //, unsigned int iter
);

/** wrapper of openPMD_io::trace_read() */
void
trace_read(openPMD_io* op, double *x, double *y, double *z, double *sx, double *sy, double *sz, double *vx, double *vy,
      double *vz, double *t, double *p);

/** wrapper of openPMD_io::save() */
void
save(openPMD_io* op);

#ifdef __cplusplus
}
#endif

#endif
