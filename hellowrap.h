#ifndef HELLOWRAP_H
#define HELLOWRAP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct openPMD_io openPMD_io;

openPMD_io* newOpenPMD_io(const char* name);

  // init the series
  void init(openPMD_io* op, const char *extension, unsigned int iter, unsigned long long int n_neutrons);

  void trace(openPMD_io* op, double x, double y, double z, double sx, double sy,
	   double sz, double vx, double vy, double vz, double t,
	   double p);
  
  
  void save(openPMD_io* op);
  int sayHi(char *name);


#ifdef __cplusplus
}
#endif

#endif
