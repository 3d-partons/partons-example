#include "../include/analysis.h"
#include "examples.h"

#include <ElementaryUtils/parameters/Parameter.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/List.h>
#include <partons/beans/observable/DDVCS/DDVCSObservableKinematic.h>
#include <partons/beans/observable/DVCS/DVCSObservableKinematic.h>
#include <partons/beans/observable/ObservableResult.h>
#include <partons/beans/observable/TCS/TCSObservableKinematic.h>
#include <partons/beans/process/VCSSubProcessType.h>
#include <partons/beans/PerturbativeQCDOrderType.h>
#include <partons/FundamentalPhysicalConstants.h>
#include <partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h>
//
#include <partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h>
//
#include <partons/modules/convol_coeff_function/DDVCS/DDVCSCFFTEST.h>
#include <partons/modules/convol_coeff_function/DVCS/DVCSCFFStandard.h>
#include <partons/modules/convol_coeff_function/TCS/TCSCFFStandard.h>
#include <partons/modules/gpd/GPDGK16Numerical.h>
#include <partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionUUMinusDVCSLimit.h>
#include <partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionUUMinusTCSLimit.h>
#include <partons/modules/observable/Observable.h>
#include <partons/modules/observable/TCS/cross_section/TCSCrossSectionUUThetaPhiIntegrated.h>
#include <partons/modules/process/DDVCS/DDVCSProcessDMSW22.h>
#include <partons/modules/process/DDVCS/DDVCSProcessTEST.h>
#include <partons/modules/process/DVCS/DVCSProcessBMJ12.h>
#include <partons/modules/process/TCS/TCSProcessBDP01.h>
#include <partons/modules/scales/DDVCS/DDVCSScalesTEST.h>
#include <partons/modules/scales/DVCS/DVCSScalesQ2Multiplier.h>
#include <partons/modules/scales/TCS/TCSScalesQ2PrimMultiplier.h>
#include <partons/modules/xi_converter/DDVCS/DDVCSXiConverterTEST.h>
#include <partons/modules/xi_converter/DVCS/DVCSXiConverterXBToXi.h>
#include <partons/modules/xi_converter/TCS/TCSXiConverterTauToXi.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/utils/type/PhysicalType.h>
#include <partons/utils/type/PhysicalUnit.h>
#include <stddef.h>
#include <cmath>
#include <iostream>
#include <utility>

#include "../include/leptonCMframe.h"

using namespace PARTONS;

