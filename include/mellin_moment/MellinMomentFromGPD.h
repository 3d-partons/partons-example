#ifndef MELLIN_MOMENT_H
#define MELLIN_MOMENT_H

/**
 * @file MellinMomentFromGPD.h
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
#include "MellinMomentModule.h"
#include "../parton_value/QuarkValue.h"
#include "../parton_value/GluonValue.h"
#include "../parton_value/PartonValues.h"

namespace PARTONS {

/**
 * @class MellinMomentFromGPD
 *
 * @brief Class representing single computation of Mellin Moment from GPD.
 *
 * This class is used to compute Mellin Moment for a specific n, GPD type, GPD model and for both quarks and gluons.
 */

class MellinMomentFromGPD: public MellinMomentModule, public MathIntegratorModule {
public:

    static const std::string MELLIN_MOMENT_MODULE_CLASS_NAME; ///< Type of the module in XML automation.
    static const std::string MELLIN_MOMENT_MODULE_GPD_MODEL; ///< Parameter used in configure() or XML automation to set the type of the GPD to compute.

    /**
     * ID assigned by BaseObjectRegistry.
     */
    static const unsigned int classId;
    /**
     * Default constructor.
     * @param className Name of class.
     */
    MellinMomentFromGPD(const std::string &className);

	virtual MellinMomentFromGPD* clone() const;

	virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

	GPDModule* getGPDModule() const;
	void setGPDModule(GPDModule* pGPDModel);

	virtual List<QuarkFlavor> getListOfAvailableQuarkFlavor(MellinMomentKinematic mKinematic, const GPDType &gpdType);

	virtual List<GPDType> getListOfAvailableGPDType();

protected:
	virtual void isModuleWellConfigured();
	virtual void initModule();
	MellinMomentFromGPD(const MellinMomentFromGPD &other);

private:
	GPDModule* m_pGPDModel; ///< Pointer to GPD model to use.
	NumA::FunctionType1D* m_pint; ///< Functor related to integrant.

	virtual double compute(MellinMomentKinematic mKinematic);

	double integrant(double x, std::vector<double> par);
}
;

} /* namespace PARTONS */

#endif /* MELLIN_MOMENT_H */
