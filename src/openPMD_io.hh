#ifndef MCSTAS_COMPONENT_BASE_HH
#define MCSTAS_COMPONENT_BASE_HH
///\file
#include "openpmd_output_formats.h" // enum with the available openPMD backends
#include "particle.hh"              // helper class that stores the neutrons and returns 1D vectors
#include <openPMD/openPMD.hpp>      // openPMD C++ API
#include <string>

#define ITER 1

/** \class openPMD_io
 * \brief I/O module for McStas to openPMD format using openPMD API
 */
class openPMD_io {
public:
	///\brief constructor
	explicit openPMD_io(const std::string& filename,             ///< filename
			    openPMD::Access read_mode,               ///< file access mode (read/write/append)
			    std::string mc_code_name           = "", ///< McStas code name
			    std::string mc_code_version        = "", ///< McStas code version
			    std::string instrument_name        = "", ///< McStas instrument name
			    std::string name_current_component = ""  ///< name of the current component
	);

	/// \brief initializes the "series" object from the openPMD API in WRITE MODE
	void
	init(openPMD_output_format_t output_format, ///< output format
	     unsigned long long int n_neutrons,     ///< number of neutrons being simulated (max)
	     unsigned int iter = 1                  ///< openPMD iteration, always using the default value
	);

	/** \brief save neutron properties in a vector
	 *
	 * it writes to the file when the number of neutrons reaches CHUNK_SIZE as defined in the openPMD_io.cc file 
	 * \param[in] x,y,z : neutron position          -> it is converted to cm ( * 100)
	 * \param[in] sx, sy, sz : neutron polarization
	 * \param[in] vx, vy, vz : neutron velocity     -> only the direction is stored
	 * \param[in] t : time
 	 * \param[in] p : weight
 	 */
	void
	trace(double x, double y, double z,    // position
	      double sx, double sy, double sz, // polarization
	      double vx, double vy, double vz, // velocity
	      double t, double p);

	/** Flushes the output to file before closing it */
	void
	save(void);

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
	void
	init_neutrons(unsigned int iter, unsigned long long int n_neutrons);

	inline openPMD::Iteration&
	iter_pmd(void) {
		return _series->iterations[ITER];
	}

	inline openPMD::ParticleSpecies&
	neutrons_pmd(void) {
		auto i = iter_pmd();
		return i.particles["neutron"];
	}

	void
	init_neutron_prop(std::string name, openPMD::Dataset& dataset, bool isScalar,
			  std::map<openPMD::UnitDimension, double> const& dims = {{openPMD::UnitDimension::L,
										   0.}},
			  double unitSI                                        = 0.);
};

#endif
