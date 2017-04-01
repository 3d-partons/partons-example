#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/gpd/GPDResult.h>
#include <partons/beans/KinematicUtils.h>
#include <partons/beans/List.h>
#include <partons/beans/observable/ObservableKinematic.h>
#include <partons/beans/observable/ObservableResult.h>
#include <partons/beans/PerturbativeQCDOrderType.h>
#include <partons/modules/convol_coeff_function/DVCS/DVCSCFFModel.h>
#include <partons/modules/gpd/MMS13Model.h>
#include <partons/modules/observable/Alu.h>
#include <partons/modules/process/DVCS/GV2008Model.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/services/ConvolCoeffFunctionService.h>
#include <partons/services/GPDService.h>
#include <partons/services/ObservableService.h>
#include <partons/ServiceObjectRegistry.h>

void computeOneGPDModel() {
    // Retrieve GPD service
    GPDService* pGPDService =
            Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    GPDModule* pGPDModel =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    MMS13Model::classId);

    // Create a GPDKinematic(x, xi, t, MuF, MuR) to compute
    GPDKinematic gpdKinematic(-0.99, 0.99, 0., 1., 1.);

    GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic, pGPDModel);

    // Print results
    Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResult.toString());

    // Remove pointer reference ; Module pointers are managed by PARTONS.
    Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}

void computeOneComptonFormFactor() {
    // Retrieve service
    ConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService =
            Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService();

    // Create GPD module with the BaseModuleFactory
    GPDModule* pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    MMS13Model::classId);

    // Create CFF module with the BaseModuleFactory
    DVCSConvolCoeffFunctionModule* pDVCSCFFModule =
            Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    DVCSCFFModel::classId);

    // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
    ElemUtils::Parameters parameters(
            PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PerturbativeQCDOrderType::LO);

    // Configure DVCSCFFModule with previous parameters.
    pDVCSCFFModule->configure(parameters);

    // Link GPDModule to DVCSCFFModule
    pDVCSCFFModule->setGPDModule(pGPDModule);

    // Create kinematic
    DVCSConvolCoeffFunctionKinematic kinematic =
            DVCSConvolCoeffFunctionKinematic(0.0088933952793878, -0.1346,
                    1.5557, 4., 4.);

    // Run computation
    DVCSConvolCoeffFunctionResult result =
            pDVCSConvolCoeffFunctionService->computeForOneCCFModel(kinematic,
                    pDVCSCFFModule);

    // Print results for DVCSCFFModule
    Partons::getInstance()->getLoggerManager()->info("main", __func__,
            result.toString());

    // Remove DVCSCFFModule pointer reference
    Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModule, 0);
    pDVCSCFFModule = 0;

    // Remove GPDModule pointer reference
    Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}

void computeOneDVCSObservableManyKinematics() {
    // Retrieve Observable service
    ObservableService* pObservableService =
            Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

    // Create Observable
    Observable* pObservable =
            Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    Alu::classId);

    // Create ProcessModule
    DVCSModule* pProcessModule =
            Partons::getInstance()->getModuleObjectFactory()->newProcessModule(
                    GV2008Model::classId);

    // Create CFF module
    DVCSConvolCoeffFunctionModule* pDVCSCFFModel =
            Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    DVCSCFFModel::classId);

    // Set its PerturbativeQCDOrder
    ElemUtils::Parameters dvcsConvolCoeffFunctionParameters;

    dvcsConvolCoeffFunctionParameters.add(
            PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PerturbativeQCDOrderType::NLO);

    pDVCSCFFModel->configure(dvcsConvolCoeffFunctionParameters);

    // Create GPDModule
    GPDModule* pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    MMS13Model::classId);

    // Link module to each other
    pObservable->setProcessModule(pProcessModule);
    pProcessModule->setConvolCoeffFunctionModule(pDVCSCFFModel);
    pDVCSCFFModel->setGPDModule(pGPDModule);

    // Load list of kinematics from file
    List<ObservableKinematic> observableKinematicList =
            KinematicUtils().getObservableKinematicFromFile(
                    "/home/debian/workspace/PARTONS_release/bin/examples/Observable_kinematics.csv");

    // Run computation
    List<ObservableResult> results =
            pObservableService->computeManyKinematicOneModel(
                    observableKinematicList, pObservable);

    // Print results
    Partons::getInstance()->getLoggerManager()->info("main", __func__,
            results.toString());

    // Remove Observable pointer reference
    Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pObservable, 0);
    pObservable = 0;

    // Remove ProcessModule pointer reference
    Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    // Remove DVCSCFFModel pointer reference
    Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModel, 0);
    pDVCSCFFModel = 0;

    // Remove GPDModule pointer reference
    Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}
