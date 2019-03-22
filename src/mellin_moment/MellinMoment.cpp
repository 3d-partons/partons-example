#include "../../include/mellin_moment/MellinMoment.h"

#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/parton_distribution/GluonDistribution.h>
#include <partons/beans/parton_distribution/PartonDistribution.h>
#include <partons/beans/parton_distribution/QuarkDistribution.h>
#include <partons/modules/gpd/GPDModule.h>
#include <cmath>

namespace PARTONS {

MellinMoment::MellinMoment() {
		m_pint = NumA::Integrator1D::newIntegrationFunctor(this,
				&MellinMoment::integrant);

		NumA::IntegratorType1D::Type integratorType =
				NumA::IntegratorType1D::DEXP;

		setIntegrator(integratorType);
	}

	/*
	 * @param n index of Mellin Moment.
	 * @param mKinematic Mellin Moment kinematic.
     * @param pGPDModel GPD model to use.
     * @param mtype Type of GPD to compute.
     * @return \f$n\f$-th gluon Mellin Moment, where \f$n\f$-th means \f$f_n^g = \int dx x^{n-2} f^g(x) \f$-th
	 */
	double MellinMoment::compute(int n, MellinMomentKinematic mKinematic,
			GPDModule* pGPDModel, GPDType mtype) {
		m_type = mtype;
		m_gluon = true;

		return MellinMoment::compute(n, mKinematic, pGPDModel);
	}

	/*
	 * @param n index of Mellin Moment.
	 * @param mKinematic Mellin Moment kinematic.
     * @param pGPDModel GPD model to use.
     * @param mtype Type of GPD to compute.
     * @param mflavor Flavor of quark.
     * @return \f$n\f$-th quark Mellin Moment of flavor \f$mflavor\f$, where \f$n\f$-th means \f$f_n^1 = \int dx x^{n-1} f^g(x) \f$-th
	 */
	double MellinMoment::compute(int n, MellinMomentKinematic mKinematic,
			GPDModule* pGPDModel, GPDType mtype,
			QuarkFlavor mflavor) {
		m_type = mtype;
		m_gluon = false;
		m_flavor = mflavor;

		return MellinMoment::compute(n, mKinematic, pGPDModel);
	}


	double MellinMoment::compute(int n, MellinMomentKinematic mKinematic,
			GPDModule* pGPDModel) ///< Compute when everything is set.
	{
		m_n = n;
		m_pGPDModel = pGPDModel;

		std::vector<double> parameters;
		parameters.push_back(mKinematic.getT());
		parameters.push_back(mKinematic.getMuF2());
		parameters.push_back(mKinematic.getMuR2());

		return integrate(m_pint, -1.0, 1.0, parameters);

	}

	double MellinMoment::integrant(double x, std::vector<double> par) ///< Integrand
			{

		// Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
		GPDKinematic gpdKinematic(x, 0.1, par[0], par[1], par[2]);

		if (m_gluon) {
			return pow(x, m_n - 2)
					* m_pGPDModel->compute(gpdKinematic, m_type).getGluonDistribution().getGluonDistribution();
		} else {
			return pow(x, m_n - 1)
					* m_pGPDModel->compute(gpdKinematic, m_type).getQuarkDistribution(
							m_flavor).getQuarkDistribution();
		}
	}


} /* namespace PARTONS */
