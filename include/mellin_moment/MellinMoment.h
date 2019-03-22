#ifndef MELLIN_MOMENT_H
#define MELLIN_MOMENT_H

/**
 * @file MellinMomentResult.h
 * @author: Arkadiusz P. Trawiński (DPhN/CEA-Saclay & CPHT/Ã‰cole polytechnique)
 * @date 22 March 2019
 * @version 1.0
 */

#include <partons/beans/gpd/GPDType.h>
#include <partons/modules/gpd/GPDModule.h>
#include <partons/beans/QuarkFlavor.h>
#include <partons/modules/MathIntegratorModule.h>
#include <vector>

#include "MellinMomentKinematic.h"

namespace PARTONS {

/**
 * @class MellinMoment
 *
 * @brief Class representing single computation of Mellin Moment.
 *
 * This class is used to compute Mellin Moment for a specific n, GPD type, GPD model and for both quarks and gluons.
 */

class MellinMoment: public MathIntegratorModule {
public:

	MellinMoment();
	/*
	 * @param n index of Mellin Moment.
	 * @param mKinematic Mellin Moment kinematic.
	 * @param pGPDModel GPD model to use.
	 * @param mtype Type of GPD to compute.
	 * @return \f$n\f$-th gluon Mellin Moment, where \f$n\f$-th means \f$f_n^g = \int dx x^{n-2} f^g(x) \f$-th
	 */
	double compute(int n, MellinMomentKinematic mKinematic,
			GPDModule* pGPDModel, GPDType mtype);

	/*
	 * @param n index of Mellin Moment.
	 * @param mKinematic Mellin Moment kinematic.
	 * @param pGPDModel GPD model to use.
	 * @param mtype Type of GPD to compute.
	 * @param mflavor Flavor of quark.
	 * @return \f$n\f$-th quark Mellin Moment of flavor \f$mflavor\f$, where \f$n\f$-th means \f$f_n^1 = \int dx x^{n-1} f^g(x) \f$-th
	 */
	double compute(int n, MellinMomentKinematic mKinematic,
			GPDModule* pGPDModel, GPDType mtype,
			QuarkFlavor mflavor);

private:
	int m_n=0; ///< Mellin Moment index
	bool m_gluon = false; ///< If compute for gluon.
	QuarkFlavor m_flavor = QuarkFlavor::UNDEFINED; ///< For witch quark flavor compute.
	GPDType m_type = GPDType::UNDEFINED; ///< Witch type of GPD compute.
	GPDModule* m_pGPDModel = 0; ///< Pointer to GPD model to use.
	NumA::FunctionType1D* m_pint; ///< Functor related to integrant.

	double compute(int n, MellinMomentKinematic mKinematic,
			GPDModule* pGPDModel);

	double integrant(double x, std::vector<double> par);
}
;

} /* namespace PARTONS */

#endif /* MELLIN_MOMENT_H */
