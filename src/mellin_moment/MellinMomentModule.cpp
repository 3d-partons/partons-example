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
#include "../../include/mellin_moment/MellinMomentModule.h"

namespace PARTONS {

const std::string MellinMomentModule::MELLIN_MOMENT_MODULE_CLASS_NAME =
		"MellinMomentModule";

const std::string MellinMomentModule::PARAMETER_NAME_MELLIN_MOMENT_MODULE_N = "MELLIN_INDEX_N";

MellinMomentModule::MellinMomentModule(const std::string &className) :
		ModuleObject(className) {
	m_n = 0;
	m_gluon = false;

	initModule();
}

MellinMomentModule::MellinMomentModule(const MellinMomentModule& other) :
		ModuleObject(other) {

	m_n = other.m_n;
	m_gluon = other.m_gluon;
	m_flavor = other.m_flavor;
	m_type = other.m_type;

	initModule();

}

MellinMomentModule::~MellinMomentModule(){

}

void MellinMomentModule::isModuleWellConfigured() {
	debug(__func__, ElemUtils::Formatter() << "executed");
}
void MellinMomentModule::initModule() {
	debug(__func__, ElemUtils::Formatter() << "executed");
}

void MellinMomentModule::configure(const ElemUtils::Parameters &parameters) {

    if (parameters.isAvailable(MellinMomentModule::PARAMETER_NAME_MELLIN_MOMENT_MODULE_N)) {

        m_n = parameters.getLastAvailable().toDouble();
        info(__func__,
                ElemUtils::Formatter() << "Parameter "
                        << MellinMomentModule::PARAMETER_NAME_MELLIN_MOMENT_MODULE_N << " changed to "
                        << m_n);
    }
}

PartonValues MellinMomentModule::computeAll(MellinMomentKinematic mKinematic, GPDType mGPDtype) {
	PartonValues result;

	GluonValue gluon = computeGluonValue(mKinematic,mGPDtype);
	result.setGluonValue(gluon);

	List<QuarkFlavor> flavorList = getListOfAvailableQuarkFlavor(mKinematic, mGPDtype);

	for (unsigned int i = 0; i != flavorList.size(); i++) {
		result.addQuarkValue(computeQuarkValue(mKinematic, mGPDtype,
						flavorList[i]));
	}

	return result;

}

GluonValue MellinMomentModule::computeGluonValue(MellinMomentKinematic mKinematic, GPDType mGPDtype) {
	m_type = mGPDtype;
	m_gluon = true;
	GluonValue result;

	result.setGluonValue(compute(mKinematic));

	return result;
}

QuarkValue MellinMomentModule::computeQuarkValue(MellinMomentKinematic mKinematic, GPDType mGPDtype,
		QuarkFlavor mflavor) {
	m_type = mGPDtype;
	m_gluon = false;
	m_flavor = mflavor;
	QuarkValue result;

	result.setQuarkFlavor(mflavor);
	result.setQuarkValue(compute(mKinematic));

	return result;
}



int MellinMomentModule::getN()
{
	return m_n;
}

void MellinMomentModule::setN(int n)
{
	m_n = n;
}

} /* namespace PARTONS */
