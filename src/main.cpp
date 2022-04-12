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

        // Kinematics
        DDVCSObservableKinematic processKinematic(0.175257269, -0.15, 1.25,
                0.428846154, 11., 0.1, 0.2, 0.3);

        // GPD list
        List<GPDType> gpdTypes;
        gpdTypes.add(GPDType::H);
//        gpdTypes.add(GPDType::E);
//        gpdTypes.add(GPDType::Ht);
//        gpdTypes.add(GPDType::Et);
//        gpdTypes.add(GPDType::HL);
//        gpdTypes.add(GPDType::EL);

        // -----------------------------------------------

        // Test xi converter
//        PhysicalType<double> xiConverterResult_xi = pXiConverterModule->compute(
//                processKinematic);

//        Partons::getInstance()->getLoggerManager()->info("main", __func__,
//                ElemUtils::Formatter() << "xi converter test: xi: "
//                        << xiConverterResult_xi.toString());

        // -----------------------------------------------

        // Test scales converter
        Scales scalesResult = pScalesModule->compute(processKinematic);

        Partons::getInstance()->getLoggerManager()->info("main", __func__,
                ElemUtils::Formatter() << "scales test: "
                        << scalesResult.toString());

        // -----------------------------------------------

        //CFF kinematics
        //       DDVCSConvolCoeffFunctionKinematic cffKinematics(
//                xiConverterResult_xi.getValue(),
//                processKinematic.getT().getValue(),
//                processKinematic.getQ2().getValue(),
//                processKinematic.getQ2Prim().getValue(),
//                scalesResult.getMuF2().getValue(),
//                scalesResult.getMuR2().getValue());
//
//        // Test CFF module
//        DDVCSConvolCoeffFunctionResult cffResult = pDDVCSCFFModel->compute(
//                cffKinematics, gpdTypes);
//
//        Partons::getInstance()->getLoggerManager()->info("main", __func__,
//                ElemUtils::Formatter() << "cff test: " << cffResult.toString());


        /*int total = 150;

        for (int i = 0; i <= total; i++) {

            double Q2PrimMin = 0.;
            double Q2PrimMax = 1.;

            double Q2Prim = Q2PrimMin + i * (Q2PrimMax - Q2PrimMin) / total;
            double t = -0.1;
            double Q2 = 1.;
            double xi = 0.01;
            double muF2 = Q2 + Q2Prim; //scales defined here. They're also used in examples.cpp for computing DVCS and TCS point
            double muR2 = Q2 + Q2Prim;
            //double eta = xi*(Q2 - Q2Prim + t/2.)/(Q2 + Q2Prim);//if I include this line, I get two different eta values for real and imag parts of CFF H

            if (Q2Prim == 0.) { //DVCS point
                PARTONS::DVCSConvolCoeffFunctionResult cffDVCSpoint =
                        computeSingleKinematicsForDVCSComptonFormFactor(xi, t,
                                Q2, muF2, muR2);

                //Q'2 vs real and imaginary parts of CFF_H
                std::cout << Q2Prim << " "
                        << cffDVCSpoint.getResult(GPDType::H).real() << " Q'2"
                        << " realCFF_H" << std::endl;

                std::cout << Q2Prim << " "
                        << cffDVCSpoint.getResult(GPDType::H).imag() << " Q'2"
                        << " imagCFF_H" << std::endl;

            } else {
                DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t, Q2,
                        Q2Prim, muF2, muR2);

                DDVCSConvolCoeffFunctionResult cffResult =
                        pDDVCSCFFModel->compute(thiscffKinematics, gpdTypes);

                //Q'2 vs real and imaginary parts of CFF_H
                std::cout << Q2Prim << " "
                        << cffResult.getResult(GPDType::H).real() << " Q'2"
                        << " realCFF_H" << std::endl;

                std::cout << Q2Prim << " "
                        << cffResult.getResult(GPDType::H).imag() << " Q'2"
                        << " imagCFF_H" << std::endl;
            }

        }*/


        int total = 150;

        for (int i = 0; i <= total; i++) {

            double Q2Min = 0.;
            double Q2Max = 1.;

            double Q2 = Q2Min + i * (Q2Max - Q2Min) / total;
            double t = -0.1;
            double Q2Prim = 1.;
            double xi = 0.1;
            double muF2 = Q2 + Q2Prim;
            double muR2 = Q2 + Q2Prim;
            //double eta = xi*(Q2 - Q2Prim + t/2.)/(Q2 + Q2Prim);//if I include this line, I get two different eta values for real and imag parts of CFF H

            if (Q2 == 0.) { //TCS point
                PARTONS::TCSConvolCoeffFunctionResult cffTCSpoint =
                        computeSingleKinematicsForTCSComptonFormFactor(xi, t,
                                Q2Prim, muF2, muR2);

                //Q2 vs real and imaginary parts of CFF_H
                std::cout << Q2 << " "
                        << cffTCSpoint.getResult(GPDType::H).real() << " Q2"
                        << " realCFF_H" << std::endl;

                std::cout << Q2 << " "
                        << cffTCSpoint.getResult(GPDType::H).imag() << " Q2"
                        << " imagCFF_H" << std::endl;
            } else {

                DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t, Q2,
                        Q2Prim, muF2, muR2);

                DDVCSConvolCoeffFunctionResult cffResult =
                        pDDVCSCFFModel->compute(thiscffKinematics, gpdTypes);

                //Q2 vs real and imaginary parts of CFF_H
                std::cout << Q2 << " " << cffResult.getResult(GPDType::H).real()
                        << " Q2" << " realCFF_H" << std::endl;

                std::cout << Q2 << " " << cffResult.getResult(GPDType::H).imag()
                        << " Q2" << " imagCFF_H" << std::endl;
            }

        }

