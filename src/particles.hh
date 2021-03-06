#ifndef PARTICLE_CLASS_HH
#define PARTICLE_CLASS_HH
///\file
#include <vector>

/** \class particles
 * \brief stores the neutron quantities
 */
class particles {
private:
	std::vector<float> _x, _y, _z,         // position
	        _sx, _sy, _sz,                 // polarization
	        _vx, _vy, _vz,                 // direction (vx^2+vy^2+vz^2) = 1
	        _abs_v, _time, _weight, _ekin; // velocity, time, weight, kinetic energy
	//	std::vector<uint32_t> _userflag;
	size_t _size; // number of stored neutrons
	size_t _read; // current index when reading

public:
	/// \brief default constructor
	particles();

	/** \brief append a new neutron from McStas (\b doing \b conversions)
	 * \param[in] x,y,z : neutron position          -> it is converted to cm ( * 100)
	 * \param[in] sx, sy, sz : neutron polarization
	 * \param[in] vx, vy, vz : neutron velocity     -> only the direction is stored
	 * \param[in] t : time
	 * \param[in] p : weight
	 */
	void
	push_back(double x, double y, double z,    // position
	          double sx, double sy, double sz, // polarization
	          double vx, double vy, double vz, // velocity
	          double t, double p);             // time and weight //, uint32_t userflag = 0);

	/** \brief append a new neutron from *openPMD data*
     * \param[in] x,y,z : neutron position in cm
     * \param[in] sx, sy, sz : neutron polarization (not working yet)
     * \param[in] dx, dy, dz : neutron direction
     * \param[in] time : time in ms
     * \param[in] weight : weight
     * \param[in] ekin : kinetic energy
     */
	void
	store(double x, double y, double z,    //  position
	      double sx, double sy, double sz, //  polarization
	      double dx, double dy, double dz, //  velocity
	      double time, double weight,      //  time and weight
	      double ekin);                    //  kinetic energy

	/** \brief read a new neutron from openPMD data
	 * It increaments the reader count
     * \param[out] x,y,z : neutron position in m
     * \param[out] sx, sy, sz : neutron polarization (not working yet)
     * \param[out] vx, vy, vz : neutron velocity in m/s
     * \param[out] t : time in s
     * \param[out] p : weight
     */
	void
	retrieve_next(double* x, double* y, double* z,    // position
	              double* sx, double* sy, double* sz, // polarization
	              double* vx, double* vy, double* vz, // velocity
	              double* t, double* p) { // time and weight //, uint32_t userflag = 0);)
		retrieve(x, y, z, sx, sy, sz, vx, vy, vz, t, p);
		++_read;
	}

	void
	retrieve(double* x, double* y, double* z,    // position
	         double* sx, double* sy, double* sz, // polarization
	         double* vx, double* vy, double* vz, // velocity
	         double* t, double* p);              // time and weight //, uint32_t userflag = 0);)

	/** \brief reset the container, removing all the neutrons */
	void
	clear(void) {
		_size = 0;
		_read = 0;
		_x.clear();
		_y.clear();
		_z.clear();
		_vx.clear();
		_vy.clear();
		_vz.clear();
		_sx.clear();
		_sy.clear();
		_sz.clear();
		_time.clear();
		_ekin.clear();
		_weight.clear();
	};

	/** \brief returns the number of stored neutrons */
	size_t
	size() const {
		return _size;
	};

	/** \brief check if all the data have been already retrieved
	 * \return bool : true if all the data stored have been retrieved
	 * it return true also if it is empty
	 */
	bool
	is_chunk_finished(void) {
		return _read == _size;
	}

	// clang-format off
	/** \name 1D vector quantities of stored neutrons
| Variable | Comment        | Units                  |
| ---------|----------------|------------------------|
| x,y,z    | position in    | [cm]		     |
| vx,vy,vz | direction      | (normalized velocity)  |
| sx,sy,sz | polarization   | 			     |
| time     |                | [ms]		     |
| ekin     | kinetic energy | [MeV]		     |
| weight   | weight         |                        |
 *
	 */
	// clang-format on
	const std::vector<float>&
	x(void) const {
		return _x;
	};
	const std::vector<float>&
	y(void) const {
		return _y;
	};
	const std::vector<float>&
	z(void) const {
		return _z;
	};
	const std::vector<float>&
	vx(void) const {
		return _vx;
	};
	const std::vector<float>&
	vy(void) const {
		return _vy;
	};
	const std::vector<float>&
	vz(void) const {
		return _vz;
	};
	const std::vector<float>&
	sx(void) const {
		return _sx;
	};
	const std::vector<float>&
	sy(void) const {
		return _sy;
	};
	const std::vector<float>&
	sz(void) const {
		return _sz;
	};
	const std::vector<float>&
	time(void) const {
		return _time;
	};
	const std::vector<float>&
	ekin(void) const {
		return _ekin;
	};
	const std::vector<float>&
	weight(void) const {
		return _weight;
	};
	///@}
	};

#endif
