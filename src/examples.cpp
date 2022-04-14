//#include "../include/examples.h"

#include "examples.h"

#include <ElementaryUtils/parameters/Parameters.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h>
#include <partons/beans/PerturbativeQCDOrderType.h>
#include <partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h>
#include <partons/modules/convol_coeff_function/DVCS/DVCSCFFStandard.h>
#include <partons/modules/convol_coeff_function/TCS/TCSCFFStandard.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/services/ConvolCoeffFunctionService.h>
#include <partons/services/DVCSConvolCoeffFunctionService.h>
#include <partons/services/TCSConvolCoeffFunctionService.h>
#include <partons/ServiceObjectRegistry.h>

using namespace PARTONS;

DVCSConvolCoeffFunctionResult computeSingleKinematicsForDVCSComptonFormFactor(
        double xi, double t, double Q2, double muF2, double muR2) {

    // Retrieve service
    DVCSConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService =
            Partons::getInstance()->getServiceObjectRegistry()->getDVCSConvolCoeffFunctionService();

    // Create GPD module with the BaseModuleFactory
    GPDModule* pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    GPDGK16::classId);

    // Create CFF module with the BaseModuleFactory
    DVCSConvolCoeffFunctionModule* pDVCSCFFModule =
            Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    DVCSCFFStandard::classId);

    // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
    ElemUtils::Parameters parameters(
            PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PerturbativeQCDOrderType::LO);

    // Configure DVCSCFFModule with previous parameters.
    pDVCSCFFModule->configure(parameters);

    // Link modules (set physics assumptions of your computation)
    pDVCSCFFModule->setGPDModule(pGPDModule);

    // Create kinematic
    DVCSConvolCoeffFunctionKinematic cffKinematic =
            DVCSConvolCoeffFunctionKinematic(xi, t, Q2, muF2, muR2);

    // Run computation
    return pDVCSConvolCoeffFunctionService->computeSingleKinematic(cffKinematic,
            pDVCSCFFModule);
}

TCSConvolCoeffFunctionResult computeSingleKinematicsForTCSComptonFormFactor(
        double xi, double t, double Q2Prim, double muF2, double muR2) {

    // Retrieve service
    TCSConvolCoeffFunctionService* pTCSConvolCoeffFunctionService =
            Partons::getInstance()->getServiceObjectRegistry()->getTCSConvolCoeffFunctionService();

    // Create GPD module with the BaseModuleFactory
    GPDModule* pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    GPDGK16::classId);

    // Create CFF module with the BaseModuleFactory
    TCSConvolCoeffFunctionModule* pTCSCFFModule =
            Partons::getInstance()->getModuleObjectFactory()->newTCSConvolCoeffFunctionModule(
                    TCSCFFStandard::classId);

    // Create parameters to configure later TCSCFFModel with PerturbativeQCD = LO
    ElemUtils::Parameters parameters(
            PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PerturbativeQCDOrderType::LO);

    // Configure DVCSCFFModule with previous parameters.
    pTCSCFFModule->configure(parameters);

    // Link modules (set physics assumptions of your computation)
    pTCSCFFModule->setGPDModule(pGPDModule);

    // Create kinematic
    TCSConvolCoeffFunctionKinematic cffKinematic =
            TCSConvolCoeffFunctionKinematic(xi, t, Q2Prim, muF2, muR2);

    // Run computation
    return pTCSConvolCoeffFunctionService->computeSingleKinematic(cffKinematic,
            pTCSCFFModule);
}
