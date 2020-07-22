#include "component.hh"
#include <iostream>

component::component(const std::string &name) : _name(name) {}

openPMD_io::openPMD_io(const std::string &name)
    : component(name), _series(nullptr){};

/** n_neutrons is the ncount of McStas, so the number of neutrons to be simulated */
void openPMD_io::init_neutrons(unsigned int iter, unsigned long long int n_neutrons) {
  auto i = _series->iterations[iter];
  // series.setAttribute("dinner", "Pizza and Coke");
  i.setAttribute("vacuum", true);

  openPMD::ParticleSpecies neutrons = i.particles["neutrons"];
  neutrons.setAttribute("speciesType", "2112");
  // declare the dataset total size: the final size
  openPMD::Dataset dataset =
      openPMD::Dataset(openPMD::Datatype::FLOAT, openPMD::Extent{n_neutrons});

  // now define the RECORD
  neutrons["position"].setUnitDimension({{openPMD::UnitDimension::L, 1.}}); //init std::map

  // now define the RECORD_COMPONENT
  neutrons["position"]["x"].resetDataset(dataset);
  neutrons["position"]["x"].setUnitSI(1e-2);
  
}

void openPMD_io::init(const char *extension, unsigned int iter, unsigned long long int n_neutrons) {
  std::string filename = "data_%05T.";
  filename += extension;

  // assign the global variable to keep track of it
  _series = std::unique_ptr<openPMD::Series>(
      new openPMD::Series(filename, openPMD::AccessType::CREATE));
  auto series = *_series;

  series.setAuthor("openPMD output component");
  std::cout << "Filename: " << filename << std::endl; // remove

  init_neutrons(iter, n_neutrons);
  // openPMD::Record mass = neutrons["mass"];
  // openPMD::RecordComponent mass_scalar =
  // mass[openPMD::RecordComponent::SCALAR];

  // mass_scalar.resetDataset(dataset);

  // // neutrons["position"]["y"].resetDataset(dataset);
  // // neutrons["position"]["z"].resetDataset(dataset);

  // series.flush();
}

void openPMD_io::trace(double x, double y, double z, double sx, double sy,
                       double sz, double vx, double vy, double vz, double t,
                       double p) {
  //_neutrons.emplace_back(particle(x, y, z, sx, sy, sz, vx, vy, vz, t, p));
  _neutrons.add(x, y, z, sx, sy, sz, vx, vy, vz, t, p);
};

void openPMD_io::save(void) {
  std::cout << "Number of saved neutrons: " << _neutrons.size() << std::endl;

    // // std::vector<float> positions = {0,0,0};
  //std::shared_ptr< std::vector<float> > _p( &positions);
  // std::shared_ptr<float> f(new float);
  // *f = 5.;
  // neutrons["position"]["x"].storeChunk(f, openPMD::Extent{0},
  //                                      openPMD::Extent{1});
  // series.flush();
  // neutrons["position"]["x"].storeChunk(f, openPMD::Extent{1},
  //                                      openPMD::Extent{1});

  //_series->flush(); // this crashes
}