void compareLimit(int lim, int cff) { // lim = 0 for DVCS limit, 1 for TCS; cff = 0 if you don't want cff to be computed but only xsec, 1 if you want only cff

    // ******************************************************
    // Modules **********************************************
    // ******************************************************

    // Create GPDModule
    GPDModule* pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    GPDGK16Numerical::classId);

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
    DDVCSXiConverterModule* pDDVCSXiConverterModule =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSXiConverterModule(
                    DDVCSXiConverterTEST::classId);

    // Create ScalesModule
    DDVCSScalesModule* pDDVCSScalesModule =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSScalesModule(
                    DDVCSScalesTEST::classId);

    // Create ProcessModule
    DDVCSProcessModule* pDDVCSProcessModule =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSProcessModule(
//                    DDVCSProcessTEST::classId);
                    DDVCSProcessDMSW22::classId);

    // Create ObservableModule
    DDVCSObservable* pDDVCSObservableModuleDVCSLimit =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSObservable(
                    DDVCSCrossSectionUUMinusDVCSLimit::classId);
    DDVCSObservable* pDDVCSObservableModuleTCSLimit =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSObservable(
                    DDVCSCrossSectionUUMinusTCSLimit::classId);

    // Link modules (set physics assumptions of your computation)
    pDDVCSCFFModel->setGPDModule(pGPDModule);
    pDDVCSProcessModule->setScaleModule(pDDVCSScalesModule);
    pDDVCSProcessModule->setXiConverterModule(pDDVCSXiConverterModule);
    pDDVCSProcessModule->setConvolCoeffFunctionModule(pDDVCSCFFModel);
    pDDVCSObservableModuleDVCSLimit->setProcessModule(pDDVCSProcessModule);
    pDDVCSObservableModuleTCSLimit->setProcessModule(pDDVCSProcessModule);

    // Create CFF module
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Set its PerturbativeQCDOrder
    pDVCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    // Create XiConverterModule
    PARTONS::DVCSXiConverterModule* pDVCSXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSXiConverterModule(
                    PARTONS::DVCSXiConverterXBToXi::classId);

    // Create ScalesModule
    PARTONS::DVCSScalesModule* pDVCSScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSScalesModule(
                    PARTONS::DVCSScalesQ2Multiplier::classId);

    // Create ProcessModule
    PARTONS::DVCSProcessModule* pDVCSProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(
                    PARTONS::DVCSProcessBMJ12::classId);

    // Link modules (set physics assumptions of your computation)
    pDVCSProcessModule->setScaleModule(pDVCSScalesModule);
    pDVCSProcessModule->setXiConverterModule(pDVCSXiConverterModule);
    pDVCSProcessModule->setConvolCoeffFunctionModule(pDVCSCFFModel);
    pDVCSCFFModel->setGPDModule(pGPDModule);

    // Create CFF module
    PARTONS::TCSConvolCoeffFunctionModule* pTCSCFFModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSConvolCoeffFunctionModule(
                    PARTONS::TCSCFFStandard::classId);

    // Set its PerturbativeQCDOrder
    pTCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    // Create XiConverterModule
    PARTONS::TCSXiConverterModule* pTCSXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSXiConverterModule(
                    PARTONS::TCSXiConverterTauToXi::classId);

    // Create ScalesModule
    PARTONS::TCSScalesModule* pTCSScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSScalesModule(
                    PARTONS::TCSScalesQ2PrimMultiplier::classId);

    // Create ProcessModule
    PARTONS::TCSProcessModule* pTCSProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSProcessModule(
                    PARTONS::TCSProcessBDP01::classId);

    PARTONS::TCSObservable* pTCSObservableModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSObservable(
                    TCSCrossSectionUUThetaPhiIntegrated::classId);

    // Link modules (set physics assumptions of your computation)
    pTCSProcessModule->setScaleModule(pTCSScalesModule);
    pTCSProcessModule->setXiConverterModule(pTCSXiConverterModule);
    pTCSProcessModule->setConvolCoeffFunctionModule(pTCSCFFModel);
    pTCSCFFModel->setGPDModule(pGPDModule);
    pTCSObservableModule->setProcessModule(pTCSProcessModule);

    //Get GPDs for xsec
    List<GPDType> gpdTypes;

//    gpdTypes.add(GPDType::H);
    gpdTypes.add(GPDType::E);
