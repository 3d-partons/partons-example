#include "../../include/mellin_moment/MellinMomentFromGPD.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/functor/one_dimension/Functor1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <partons/beans/automation/BaseObjectData.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/parton_distribution/GluonDistribution.h>
#include <partons/beans/parton_distribution/PartonDistribution.h>
#include <partons/beans/parton_distribution/QuarkDistribution.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <cmath>
#include <map>
#include <string>


namespace PARTONS {

const unsigned int MellinMomentFromGPD::classId =
		BaseObjectRegistry::getInstance()->registerBaseObject(
				new MellinMomentFromGPD("MellinMomentFromGPD"));

const std::string MellinMomentFromGPD::MELLIN_MOMENT_MODULE_CLASS_NAME =
		"MellinMomentFromGPD";
const std::string MellinMomentFromGPD::MELLIN_MOMENT_MODULE_GPD_MODEL =
		"GPD_MODULE_GPD_MODEL";

MellinMomentFromGPD::MellinMomentFromGPD(const std::string &className) :
		MellinMomentModule(className) {
	m_pGPDModel = 0;
	m_pint = NumA::Integrator1D::newIntegrationFunctor(this,
			&MellinMomentFromGPD::integrant);

	initModule();
}

MellinMomentFromGPD::MellinMomentFromGPD(const MellinMomentFromGPD& other) :
		MellinMomentModule(other), MathIntegratorModule(other) {

	m_pGPDModel = other.m_pGPDModel;

	m_pint = NumA::Integrator1D::newIntegrationFunctor(this,
			&MellinMomentFromGPD::integrant);

	initModule();

}

MellinMomentFromGPD* MellinMomentFromGPD::clone() const {
	return new MellinMomentFromGPD(*this);
}

void MellinMomentFromGPD::isModuleWellConfigured() {
	MellinMomentModule::isModuleWellConfigured();

	if (m_pGPDModel == 0)
	throw ElemUtils::CustomException(getClassName(), __func__,
	                ElemUtils::Formatter()
	                        << "GPDModule not provided.");
}

void MellinMomentFromGPD::initModule() {
	MellinMomentModule::initModule();

	NumA::IntegratorType1D::Type integratorType = NumA::IntegratorType1D::DEXP;

	setIntegrator(integratorType);
}

void MellinMomentFromGPD::configure(const ElemUtils::Parameters &parameters) {
	MathIntegratorModule::configureIntegrator(parameters);
	MellinMomentModule::configure(parameters);
}

void MellinMomentFromGPD::prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData)
{

    ModuleObject::prepareSubModules(subModulesData);

    std::map<std::string, BaseObjectData>::const_iterator it;

    if (m_pGPDModel != 0) setGPDModule(0);

    it = subModulesData.find(
            GPDModule::GPD_MODULE_CLASS_NAME);

    if (it != subModulesData.end()) {

       	m_pGPDModel = Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                            (it->second).getModuleClassName());

		info(__func__,
				ElemUtils::Formatter() << "GPD Module is set to: "
						<< m_pGPDModel->getClassName());

		m_pGPDModel->configure((it->second).getParameters());
		m_pGPDModel->prepareSubModules(
                    (it->second).getSubModules());
    } else
    {
    	throw ElemUtils::CustomException(getClassName(), __func__,
    	                ElemUtils::Formatter()
    	                        << "GPDModule not provided.");
	}
}

double MellinMomentFromGPD::compute(MellinMomentKinematic mKinematic) ///< Compute when everything is set.
		{
	std::vector<double> parameters;
	parameters.push_back(mKinematic.getXi());
	parameters.push_back(mKinematic.getT());
	parameters.push_back(mKinematic.getMuF2());
	parameters.push_back(mKinematic.getMuR2());

	isModuleWellConfigured();

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

List<QuarkFlavor> MellinMomentFromGPD::getListOfAvailableQuarkFlavor(
		MellinMomentKinematic mKinematic, const GPDType &gpdType) {

	List<QuarkFlavor> result;
	List<GPDType> gpdTypeList = getListOfAvailableGPDType();
	bool isGPDType = false;


	for (unsigned int i = 0; i != gpdTypeList.size(); i++) {
		if (gpdTypeList[i].getType() == gpdType.getType())
			isGPDType = true;
	}

	if (isGPDType) {
		GPDKinematic kinematic(1., mKinematic.getXi(), mKinematic.getT(),
				mKinematic.getMuF2(), mKinematic.getMuR2());

		List<QuarkDistribution> list =
				m_pGPDModel->compute(kinematic, gpdType).getListOfQuarkDistribution();

		for (unsigned int i = 0; i != list.size(); i++) {
			QuarkFlavor flavor = list[i].getQuarkFlavor();
			result.add(flavor);
		}
	}

	return result;
}

List<GPDType> MellinMomentFromGPD::getListOfAvailableGPDType(){
	return m_pGPDModel -> getListOfAvailableGPDTypeForComputation();
}

GPDModule* MellinMomentFromGPD::getGPDModule() const {
	return m_pGPDModel;
}

void MellinMomentFromGPD::setGPDModule(GPDModule* pGPDModel) {
	m_pModuleObjectFactory->updateModulePointerReference(m_pGPDModel,
			pGPDModel);
	m_pGPDModel = pGPDModel;
}

} /* namespace PARTONS */
