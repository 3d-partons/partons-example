#include "examples.h"
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/List.h>
#include <partons/beans/observable/DDVCS/DDVCSObservableKinematic.h>
#include <partons/beans/observable/TCS/TCSObservableKinematic.h>
#include <partons/beans/PerturbativeQCDOrderType.h>
#include <partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h>
#include <partons/modules/convol_coeff_function/DDVCS/DDVCSCFFTEST.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/modules/process/DDVCS/DDVCSProcessTEST.h>
#include <partons/modules/scales/DDVCS/DDVCSScalesTEST.h>
#include <partons/beans/Scales.h>
#include <partons/modules/xi_converter/DDVCS/DDVCSXiConverterTEST.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <QtCore/qcoreapplication.h>
#include <complex>
#include <iostream>
#include <string>
#include <vector>

using namespace PARTONS;

/*
 * Parse XML scenarios.
 */
std::vector<std::string> parseArguments(int argc, char** argv) {
    std::vector<std::string> xmlScenarios(argc - 1);

    for (unsigned int i = 1; i < argc; i++) {
        xmlScenarios[i - 1] = argv[i];
    }

    return xmlScenarios;
}

/*
 * Main function.
 */
int main(int argc, char** argv) {

    // Init Qt4
    QCoreApplication a(argc, argv);
    PARTONS::Partons* pPartons = 0;

    try {

        // Init PARTONS application
        pPartons = PARTONS::Partons::getInstance();
        pPartons->init(argc, argv);

        // ******************************************************
        // CODE *************************************************
        // ******************************************************

        // Create GPDModule
        GPDModule* pGPDModule =
                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                        GPDGK16::classId);

        // Create CFF module
        DDVCSConvolCoeffFunctionModule* pDDVCSCFFModel =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSConvolCoeffFunctionModule(
                        DDVCSCFFTEST::classId);

        // Set its PerturbativeQCDOrder
        pDDVCSCFFModel->configure(
                ElemUtils::Parameter(
                        PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                        PerturbativeQCDOrderType::LO));

        // Create XiConverterModule
        DDVCSXiConverterModule* pXiConverterModule =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSXiConverterModule(
                        DDVCSXiConverterTEST::classId);

        // Create ScalesModule
        DDVCSScalesModule* pScalesModule =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSScalesModule(
                        DDVCSScalesTEST::classId);

        // Create ProcessModule
        DDVCSProcessModule* pProcessModule =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSProcessModule(
                        DDVCSProcessTEST::classId);

        // Link modules (set physics assumptions of your computation)
        pProcessModule->setScaleModule(pScalesModule);
        pProcessModule->setXiConverterModule(pXiConverterModule);
        pProcessModule->setConvolCoeffFunctionModule(pDDVCSCFFModel);
        pDDVCSCFFModel->setGPDModule(pGPDModule);

        // -----------------------------------------------

        // GPD list
        List<GPDType> gpdTypes;
        gpdTypes.add(GPDType::H);

        int total = 20;

        //DVCS

        for (int i = -1; i <= total; i++) {

            double log10Q2PrimMin = -4;
            double log10Q2PrimMax = 0.;

            double Q2Prim = pow(10.,
                    log10Q2PrimMin
                            + i * (log10Q2PrimMax - log10Q2PrimMin) / total);
            double Q2 = 1.;

            double xi = 0.1;
            double t = -0.1;
            double muF2 = Q2 + Q2Prim;
            double muR2 = Q2 + Q2Prim;

            if (i == -1) { //DVCS point

                PARTONS::DVCSConvolCoeffFunctionResult cffResult =
                        computeSingleKinematicsForDVCSComptonFormFactor(xi, t,
                                Q2, muF2, muR2);

                //Q2 vs real and imaginary parts of CFF_H
                std::cout << Q2 << ' ' << 0. << ' '
                        << cffResult.getResult(GPDType::H).real() << ' '
                        << cffResult.getResult(GPDType::H).imag() << std::endl;
            } else {

                DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t, Q2,
                        Q2Prim, muF2, muR2);

                DDVCSConvolCoeffFunctionResult cffResult =
                        pDDVCSCFFModel->compute(thiscffKinematics, gpdTypes);

                //Q2 vs real and imaginary parts of CFF_H
                std::cout << Q2 << ' ' << Q2Prim << ' '
                        << cffResult.getResult(GPDType::H).real() << ' '
                        << cffResult.getResult(GPDType::H).imag() << std::endl;
            }
        }

        //TCS

        for (int i = -1; i <= total; i++) {

            double log10Q2Min = -4.;
            double log10Q2Max = 0.;

            double Q2 = pow(10.,
                    log10Q2Min + i * (log10Q2Max - log10Q2Min) / total);
            double Q2Prim = 1.;

            double xi = 0.1;
            double t = -0.1;
            double muF2 = Q2 + Q2Prim;
            double muR2 = Q2 + Q2Prim;

            if (i == -1) { //TCS point

                PARTONS::TCSConvolCoeffFunctionResult cffResult =
                        computeSingleKinematicsForTCSComptonFormFactor(xi, t,
                                Q2Prim, muF2, muR2);

                //Q2 vs real and imaginary parts of CFF_H
                std::cout << 0. << ' ' << Q2Prim << ' '
                        << cffResult.getResult(GPDType::H).real() << ' '
                        << cffResult.getResult(GPDType::H).imag() << std::endl;
            } else {

                DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t, Q2,
                        Q2Prim, muF2, muR2);

                DDVCSConvolCoeffFunctionResult cffResult =
                        pDDVCSCFFModel->compute(thiscffKinematics, gpdTypes);

                //Q2 vs real and imaginary parts of CFF_H
                std::cout << Q2 << ' ' << Q2Prim << ' '
                        << cffResult.getResult(GPDType::H).real() << ' '
                        << cffResult.getResult(GPDType::H).imag() << std::endl;
            }
        }

        // -----------------------------------------------
        // Remove pointer references
        // Module pointers are managed by PARTONS
        Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                pGPDModule, 0);
        pGPDModule = 0;

        Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                pDDVCSCFFModel, 0);
        pDDVCSCFFModel = 0;

        Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                pXiConverterModule, 0);
        pXiConverterModule = 0;

        Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                pScalesModule, 0);
        pScalesModule = 0;

        Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                pProcessModule, 0);
        pProcessModule = 0;

    }
// Appropriate catching of exceptions is crucial for working of PARTONS.
// PARTONS defines its own type of exception, which allows to display class name and function name
// where the exception has occurred, but also a human readable explanation.
    catch (const ElemUtils::CustomException &e) {

        // Display what happened
        pPartons->getLoggerManager()->error(e);

        // Close PARTONS application properly
        if (pPartons) {
            pPartons->close();
        }
    }
// In a case of standard exception.
    catch (const std::exception &e) {

        // Display what happened
        pPartons->getLoggerManager()->error("main", __func__, e.what());

        // Close PARTONS application properly
        if (pPartons) {
            pPartons->close();
        }
    }

// Close PARTONS application properly
    if (pPartons) {
        pPartons->close();
    }

    return 0;
}
