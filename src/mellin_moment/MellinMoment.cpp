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

#include "../../include/parton_value/GluonValue.h"
#include "../../include/parton_value/PartonValues.h"
#include "../../include/parton_value/QuarkValue.h"

namespace PARTONS {
class GluonValue;
class QuarkValue;
} /* namespace PARTONS */

namespace PARTONS {

MellinMoment::MellinMoment() {
	m_pint = NumA::Integrator1D::newIntegrationFunctor(this,
			&MellinMoment::integrant);

	NumA::IntegratorType1D::Type integratorType = NumA::IntegratorType1D::DEXP;

	setIntegrator(integratorType);
}

MellinMoment::MellinMoment(const MellinMoment& other) :
		ModuleObject(other), MathIntegratorModule(other) {

	m_n = other.m_n;
	m_gluon = other.m_gluon;
	m_flavor = other.m_flavor;
	m_type = other.m_type;
	m_pGPDModel = 0;

	m_pint = NumA::Integrator1D::newIntegrationFunctor(this,
			&MellinMoment::integrant);

}

MellinMoment* MellinMoment::clone() const {
	return new MellinMoment(*this);
}

void MellinMoment::isModuleWellConfigured() {
	ModuleObject::isModuleWellConfigured();
}
void MellinMoment::initModule() {
	ModuleObject::initModule();
}

GluonValue MellinMoment::computeGluonValue(int n,
		MellinMomentKinematic mKinematic, GPDModule* pGPDModel, GPDType mtype) {
	m_type = mtype;
	m_gluon = true;
	GluonValue result;

	result.setGluonValue(MellinMoment::compute(n, mKinematic, pGPDModel));

	return result;
}

QuarkValue MellinMoment::computeQuarkValue(int n,
		MellinMomentKinematic mKinematic, GPDModule* pGPDModel, GPDType mtype,
		QuarkFlavor mflavor) {
	m_type = mtype;
	m_gluon = false;
	m_flavor = mflavor;
	QuarkValue result;

	result.setQuarkFlavor(mflavor);
	result.setQuarkValue(MellinMoment::compute(n, mKinematic, pGPDModel));

	return result;
}

PartonValues compute(int n, MellinMomentKinematic mKinematic,
		GPDModule* pGPDModel, GPDType mtype) {
	PartonValues result;

	GluonValue gluon = computeGluonValue(n, mKinematic, pGPDModel, mtype);
	result.setGluonValue(gluon);

	//TODO loop over all flavor

	return result;

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
