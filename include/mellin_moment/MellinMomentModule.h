#ifndef MELLIN_MOMENT_MODULE_H
#define MELLIN_MOMENT_MODULE_H

/**
 * @file MellinMomentModule.h
 * @author: Arkadiusz P. Trawiński (DPhN/CEA-Saclay & CPHT/École polytechnique)
 * @date 22 March 2019
 * @version 1.0
 */

#include <partons/beans/gpd/GPDType.h>
#include <partons/modules/gpd/GPDModule.h>
#include <partons/beans/List.h>
#include <partons/beans/QuarkFlavor.h>
#include <partons/modules/MathIntegratorModule.h>
#include <partons/ModuleObject.h>
#include <vector>

#include "MellinMomentKinematic.h"
#include "../parton_value/QuarkValue.h"
#include "../parton_value/GluonValue.h"
#include "../parton_value/PartonValues.h"

namespace PARTONS {

/**
 * @class MellinMomentModule
 *
 * @brief Abstract class representing single computation of Mellin Moment.
 *
 */

class MellinMomentModule: public ModuleObject {
public:

	static const std::string MELLIN_MOMENT_MODULE_CLASS_NAME; ///< Type of the module in XML automation.
	static const std::string PARAMETER_NAME_MELLIN_MOMENT_MODULE_N; ///< Index of the Mellin Model in XML automation.


	/**
	 * ID assigned by BaseObjectRegistry.
	 */
	static const unsigned int classId;
	/**
	 * Default constructor.
	 * @param className Name of class.
	 */
	MellinMomentModule(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~MellinMomentModule();

	virtual MellinMomentModule* clone() const = 0;

	virtual void configure(const ElemUtils::Parameters &parameters);

	/*
	 * @param n index of Mellin Moment.
	 * @param mKinematic Mellin Moment kinematic.
	 * @param mtype Type of GPD to compute.
	 * @return Result of \f$n\f$-th Mellin Moment computation for all avaible quark flavors and gluon.
	 */

	PartonValues computeAll(MellinMomentKinematic mKinematic,GPDType mGPDtype);

	/*
	 * @param n index of Mellin Moment.
	 * @param mKinematic Mellin Moment kinematic.
	 * @param mtype Type of GPD to compute.
	 * @return \f$n\f$-th gluon Mellin Moment, where \f$n\f$-th means \f$f_n^g = \int dx x^{n-2} f^g(x) \f$-th
	 */
	GluonValue computeGluonValue(MellinMomentKinematic mKinematic,GPDType mtype);

	/*
	 * @param mKinematic Mellin Moment kinematic.
	 * @param mflavor Flavor of quark.
	 * @return \f$n\f$-th quark Mellin Moment of flavor \f$mflavor\f$, where \f$n\f$-th means \f$f_n^1 = \int dx x^{n-1} f^g(x) \f$-th
	 */
	QuarkValue computeQuarkValue(MellinMomentKinematic mKinematic,GPDType mtype, QuarkFlavor mflavor);

	int getN();
	void setN(int n);

	virtual List<QuarkFlavor> getListOfAvailableQuarkFlavor(MellinMomentKinematic mKinematic, const GPDType &gpdType) = 0;

	virtual List<GPDType> getListOfAvailableGPDType() = 0;

protected:
	virtual void isModuleWellConfigured();
	virtual void initModule();
	MellinMomentModule(const MellinMomentModule &other);

	int m_n; ///< Mellin Moment index
	bool m_gluon; ///< If compute for gluon.
	QuarkFlavor m_flavor; ///< For witch quark flavor compute.
	GPDType m_type; ///< Witch type of GPD compute.



private:
	virtual double compute(MellinMomentKinematic mKinematic) = 0;

}
;

} /* namespace PARTONS */

#endif /* MELLIN_MOMENT_MODULE_H */