//        for (int i = 0; i <= total; i++) {
//
//            double xiMin = 0.14;
//            double xiMax = 0.95;
//
//            double xi = xiMin + i * (xiMax - xiMin) / total;
//            double t = -0.15;
//            double xB = 0.175257269;
//            double Q2Prim = 0.5;
//            double Q2 = (xi*(Q2Prim - t) + Q2Prim)/((2./xB - 1.)*xi - 1.);
//            double mu = Q2 + Q2Prim;
//
//            DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t, Q2,
//                    Q2Prim, mu, mu);
//
//            DDVCSConvolCoeffFunctionResult cffResult = pDDVCSCFFModel->compute(
//                    thiscffKinematics, gpdTypes);
//
//            //xi vs real and imaginary parts of CFF_H
//            std::cout << xi << " " << Q2 << " " << cffResult.getResult(GPDType::H).real()
//                    << " " << "xi" << " Q2" << " realCFF_H" << std::endl;
//
//            std::cout << xi << " " << Q2 << " " << cffResult.getResult(GPDType::H).imag()
//                    << " " << "xi" << " Q2" << " imagCFF_H" << std::endl;
//            Partons::getInstance()->getLoggerManager()->info("main", __func__,
//                    ElemUtils::Formatter() << "cff test: "
//                            << cffResult.toString());
//              }

        // -----------------------------------------------
        // Test process result
//        int total = 200;
//        for (int i = 0; i <= total; i++) {
//            double xBMin = 0.07;
//            double xBMax = 0.24;
//            double thisXb = xBMin + i * (xBMax - xBMin) / total;
//
//            DDVCSObservableKinematic thisprocessKinematic(thisXb, -0.15, 1.25,
//                    0.428846154, 11., 0.1, 0.2, 0.3);
//
//            PARTONS::DDVCSObservableResult processResult =
//                    pProcessModule->compute(1., 1., NumA::Vector3D(0., 0., 0.),
//                            thisprocessKinematic, gpdTypes,
//                            VCSSubProcessType::BH);
//
//            //xB vs xsec7 in pb/GeV^6
//            std::cout << thisXb << " "
//                    << processResult.getValue().makeSameUnitAs(PhysicalUnit::PB).getValue()
//                    << " " << "xB" << " xsec7_BH" << std::endl;
//
//            /*Partons::getInstance()->getLoggerManager()->info("main", __func__,
//             ElemUtils::Formatter() << "process converter test: "
//             << processResult.toString());*/
//
//        }
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
