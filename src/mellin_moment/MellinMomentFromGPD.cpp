#include "../../include/mellin_moment/MellinMomentFromGPD.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/parton_distribution/GluonDistribution.h>
#include <partons/beans/parton_distribution/PartonDistribution.h>
#include <partons/beans/parton_distribution/QuarkDistribution.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/ModuleObjectFactory.h>
#include <cmath>
#include <string>


namespace PARTONS {

const unsigned int MellinMomentFromGPD::classId =
		BaseObjectRegistry::getInstance()->registerBaseObject(
				new MellinMomentFromGPD("MellinMomentFromGPD"));

const std::string MellinMomentFromGPD::MELLIN_MOMENT_MODULE_CLASS_NAME =
		"MellinMomentFromGPD";
const std::string MellinMomentFromGPD::MELLIN_MOMENT_MODULE_GPD_TYPE =
		"GPD_MODULE_GPD_TYPE";

MellinMomentFromGPD::MellinMomentFromGPD(const std::string &className) :
		MellinMomentModule(className) {
	m_pGPDModel = 0;
	m_pint = NumA::Integrator1D::newIntegrationFunctor(this,
			&MellinMomentFromGPD::integrant);

	NumA::IntegratorType1D::Type integratorType = NumA::IntegratorType1D::DEXP;

	setIntegrator(integratorType);
}

MellinMomentFromGPD::MellinMomentFromGPD(const MellinMomentFromGPD& other) :
		MellinMomentModule(other), MathIntegratorModule(other) {

	m_pGPDModel = 0;

	m_pint = NumA::Integrator1D::newIntegrationFunctor(this,
			&MellinMomentFromGPD::integrant);

}

MellinMomentFromGPD* MellinMomentFromGPD::clone() const {
	return new MellinMomentFromGPD(*this);
}

void MellinMomentFromGPD::isModuleWellConfigured() {
	debug(__func__, ElemUtils::Formatter() << "executed");
}
void MellinMomentFromGPD::initModule() {
	debug(__func__, ElemUtils::Formatter() << "executed");
}

void MellinMomentFromGPD::configure(const ElemUtils::Parameters &parameters) {
	MathIntegratorModule::configureIntegrator(parameters);
	MellinMomentModule::configure(parameters);
}

double MellinMomentFromGPD::compute(MellinMomentKinematic mKinematic) ///< Compute when everything is set.
		{
	std::vector<double> parameters;
	parameters.push_back(mKinematic.getXi());
	parameters.push_back(mKinematic.getT());
	parameters.push_back(mKinematic.getMuF2());
	parameters.push_back(mKinematic.getMuR2());

	return integrate(m_pint, -1.0, 1.0, parameters);

}

double MellinMomentFromGPD::integrant(double x, std::vector<double> par) ///< Integrand
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

List<QuarkFlavor> MellinMomentFromGPD::getQuarkFlavorList(
		MellinMomentKinematic mKinematic, const GPDType &gpdType) {

	List<QuarkFlavor> result;
	List<GPDType> gpdTypeList =
			m_pGPDModel->getListOfAvailableGPDTypeForComputation();
	bool isGPDType = false;

	for (unsigned int i = 0; i != gpdTypeList.size(); i++) {
		if (gpdTypeList[i].getType() == gpdType.getType())
			isGPDType = true;
	}

	if (isGPDType) {
		GPDKinematic kinematic(1, mKinematic.getXi, mKinematic.getT,
				mKinematic.getMuF2, mKinematic.getMuR2);
		List<QuarkDistribution> list =
				m_pGPDModel->compute(kinematic, gpdType).getListOfQuarkDistribution();
		for (unsigned int i = 0; i != list.size(); i++) {
			QuarkFlavor flavor = list[i].getQuarkFlavor();
			result.add(flavor);
		}
	}

	return result;
}

GPDModule* MellinMomentFromGPD::getGPDModule() const {
	return m_pGPDModel;
}

void MellinMomentFromGPD::setGPDModule(GPDModule* pGPDModel) {
	m_pModuleObjectFactory->updateModulePointerReference(m_pGPDModel,
			pGPDModel);
	m_pGPDModel = pGPDModel;

	if (m_pGPDModel != 0) {
		info(__func__,
				ElemUtils::Formatter() << "GPD Module is set to : "
						<< m_pGPDModel->getClassName());
	} else {
		info(__func__, "GPDModule is set to : 0");
	}
}

} /* namespace PARTONS */
