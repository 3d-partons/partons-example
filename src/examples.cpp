#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/gpd/GPDResult.h>
#include <partons/beans/KinematicUtils.h>
#include <partons/beans/List.h>
#include <partons/beans/observable/ObservableKinematic.h>
#include <partons/beans/observable/ObservableResult.h>
#include <partons/beans/PerturbativeQCDOrderType.h>
#include <partons/modules/convol_coeff_function/DVCS/DVCSCFFStandard.h>
#include <partons/modules/gpd/GPDMMS13.h>
#include <partons/modules/observable/DVCS/asymmetry/DVCSAllMinus.h>
#include <partons/modules/process/DVCS/DVCSProcessGV08.h>
#include <partons/modules/scales/ScalesQ2Multiplier.h>
#include <partons/modules/xi_converter/XiConverterXBToXi.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/services/ConvolCoeffFunctionService.h>
#include <partons/services/GPDService.h>
#include <partons/services/ObservableService.h>
#include <partons/ServiceObjectRegistry.h>

/*
 * This function demonstrates a simple task as the evaluation of GPD model in a single kinematic point.
 * The result of this function is printed out to the standard output.
 * These are the values for all defined in the model GPDs, including singlet and non-singlet combinations for quarks.
 */
void computeSingleKinematicsForGPD() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Create a GPDKinematic(x, xi, t, MuF, MuR) to compute
    PARTONS::GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 2., 2.);

    // Run computation
    PARTONS::GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic,
            pGPDModel);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResult.toString());

    // Remove pointer reference ; Module pointers are managed by PARTONS.
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}

/*
 * This function demonstrates the evaluation of GPD model for kinematics defined in a text file.
 * In this file kinematic points are encoded in separate lines using the following format: "x|xi|t|MuF2|MuR2".
 * The result of this function is printed out to the standard output.
 * These are the values for all defined in the model GPDs, including singlet and non-singlet combinations for quarks.
 */
void computeManyKinematicsForGPD() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Load list of kinematics from file
    PARTONS::List<PARTONS::GPDKinematic> gpdKinematicList =
            PARTONS::KinematicUtils().getGPDKinematicFromFile(
                    "/home/partons/git/partons-example/bin/examples/kinematics_gpd.csv");

    // Run computation
    PARTONS::List<PARTONS::GPDResult> gpdResultList =
            pGPDService->computeManyKinematicOneModel(gpdKinematicList,
                    pGPDModel);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResultList.toString());

    // Remove pointer reference ; Module pointers are managed by PARTONS.
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}

/*
 * This function demonstrates a simple task as the evaluation of DVCS Compton Form Factors (CFF) in a single kinematic point.
 * The result of this function is printed out to the standard output.
 * These are CFF values for all GPD types defined in the selected GPD model.
 */
void computeSingleKinematicsForDVCSComptonFormFactor() {

    // Retrieve service
    PARTONS::ConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Create CFF module with the BaseModuleFactory
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
    ElemUtils::Parameters parameters(
            PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PARTONS::PerturbativeQCDOrderType::LO);

    // Configure DVCSCFFModule with previous parameters.
    pDVCSCFFModule->configure(parameters);

    // Link GPDModule to DVCSCFFModule
    pDVCSCFFModule->setGPDModule(pGPDModule);

    // Create kinematic
    PARTONS::DVCSConvolCoeffFunctionKinematic cffKinematic =
            PARTONS::DVCSConvolCoeffFunctionKinematic(0.01, -0.1, 4., 4., 4.);

    // Run computation
    PARTONS::DVCSConvolCoeffFunctionResult cffResult =
            pDVCSConvolCoeffFunctionService->computeForOneCCFModel(cffKinematic,
                    pDVCSCFFModule);

    // Print results for DVCSCFFModule
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            cffResult.toString());

    // Remove DVCSCFFModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModule, 0);
    pDVCSCFFModule = 0;

    // Remove GPDModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}

/*
 * This function demonstrates the evaluation of DVCS Compton Form Factors (CFF) for kinematics defined in a text file.
 * In this file kinematic points are encoded in separate lines using the following format: "xi|t|MuF2|MuR2|Q2".
 * The result of this function is printed out to the standard output.
 * These are CFF values for all GPD types defined in the selected GPD model.
 */
void computeManyKinematicsForDVCSComptonFormFactor() {

    // Retrieve service
    PARTONS::ConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Create CFF module with the BaseModuleFactory
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
    ElemUtils::Parameters parameters(
            PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PARTONS::PerturbativeQCDOrderType::LO);

    // Configure DVCSCFFModule with previous parameters.
    pDVCSCFFModule->configure(parameters);

    // Link GPDModule to DVCSCFFModule
    pDVCSCFFModule->setGPDModule(pGPDModule);

    // Load list of kinematics from file
    PARTONS::List<PARTONS::DVCSConvolCoeffFunctionKinematic> cffKinematicList =
            PARTONS::KinematicUtils().getCCFKinematicFromFile(
                    "/home/partons/git/partons-example/bin/examples/kinematics_dvcs_cff.csv");

    // Run computation
    PARTONS::List<PARTONS::DVCSConvolCoeffFunctionResult> cffResultList =
            pDVCSConvolCoeffFunctionService->computeForOneCCFModelAndManyKinematics(
                    cffKinematicList, pDVCSCFFModule);

    // Print results for DVCSCFFModule
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            cffResultList.toString());

    // Remove DVCSCFFModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModule, 0);
    pDVCSCFFModule = 0;

    // Remove GPDModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}

/*
 * This function demonstrates a simple task as the evaluation of DVCS observable in a single kinematic point.
 * The result of this function is printed out to the standard output.
 */
