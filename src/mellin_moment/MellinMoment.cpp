#include "../../include/mellin_moment/MellinMoment.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/parton_distribution/GluonDistribution.h>
#include <partons/beans/parton_distribution/PartonDistribution.h>
#include <partons/beans/parton_distribution/QuarkDistribution.h>
#include <partons/BaseObjectRegistry.h>
#include <cmath>
#include <iostream>
#include <string>

namespace PARTONS {

const unsigned int MellinMoment::classId = BaseObjectRegistry::getInstance()->registerBaseObject(new MellinMoment("MellinMomentModule"));

const std::string MellinMoment::MELLIN_MOMENT_MODULE_CLASS_NAME =
		"MellinMomentModule";
const std::string MellinMoment::MELLIN_MOMENT_MODULE_GPD_TYPE =
		"GPD_MODULE_GPD_TYPE";

MellinMoment::MellinMoment(const std::string &className) :
		ModuleObject(className) {
	m_n = 0;
	m_gluon = false;
	m_pGPDModel = 0;
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
	debug(__func__, ElemUtils::Formatter() << "executed");
}
void MellinMoment::initModule() {
	debug(__func__, ElemUtils::Formatter() << "executed");
}

GluonValue MellinMoment::computeGluonValue(int n,
		MellinMomentKinematic mKinematic, GPDModule* pGPDModel, GPDType mGPDtype) {
	m_type = mGPDtype;
	m_gluon = true;
	GluonValue result;

	result.setGluonValue(MellinMoment::compute(n, mKinematic, pGPDModel));

	return result;
}

QuarkValue MellinMoment::computeQuarkValue(int n,
		MellinMomentKinematic mKinematic, GPDModule* pGPDModel, GPDType mGPDtype,
		QuarkFlavor mflavor) {
	m_type = mGPDtype;
	m_gluon = false;
	m_flavor = mflavor;
	QuarkValue result;

	result.setQuarkFlavor(mflavor);
	result.setQuarkValue(MellinMoment::compute(n, mKinematic, pGPDModel));

	return result;
}

PartonValues MellinMoment::compute(int n, MellinMomentKinematic mKinematic,
		GPDModule* pGPDModel, GPDType mGPDtype) {
	PartonValues result;

	GluonValue gluon = MellinMoment::computeGluonValue(n, mKinematic, pGPDModel, mGPDtype);
	result.setGluonValue(gluon);

	List<QuarkFlavor> flavorList = MellinMoment::getQuarkFlavorList(mKinematic, pGPDModel, mGPDtype);

	for (unsigned int i = 0; i != flavorList.size(); i++) {
		result.addQuarkValue(MellinMoment::computeQuarkValue(n, mKinematic, pGPDModel, mGPDtype,flavorList[i]));
	}

	return result;

}

double MellinMoment::compute(int n, MellinMomentKinematic mKinematic,
		GPDModule* pGPDModel) ///< Compute when everything is set.
		{
	m_n = n;
	m_pGPDModel = pGPDModel;

	std::vector<double> parameters;
	parameters.push_back(mKinematic.getXi());
	parameters.push_back(mKinematic.getT());
	parameters.push_back(mKinematic.getMuF2());
	parameters.push_back(mKinematic.getMuR2());

	return integrate(m_pint, -1.0, 1.0, parameters);

}

double MellinMoment::integrant(double x, std::vector<double> par) ///< Integrand
		{

	// Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
	GPDKinematic gpdKinematic(x, par[0], par[1], par[2], par[3]);

	if (m_gluon) {
		return pow(x, m_n - 2)
				* m_pGPDModel->compute(gpdKinematic, m_type).getGluonDistribution().getGluonDistribution();
	} else {
		return pow(x, m_n - 1)
				* m_pGPDModel->compute(gpdKinematic, m_type).getQuarkDistribution(
						m_flavor).getQuarkDistribution();
	}
}

List<QuarkFlavor> MellinMoment::getQuarkFlavorList(
		MellinMomentKinematic mKinematic, GPDModule* pGPDModule,
		const GPDType &gpdType) {

	List<QuarkFlavor> result;
	List<GPDType> gpdTypeList =
			pGPDModule->getListOfAvailableGPDTypeForComputation();
	bool isGPDType = false;

	for (unsigned int i = 0; i != gpdTypeList.size(); i++) {
		if (gpdTypeList[i].getType() == gpdType.getType())
			isGPDType = true;
	}

	if (isGPDType) {
		GPDKinematic kinematic(1, mKinematic.getXi, mKinematic.getT,
				mKinematic.getMuF2, mKinematic.getMuR2);
		List<QuarkDistribution> list =
				pGPDModule->compute(kinematic, gpdType).getListOfQuarkDistribution();
		for (unsigned int i = 0; i != list.size(); i++) {
			QuarkFlavor flavor = list[i].getQuarkFlavor();
			result.add(flavor);
		}
	}

	return result;
}

} /* namespace PARTONS */