//    gpdTypes.add(GPDType::Ht);
//    gpdTypes.add(GPDType::Et);
//    gpdTypes.add(GPDType::HL);
//    gpdTypes.add(GPDType::EL);

    //Choose GPD for CFF
    GPDType::Type currentGPD;
    currentGPD = GPDType::E;

    // ******************************************************
    // DVCS limit  ******************************************
    // ******************************************************

    if (lim == 0) {

        if (cff == 0) {
            //xsec evaluation

            double xB = 0.04;
            double t = -0.1;
            double Q2 = 10.;
            double Q2Prim = 100 * 2 * Constant::ELECTRON_MASS;
            double E = 160.;

            for (size_t i = 0; i <= 50; i++) {

                //double phi = 0.001 + i * (2 * M_PI - 0.002)/ 50.;

                double phi = 0.; //ignored
//            t = -1 * (0.01 + i * (0.5 - 0.02) / 50.);

//            double phi = 0; //ignored
//            t = -0.01;
                //Q2Prim = pow(10., -6.) + i * (0.001 - pow(10., -6.)) / 100;
                Q2Prim = 0.0001 + i * (0.01 - 0.0001) / 20.;
//            Q2Prim = 0.;

                double NoverQPrim2 = Constant::FINE_STRUCTURE_CONSTANT
                        / (4 * M_PI) * (4 / 3.) / Q2Prim;

                double ddvcsValue = pDDVCSObservableModuleDVCSLimit->compute(
                        DDVCSObservableKinematic(xB, t, Q2, Q2Prim, E, phi, 0.,
                                0.), gpdTypes).getValue().getValue();

                //DVCS
                double A = pDVCSProcessModule->compute(1., -1,
                        NumA::Vector3D(0., 0., 0.),
                        DVCSObservableKinematic(xB, t, Q2, E, phi), gpdTypes,
                        VCSSubProcessType::DVCS).getValue().makeSameUnitAs(
                        PhysicalUnit::NB).getValue();
                double B = pDVCSProcessModule->compute(-1., -1,
                        NumA::Vector3D(0., 0., 0.),
                        DVCSObservableKinematic(xB, t, Q2, E, phi), gpdTypes,
                        VCSSubProcessType::DVCS).getValue().makeSameUnitAs(
                        PhysicalUnit::NB).getValue();

                double dvcsValue = 2 * M_PI * (A + B) / 2.;

                std::cout << Q2Prim << " " << phi << " " << 2 * M_PI * dvcsValue
                        << " " << ddvcsValue / NoverQPrim2 / (2 * M_PI)
                        << " Q2Prim phi dvcs ddvcs_in_lim_of_dvcs" << std::endl;
            }
        }

        if (cff == 1) {
            //CFF evaluation

            int total = 50;

            for (int i = -1; i <= total; i++) {

                double log10Q2PrimMin = -4;
                double log10Q2PrimMax = 0.;

                double Q2Prim = pow(10.,
                        log10Q2PrimMin
                                + i * (log10Q2PrimMax - log10Q2PrimMin)
                                        / total);
                double Q2 = 1.;

                double xi = 0.1;
                double t = -0.1;
                double muF2 = Q2 + Q2Prim;
                double muR2 = Q2 + Q2Prim;

                if (i == -1) { //DVCS point

                    PARTONS::DVCSConvolCoeffFunctionResult cffResult =
                            computeSingleKinematicsForDVCSComptonFormFactor(xi,
                                    t, Q2, muF2, muR2);

                    //Q2 vs real and imaginary parts of CFF_H
                    std::cout << Q2 << ' ' << 0. << ' '
                            << cffResult.getResult(currentGPD).real() << ' '
                            << cffResult.getResult(currentGPD).imag() << ' '
                            << "Q2 Q2Prim Re(cff) Im(cff) dvcs_point"
                            << std::endl;
                } else {

                    DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t,
                            Q2, Q2Prim, muF2, muR2);

                    DDVCSConvolCoeffFunctionResult cffResult =
                            pDDVCSCFFModel->compute(thiscffKinematics,
                                    gpdTypes);

                    //Q2 vs real and imaginary parts of CFF_H
                    std::cout << Q2 << ' ' << Q2Prim << ' '
                            << cffResult.getResult(currentGPD).real() << ' '
                            << cffResult.getResult(currentGPD).imag() << ' '
                            << "Q2 Q2Prim Re(cff) Im(cff)" << std::endl;
                }
            }
        }
    }

    // ******************************************************
    // TCS limit  *******************************************
    // ******************************************************

    leptons cmFrame;

    if (lim == 1) {

        if (cff == 0) {
            //xsec evaluation

            double xB = 0.5 * pow(10., -5.);
            double t = -0.01;
            double Q2 = 0.5 * pow(10., -3.);
            double Q2Prim = 10.;
            double E = 60.;
            double thetaL = 2.5 * M_PI / 4.;

            double nu = Q2 / (2 * Constant::PROTON_MASS * xB);
            double y = nu / E;

            std::cout << "nu: " << nu << std::endl;
            std::cout << "y: " << y << std::endl;

            double Q2Min = pow(y * Constant::ELECTRON_MASS, 2) / (1. - y);

            std::cout << "Q2Min: " << Q2Min << std::endl;

            double flux = Constant::FINE_STRUCTURE_CONSTANT / (2 * M_PI * Q2)
                    * ((1. + pow(1. - y, 2)) / y
                            - 2 * (1. - y) * Q2Min / (y * Q2));

            flux *= Q2 / (2 * E * Constant::PROTON_MASS * xB * xB);

            cmFrame.computeConverterVariables(xB, t, Q2, Q2Prim,
                    Constant::PROTON_MASS);

            for (size_t i = 0; i <= 50; i++) {

                //double phiL = 0.001 + i * (2 * M_PI - 0.002) / 50.;
                double phiL = M_PI / 7.;
                double thetaL = 0.001 + i * (M_PI - 0.002) / 50.;

                std::pair<double, double> cmFrameResult =
                        cmFrame.leptonCMconverter(phiL, thetaL);

                double phiLBis = cmFrameResult.first;
                double thetaLBis = -1 * cmFrameResult.second;

                double ddvcsValue = pDDVCSObservableModuleTCSLimit->compute(
                        DDVCSObservableKinematic(xB, t, Q2, Q2Prim, E, 0., phiL,
                                thetaL), gpdTypes).getValue().getValue();

                double jac = cmFrame.jacobianLeptonCM(phiL, thetaL);

                ddvcsValue /= jac; //differential xsec of ddvcs in angles of BDP2001 (phiLBis, thetaLBis)

                //TCS
                double A =
                        pTCSProcessModule->compute(1.,
                                NumA::Vector3D(0., 0., 0.),
                                TCSObservableKinematic(t, Q2Prim, nu, phiLBis,
                                        thetaLBis), gpdTypes,
                                VCSSubProcessType::TCS).getValue().makeSameUnitAs(
                                PhysicalUnit::NB).getValue();
                double B =
                        pTCSProcessModule->compute(-1.,
                                NumA::Vector3D(0., 0., 0.),
                                TCSObservableKinematic(t, Q2Prim, nu, phiLBis,
                                        thetaLBis), gpdTypes,
                                VCSSubProcessType::TCS).getValue().makeSameUnitAs(
                                PhysicalUnit::NB).getValue();

                double tcsValue = 2 * M_PI * (A + B) / 2.;

//            std::cout << phiL << " " << phiLBis << " " << tcsValue << " "
//                    << ddvcsValue / flux / (2 * M_PI) << " " << jac
//                    << " phiL phiLBis tcs ddvcs_in_lim_of_tcs jacobian"
//                    << std::endl;

                std::cout << thetaL << " " << thetaLBis << " " << tcsValue
                        << " " << ddvcsValue / flux / (2 * M_PI) << " " << jac
                        << " thetaL thetaLBis tcs ddvcs_in_lim_of_tcs jacobian"
                        << std::endl;
            }
        }

        if (cff == 1) {
            //CFF evaluation

            int total = 50;

            for (int i = -1; i <= total; i++) {

                double log10Q2Min = -4.;
                double log10Q2Max = 0.;

                double Q2 = pow(10.,
                        log10Q2Min + i * (log10Q2Max - log10Q2Min) / total);
                double Q2Prim = 3.;

                double xi = 0.1;
                double t = -0.1;
                double muF2 = Q2 + Q2Prim;
                double muR2 = Q2 + Q2Prim;

                if (i == -1) { //TCS point

                    PARTONS::TCSConvolCoeffFunctionResult cffResult =
                            computeSingleKinematicsForTCSComptonFormFactor(xi,
                                    t, Q2Prim, muF2, muR2);

                    //Q2 vs real and imaginary parts of CFF_H
                    std::cout << 0. << ' ' << Q2Prim << ' '
                            << cffResult.getResult(currentGPD).real() << ' '
                            << cffResult.getResult(currentGPD).imag() << ' '
                            << "Q2 Q2Prim Re(cff) Im(cff) tcs_point"
                            << std::endl;
                } else {

                    DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t,
                            Q2, Q2Prim, muF2, muR2);

                    DDVCSConvolCoeffFunctionResult cffResult =
                            pDDVCSCFFModel->compute(thiscffKinematics,
                                    gpdTypes);

                    //Q2 vs real and imaginary parts of CFF_H
                    std::cout << Q2 << ' ' << Q2Prim << ' '
                            << cffResult.getResult(currentGPD).real() << ' '
                            << cffResult.getResult(currentGPD).imag() << ' '
                            << "Q2 Q2Prim Re(cff) Im(cff)" << std::endl;
                }
            }
        }
    }

}

