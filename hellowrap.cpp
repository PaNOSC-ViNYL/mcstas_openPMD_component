#include <cstdlib>

#include "hellowrap.h"
#include "component.hh"

#ifdef shervin
void position(openPMD::ParticleSpecies &particle,
              unsigned long long int num_particles) {
  // this represents the final size of the dataset
  openPMD::Extent dataset_size = {num_particles};
  // declare the dataset total size: the final size
  openPMD::Dataset dataset =
      openPMD::Dataset(openPMD::Datatype::FLOAT, dataset_size);
  // dataset.extend(newExtent)
  // dataset.setChunkSize
  particle["position"]["x"].resetDataset(dataset);

  std::vector < float std::shared_ptr<float> f(new float);
  *f = 5.;
  particle["position"]["x"].storeChunk(f, openPMD::Extent{0},
                                       openPMD::Extent{1});
  series.flush();
  particle["position"]["x"].storeChunk(f, openPMD::Extent{1},
                                       openPMD::Extent{1});
  series.flush();
}
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

    openPMD_io* newOpenPMD_io(const char* name){
	return new openPMD_io(name);
    }
    
    void init(openPMD_io* op, const char *extension, unsigned int iter, unsigned long long int n_neutrons){
	op->init(extension, iter, n_neutrons);
    }

    void trace(openPMD_io* op, double x, double y, double z, double sx, double sy,
	   double sz, double vx, double vy, double vz, double t,
	   double p) {
	op->trace(x,y,z,
	  sx,sy,sz,
	  vx,vy,vz,
	  t, p); //, userflag);
 
};

    void save(openPMD_io *op){
	op->save();
    }
    
int sayHi(char *name) { return 0; }

#ifdef __cplusplus
}
#endif
