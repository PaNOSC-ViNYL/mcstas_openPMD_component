#include "component.hh"
#include <iostream>

/** \todo ekin should be saved according to the openPMD standard */
/** \todo use particlePatches ... but I don't understand if/how */
/** \todo
 * The CHUNK_SIZE should not be hardcoded
 * it should also be optimized with tests
 */
#define CHUNK_SIZE 10000
//------------------------------------------------------------
const std::map<openPMD_output_format_t, std::string> openPMD_io::output_format_names = {
    {JSON, "json"},
    {HDF5, "h5"},
};

//------------------------------------------------------------
openPMD_io::openPMD_io(const std::string& filename, openPMD::Access read_mode, std::string mc_code_name,
		       std::string mc_code_version, std::string instrument_name,
		       std::string name_current_component)
    : _name(filename), _mc_code_name(mc_code_name), _mc_code_version(mc_code_version),
      _instrument_name(instrument_name), _name_current_component(name_current_component),
      _access_mode(read_mode), _series(nullptr){};

//------------------------------------------------------------
/** \brief
 * \param[in] name : name of the field/property
 * \param[in] isScalar : bool indicating if it is scalar
 * \param[in] dims : Unit dimensions
 */
void openPMD_io::init_neutron_prop(std::string name, openPMD::Dataset& dataset, bool isScalar,
				   std::map<openPMD::UnitDimension, double> const& dims, double unitSI) {
	auto neutrons = neutrons_pmd();

	neutrons[name].setUnitDimension(dims);

	if (isScalar) {
		neutrons[name][openPMD::RecordComponent::SCALAR].resetDataset(dataset);
		neutrons[name][openPMD::RecordComponent::SCALAR].setUnitSI(unitSI);
	} else {
		// now define the RECORD_COMPONENT
		for (auto dim : {"x", "y", "z"}) {
			neutrons[name][dim].resetDataset(dataset);
			neutrons[name][dim].setUnitSI(unitSI);
		}
	}
}

//------------------------------------------------------------
/** n_neutrons is the ncount of McStas, so the number of neutrons to be
 * simulated */
void openPMD_io::init_neutrons(unsigned int iter, unsigned long long int n_neutrons) {

	// auto i = iter_pmd();
	// series.setAttribute("dinner", "Pizza and Coke");
	// i.setAttribute("vacuum", true);

	auto neutrons = neutrons_pmd();
	neutrons.setAttribute("speciesType", "2112");
	neutrons.setAttribute("PDGID", "2112");
	// declare the dataset total size: the final size

	openPMD::Dataset dataset = openPMD::Dataset(openPMD::Datatype::FLOAT, openPMD::Extent{n_neutrons});

	init_neutron_prop("position", dataset, false, {{openPMD::UnitDimension::L, 1.}}, 1e-2);
	init_neutron_prop("direction", dataset, false);
	// now set the scalars
	init_neutron_prop("weight", dataset, true);
	init_neutron_prop("time", dataset, true, {{openPMD::UnitDimension::T, 1.}}, 1e-3);
	init_neutron_prop("energy", dataset, true,
			  {{
			       openPMD::UnitDimension::M,
			       1,
			   },
			   {
			       openPMD::UnitDimension::L,
			       2,
			   },
			   {openPMD::UnitDimension::T, -2}},
			  1.6021766e-13); // MeV
}

void openPMD_io::init(openPMD_output_format_t extension, unsigned int iter,
		      unsigned long long int n_neutrons) {
	std::string filename = _name;
	std::string a        = output_format_names.find(extension)->second;
	filename += std::string(".") + a;

	// assign the global variable to keep track of it
	_series = std::unique_ptr<openPMD::Series>(new openPMD::Series(filename, openPMD::Access::CREATE));

	_series->setAuthor("openPMD output component");
	std::cout << "Filename: " << filename << std::endl; // remove

	// set the mccode, mccode_version, component name, instrument name

	init_neutrons(iter, n_neutrons);

	// openPMD::Record mass = neutrons["mass"];
	// openPMD::RecordComponent mass_scalar =
	// mass[openPMD::RecordComponent::SCALAR];

	// mass_scalar.resetDataset(dataset);

	_series->flush();
}

//------------------------------------------------------------
void openPMD_io::trace(double x, double y, double z, double sx, double sy, double sz, double vx, double vy,
		       double vz, double t, double p) {
	//_neutrons.emplace_back(particle(x, y, z, sx, sy, sz, vx, vy, vz, t, p));
	_neutrons.add(x, y, z, sx, sy, sz, vx, vy, vz, t, p);

	if (_neutrons.size() >= CHUNK_SIZE)
		save();
};

//------------------------------------------------------------
void openPMD_io::save(void) {
	if (_neutrons.size() == 0)
		return;
#ifdef DEBUG
	std::cout << "Number of saved neutrons: " << _neutrons.size() << "\t" << _neutrons.x().size()
		  << std::endl;
#endif
	auto neutrons          = neutrons_pmd();
	openPMD::Offset offset = {0};
	openPMD::Extent extent = {_neutrons.size()};
	neutrons["position"]["x"].storeChunk(openPMD::shareRaw(_neutrons.x()), offset, extent);
	neutrons["position"]["y"].storeChunk(openPMD::shareRaw(_neutrons.y()), offset, extent);
	neutrons["position"]["z"].storeChunk(openPMD::shareRaw(_neutrons.z()), offset, extent);

	neutrons["direction"]["x"].storeChunk(openPMD::shareRaw(_neutrons.vx()), offset, extent);
	neutrons["direction"]["y"].storeChunk(openPMD::shareRaw(_neutrons.vy()), offset, extent);
	neutrons["direction"]["z"].storeChunk(openPMD::shareRaw(_neutrons.vz()), offset, extent);

	neutrons["time"][openPMD::RecordComponent::SCALAR].storeChunk(openPMD::shareRaw(_neutrons.time()),
								      offset, extent);
	neutrons["energy"][openPMD::RecordComponent::SCALAR].storeChunk(openPMD::shareRaw(_neutrons.ekin()),
								      offset, extent);
	neutrons["weight"][openPMD::RecordComponent::SCALAR].storeChunk(openPMD::shareRaw(_neutrons.weight()),
									offset, extent);

	_series->flush(); // this crashes
	_neutrons.clear();
}