void computeSingleKinematicsForDVCSObservable() {

    // Retrieve Observable service
    PARTONS::ObservableService* pObservableService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

    // Create Observable
    PARTONS::Observable* pObservable =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    PARTONS::DVCSAllMinus::classId);

    // Create ProcessModule
    PARTONS::DVCSModule* pProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSModule(
                    PARTONS::DVCSProcessGV08::classId);

    // Create ScalesModule
    PARTONS::ScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newScalesModule(
                    PARTONS::ScalesQ2Multiplier::classId);

    // Set its lambda parameter, so MuF2 = MuR2 = lambda * Q2
    pScalesModule->configure(
            ElemUtils::Parameter(
                    PARTONS::ScalesQ2Multiplier::PARAMETER_NAME_LAMBDA, 1.));

    // Create XiConverterModule
    PARTONS::XiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newXiConverterModule(
                    PARTONS::XiConverterXBToXi::classId);

    // Create CFF module
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Set its PerturbativeQCDOrder
    pDVCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::NLO));

    // Create GPDModule
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Link module to each other
    pObservable->setProcessModule(pProcessModule);
    pProcessModule->setScaleModule(pScalesModule);
    pProcessModule->setXiConverterModule(pXiConverterModule);
    pProcessModule->setConvolCoeffFunctionModule(pDVCSCFFModel);
    pDVCSCFFModel->setGPDModule(pGPDModule);

    // Load list of kinematics from file
    PARTONS::ObservableKinematic observableKinematic =
            PARTONS::ObservableKinematic(0.2, -0.1, 2., 6.);

    // Create kinematic
    PARTONS::ObservableResult observableResult =
            pObservableService->computeObservable(observableKinematic,
                    pObservable);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            observableResult.toString());

    // Remove Observable pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pObservable, 0);
    pObservable = 0;

    // Remove ProcessModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    // Remove ScalesModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pScalesModule, 0);
    pScalesModule = 0;

    // Remove XiConverterModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pXiConverterModule, 0);
    pXiConverterModule = 0;

    // Remove DVCSCFFModel pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModel, 0);
    pDVCSCFFModel = 0;

    // Remove GPDModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}

/*
 * This function demonstrates the evaluation of DVCS observable for kinematics defined in a text file.
 * In this file kinematic points are encoded in separate lines using the following format: "xB|t|Q2|phi|E".
 * The result of this function is printed out to the standard output.
 */
void computeManyKinematicsForDVCSObservable() {

    // Retrieve Observable service
    PARTONS::ObservableService* pObservableService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

    // Create Observable
    PARTONS::Observable* pObservable =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    PARTONS::DVCSAllMinus::classId);

    // Create ProcessModule
    PARTONS::DVCSModule* pProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSModule(
                    PARTONS::DVCSProcessGV08::classId);

    // Create ScalesModule
    PARTONS::ScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newScalesModule(
                    PARTONS::ScalesQ2Multiplier::classId);

    // Set its lambda parameter, so MuF2 = MuR2 = lambda * Q2
    pScalesModule->configure(
            ElemUtils::Parameter(
                    PARTONS::ScalesQ2Multiplier::PARAMETER_NAME_LAMBDA, 1.));

    // Create XiConverterModule
    PARTONS::XiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newXiConverterModule(
                    PARTONS::XiConverterXBToXi::classId);

    // Create CFF module
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Set its PerturbativeQCDOrder
    pDVCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::NLO));

    // Create GPDModule
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Link module to each other
    pObservable->setProcessModule(pProcessModule);
    pProcessModule->setScaleModule(pScalesModule);
    pProcessModule->setXiConverterModule(pXiConverterModule);
    pProcessModule->setConvolCoeffFunctionModule(pDVCSCFFModel);
    pDVCSCFFModel->setGPDModule(pGPDModule);

    // Load list of kinematics from file
    PARTONS::List<PARTONS::ObservableKinematic> observableKinematicList =
            PARTONS::KinematicUtils().getObservableKinematicFromFile(
                    "/home/partons/git/partons-example/bin/examples/kinematics_dvcs_observable.csv");

    // Run computation
    PARTONS::List<PARTONS::ObservableResult> observableResultList =
            pObservableService->computeManyKinematicOneModel(
                    observableKinematicList, pObservable);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            observableResultList.toString());

    // Remove Observable pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pObservable, 0);
    pObservable = 0;

    // Remove ProcessModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    // Remove ScalesModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pScalesModule, 0);
    pScalesModule = 0;

    // Remove XiConverterModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pXiConverterModule, 0);
    pXiConverterModule = 0;

    // Remove DVCSCFFModel pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModel, 0);
    pDVCSCFFModel = 0;

    // Remove GPDModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}

/*
 * This function demonstrates how to change the integration routine in one of modules.
 * To make it possible, the module must inherit from MathIntegratorModule class (our doxygen documentation will tell you that).
 *
 * Be careful when you perform this operation.
 * Some integration routines may speed up computations, but at the same time they can be not accurate enough to be applied in some kinematic ranges (e.g. in low xB).
 * The infinities are also treated differently by various integration routines (or they are not treated at all).
 *
 * This function is the demonstration for GPD module.
 * Note however that the way of changing the integration routine that is presented here is applicable to any type of PARTONS module.
 */
void changeIntegrationRoutine() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Change integration routine
    pGPDModel->configure(
            ElemUtils::Parameter(
                    PARTONS::MathIntegratorModule::PARAM_NAME_INTEGRATOR_TYPE,
                    NumA::IntegratorType1D::GK21_ADAPTIVE));

    // Create a GPDKinematic(x, xi, t, MuF, MuR) to compute
    PARTONS::GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 2., 2.);

    // Run computation
    PARTONS::GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic,
            pGPDModel);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResult.toString());

    // Remove pointer reference ; Module pointers are managed by PARTONS.
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}
