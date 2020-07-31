#ifndef MCSTAS_COMPONENT_BASE_HH
#define MCSTAS_COMPONENT_BASE_HH

#include "openpmd_output_formats.h"
#include "particle.hh"
#include <openPMD/openPMD.hpp>
#include <string>

#define ITER 1

/** \class openPMD_io
 * \brief I/O module for McStas to openPMD format using openPMD API
 */
class openPMD_io {
public:
	
	explicit openPMD_io(const std::string& name, openPMD::Access read_mode, std::string mc_code_name = "",
			    std::string mc_code_version = "", std::string instrument_name = "",
			    std::string name_current_component = "");

	/** initializes the "series" object from the openPMD API
	 *  \todo extension as ENUM of the accepted formats
	 *
	 * \param[in] n_neutrons : number of neutrons to be simulated as
	 * indicated on the command line of the McStas program */
	void init(openPMD_output_format_t extension, unsigned int iter, unsigned long long int n_neutrons);

	/** save neutron properties in a vector and dump it to the file in chunks */
	void trace(double x, double y, double z,    //
		   double sx, double sy, double sz, //
		   double vx, double vy, double vz, //
		   double t, double p);

	/** flushes the output to file before closing it */
	void save(void);

	//  std::shared_ptr<std::vector<float> > x(void) const;

private:
	std::string _name;
	std::string _mc_code_name;
	std::string _mc_code_version;
	std::string _instrument_name;
	std::string _name_current_component;

	openPMD::Access _access_mode;
	bool _isWriteMode;
	std::unique_ptr<openPMD::Series> _series;
	particles _neutrons;

	static const std::map<openPMD_output_format_t, std::string> output_format_names;
	/** declare the neutron particle species in the file */
	void init_neutrons(unsigned int iter, unsigned long long int n_neutrons);

	inline openPMD::Iteration& iter_pmd(void) { return _series->iterations[ITER]; }

	inline openPMD::ParticleSpecies& neutrons_pmd(void) {
		auto i = iter_pmd();
		return i.particles["neutron"];
	}

	void init_neutron_prop(
	    std::string name, openPMD::Dataset& dataset, bool isScalar,
	    std::map<openPMD::UnitDimension, double> const& dims = {{openPMD::UnitDimension::L, 0.}},
	    double unitSI                                        = 0.);
};

#endif
