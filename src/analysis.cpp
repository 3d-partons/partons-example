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

#include <partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/TCS/TCSConvolCoeffFunctionResult.h>
//
#include <partons/modules/convol_coeff_function/DDVCS/DDVCSCFFStandard.h>
#include <partons/modules/convol_coeff_function/DVCS/DVCSCFFStandard.h>
#include <partons/modules/convol_coeff_function/TCS/TCSCFFStandard.h>

#include <partons/modules/gpd/GPDGK16Numerical.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/modules/gpd/GPDVGG99.h>
#include <partons/modules/gpd/GPDMMS13.h>
#include <partons/modules/gpd/GPDHM18.h>

#include <partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionUUMinusDVCSLimit.h>
#include <partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionUUMinusTCSLimit.h>
#include <partons/modules/observable/DDVCS/cross_section/DDVCSCrossSectionTotal.h>

#include <partons/modules/observable/DDVCS/asymmetry/DDVCSAluPhi.h>
#include <partons/modules/observable/DDVCS/asymmetry/DDVCSAluPhiL.h>
//#include <partons/modules/observable/TCS/asymmetry/TCSAcu.h>
#include <partons/modules/observable/TCS/asymmetry/TCSAcuThetaIntegrated.h>

#include <partons/modules/observable/Observable.h>
#include <partons/modules/observable/TCS/cross_section/TCSCrossSectionUUThetaPhiIntegrated.h>
#include <partons/modules/process/DDVCS/DDVCSProcessDMSW22.h>
#include <partons/modules/process/DDVCS/DDVCSProcessTEST.h>
#include <partons/modules/process/DVCS/DVCSProcessBMJ12.h>
#include <partons/modules/process/TCS/TCSProcessBDP01.h>
#include <partons/modules/scales/DDVCS/DDVCSScalesModule.h>
#include <partons/modules/scales/DDVCS/DDVCSScalesVirtualitiesSum.h>
#include <partons/modules/scales/DVCS/DVCSScalesQ2Multiplier.h>
#include <partons/modules/scales/TCS/TCSScalesQ2PrimMultiplier.h>
#include <partons/modules/xi_converter/DDVCS/DDVCSXiConverterTNeglected.h>
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

#include <partons/modules/observable/DDVCS/leptonCMframe.h>

using namespace PARTONS;

/*
 * (lim, cff) = (0, 0), DVCS limit, only xsec
 *            = (1, 0), TCS limit, only xsec
 *            = (0, 1), CFF in DDVCS including DVCS point
 *            = (1, 1), CFF in DDVCS including TCS point
 *
 * asym = 0, BDP's phiL beam-spin asymmetry: A_LU(phiL)
 *      = 1, Trento's phi beam-spin asymmetry: A_LU(phi)
 *
 * intXsec = 0, totally integrated xsec
 */

void compareLimit(int lim, int cff, int asym, int intXsec) {

    // ******************************************************
    // Modules **********************************************
    // ******************************************************

    // Create GPDModule
    //GPD model: GK16
    GPDModule* pGPDModule =
            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    GPDGK16Numerical::classId);
    //GPDGK16::classId);

    // Create GPDModule
//    //GPD model: VGG99
//    GPDModule* pGPDModule =
//            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
//                    GPDVGG99::classId);
//    pGPDModule->configure(ElemUtils::Parameter("setName", "MSTW2008nlo68cl"));

    // Create GPDModule
    //GPD model: MMS13
//    GPDModule* pGPDModule =
//            Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
//                    GPDMMS13::classId);
//    pGPDModule->configure(ElemUtils::Parameter("absoluteTolerance", 0.1));
//    pGPDModule->configure(ElemUtils::Parameter("relativeTolerance", 0.1));

    // Create CFF module
    DDVCSConvolCoeffFunctionModule* pDDVCSCFFModel =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSConvolCoeffFunctionModule(
                    DDVCSCFFStandard::classId);

    // Set its PerturbativeQCDOrder
    pDDVCSCFFModel->configure(
            ElemUtils::Parameter(
                    PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PerturbativeQCDOrderType::LO));

    // Create XiConverterModule
    DDVCSXiConverterModule* pDDVCSXiConverterModule =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSXiConverterModule(
                    DDVCSXiConverterTNeglected::classId);

    // Create ScalesModule
    DDVCSScalesModule* pDDVCSScalesModule =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSScalesModule(
                    DDVCSScalesVirtualitiesSum::classId);

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
    DDVCSObservable* pDDVCSObservableModuleDDVCSCrossSectionTotal =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSObservable(
                    DDVCSCrossSectionTotal::classId);
    DDVCSObservable* pDDVCSObservableModuleDDVCSAluPhi =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSObservable(
                    DDVCSAluPhi::classId);
    DDVCSObservable* pDDVCSObservableModuleDDVCSAluPhiL =
            Partons::getInstance()->getModuleObjectFactory()->newDDVCSObservable(
                    DDVCSAluPhiL::classId);

    // Link modules (set physics assumptions of your computation)
    pDDVCSCFFModel->setGPDModule(pGPDModule);
    pDDVCSProcessModule->setScaleModule(pDDVCSScalesModule);
    pDDVCSProcessModule->setXiConverterModule(pDDVCSXiConverterModule);
    pDDVCSProcessModule->setConvolCoeffFunctionModule(pDDVCSCFFModel);
    pDDVCSObservableModuleDVCSLimit->setProcessModule(pDDVCSProcessModule);
    pDDVCSObservableModuleTCSLimit->setProcessModule(pDDVCSProcessModule);
    pDDVCSObservableModuleDDVCSCrossSectionTotal->setProcessModule(
            pDDVCSProcessModule);
    pDDVCSObservableModuleDDVCSAluPhi->setProcessModule(pDDVCSProcessModule);
    pDDVCSObservableModuleDDVCSAluPhiL->setProcessModule(pDDVCSProcessModule);

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

    PARTONS::TCSObservable* pTCSObservableModuleTCSAcuThetaIntegrated =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newTCSObservable(
                    TCSAcuThetaIntegrated::classId);

    // Link modules (set physics assumptions of your computation)
    pTCSProcessModule->setScaleModule(pTCSScalesModule);
    pTCSProcessModule->setXiConverterModule(pTCSXiConverterModule);
    pTCSProcessModule->setConvolCoeffFunctionModule(pTCSCFFModel);
    pTCSCFFModel->setGPDModule(pGPDModule);
    pTCSObservableModule->setProcessModule(pTCSProcessModule);
    pTCSObservableModuleTCSAcuThetaIntegrated->setProcessModule(
            pTCSProcessModule);

    //Get GPDs for xsec and asymmetries
    List<GPDType> gpdTypes;

    gpdTypes.add(GPDType::H);
//    gpdTypes.add(GPDType::E);
//    gpdTypes.add(GPDType::Ht);
//    gpdTypes.add(GPDType::Et);
//    gpdTypes.add(GPDType::HL);
//    gpdTypes.add(GPDType::EL);

    //Choose GPD for CFF (you need to have uncommented the corresponding gpdTypes.add() above
    GPDType::Type GPDforCFF;
    GPDforCFF = GPDType::H;

    //computation of integrated xsecs
    if (intXsec == 0) {

        double E = 10.6;

        double xsecTotal =
                pDDVCSObservableModuleDDVCSCrossSectionTotal->compute(
                        DDVCSObservableKinematic(0., 0., 0., 0., E, 0., 0., 0.),
                        gpdTypes).getValue().getValue();

        std::cout << xsecTotal << " xsecTotal" << std::endl;
    }

    //computation of asymmetries for DDVCS. lim and cff inputs are ignored if asym != 0
    if (asym == 0) {
        // ******************************************************
        // Beam spin asymmetry (as a function of phiL) **********
        // ******************************************************
        double phiLBDP; //phiL as the azymuthal lepton angle in the BDP2001 lepton-CM frame

        double t = -0.2;
        double Q2 = 0.6;
        double Q2Prim = 2.5;
        double E = 10.6;
        double y = 0.5;

        //*** Computation of E (beam energy in proton rest frame) given energies of proton and electron beams in head-to-head collision:
//        double E1 = 10.; //electron energy
//        double E2 = 100.; //hadron energy
//        double m1 = Constant::ELECTRON_MASS; //electron mass
//        double m2 = Constant::PROTON_MASS; //hadron mass
//        double p1 = sqrt(E1 * E1 - m1 * m1);
//        double p2 = sqrt(E2 * E2 - m2 * m2);
//        E = (E1 * E2 + p1 * p2) / m2; //equivalent electron energy in fixed-target experiment
        //***

        double xB = Q2 / (2. * y * (Constant::PROTON_MASS) * E);

        //check where we fall in ERBL/DGLAP plane:
        double xi = (Q2 + Q2Prim) / (2. * Q2 / xB - Q2 - Q2Prim); //t-dependence dropped
        double rho = xi * (Q2 - Q2Prim) / (Q2 + Q2Prim); // t-dependence dropped
        std::cout << rho / xi << " " << rho << " " << xi
                << " rho/xi rho xi (NOTE: t-dropped as in CFF evaluation)"
                << std::endl;

        double AluPhiLValue = 0.;
//        double AcuValue = 0.;

        int total = 35;
        for (int i = 0; i <= total; i++) {

            phiLBDP = 0.001 + i * (2 * M_PI - 0.002) / total;
            phiLBDP = 1.4367;//DEBUG peak of asymmetry

            //DDVCS A_LU(phiL) asymmetry
            AluPhiLValue = pDDVCSObservableModuleDDVCSAluPhiL->compute(
                    DDVCSObservableKinematic(xB, t, Q2, Q2Prim, E, 0., phiLBDP,
                            0.), gpdTypes).getValue().getValue();

            std::cout << phiLBDP << " " << AluPhiLValue
                    << " phiLBDP A_LU(phiLBDP)" << std::endl;

            //TCS A_CU(phiL) asymmetry (there is integration in thetaLBDP)
//            double Ephoton = y * E;
//            AcuValue = pTCSObservableModuleTCSAcuThetaIntegrated->compute(
//                    TCSObservableKinematic(t, Q2Prim, Ephoton, phiLBDP, 0.),
//                    gpdTypes).getValue().getValue();

//            AcuValue *= -1.; // maybe?

//            std::cout << phiLBDP << " " << AluPhiLValue << " " << AcuValue
//                    << " " << 100. * fabs((AluPhiLValue - AcuValue) / AcuValue)
//                    << " phiLBDP ddvcs_A_LU(phiLBDP) tcs_A_CU(phiLBDP) rel.error(%)"
//                    << std::endl;

        }

    }

    if (asym == 1) {
        // ******************************************************
        // Beam spin asymmetry (as a function of phi) ***********
        // ******************************************************
        double phi; //in Trento convention

        double t = -0.1;
        double Q2 = 0.3;
        double Q2Prim = 2.5;
        double E = 22.;
        double y = 0.15;

        //*** Computation of E (beam energy in proton rest frame) given energies of proton and electron beams in head-to-head collision:
//        double E1 = 10.; //electron energy
//        double E2 = 100.; //hadron energy
//        double m1 = Constant::ELECTRON_MASS; //electron mass
//        double m2 = Constant::PROTON_MASS; //hadron mass
//        double p1 = sqrt(E1 * E1 - m1 * m1);
//        double p2 = sqrt(E2 * E2 - m2 * m2);
//        E = (E1 * E2 + p1 * p2) / m2; //equivalent electron energy in fixed-target experiment
        //***

        double xB = Q2 / (2. * y * (Constant::PROTON_MASS) * E);

        double AluPhiValue = 0.;

        int total = 35;
        for (int i = 0; i <= total; i++) {

            phi = 0.001 + i * (2 * M_PI - 0.002) / total;

            AluPhiValue = pDDVCSObservableModuleDDVCSAluPhi->compute(
                    DDVCSObservableKinematic(xB, t, Q2, Q2Prim, E, phi, 0., 0.),
                    gpdTypes).getValue().getValue();

            std::cout << phi << " " << AluPhiValue << " phi A_LU(phi) "
                    << std::endl;

        }

    }

    //no asymmetries, just computation of xsec and CFF for DDVCS comparing with DVCS and TCS limits
    // ******************************************************
    // DVCS limit  ******************************************
    // ******************************************************

    //xsec evaluation
    if (lim == 0 && cff == 0) {

        double xB = 0.2;
        double t = -0.25;
        double Q2 = 40.;
        double Q2Prim = pow(10., -6.);
        double E = 160.;
        double phi;

        //tMin value
        double epsilon2 = pow(2. * xB * Constant::PROTON_MASS, 2.) / Q2;

        double tMin = -1. / (4. * xB * (1. - xB) + epsilon2);
        tMin *= (2. * ((1. - xB) * Q2 - xB * Q2Prim) + epsilon2 * (Q2 - Q2Prim)
                - 2. * sqrt(1. + epsilon2)
                        * sqrt(
                                pow((1. - xB) * Q2 - xB * Q2Prim, 2.)
                                        - epsilon2 * Q2 * Q2Prim));

        std::cout << tMin << ", " << t << " tMin vs t " << std::endl;

        int total = 35;
        for (size_t i = 0; i <= total; i++) {
            phi = 0.001 + i * (2. * M_PI - 0.002) / total;

            double NoverQPrim2 = Constant::FINE_STRUCTURE_CONSTANT / (4. * M_PI)
                    * (4. / 3.) / Q2Prim;

            double ddvcsValue = pDDVCSObservableModuleDVCSLimit->compute(
                    DDVCSObservableKinematic(xB, t, Q2, Q2Prim, E, phi, 0., 0.),
                    gpdTypes).getValue().getValue();

            //DVCS
            double A = pDVCSProcessModule->compute(1., -1,
                    NumA::Vector3D(0., 0., 0.),
                    DVCSObservableKinematic(xB, t, Q2, E, phi), gpdTypes,
                    VCSSubProcessType::INT).getValue().makeSameUnitAs(
                    PhysicalUnit::NB).getValue();
            double B = pDVCSProcessModule->compute(-1., -1,
                    NumA::Vector3D(0., 0., 0.),
                    DVCSObservableKinematic(xB, t, Q2, E, phi), gpdTypes,
                    VCSSubProcessType::INT).getValue().makeSameUnitAs(
                    PhysicalUnit::NB).getValue();

            double dvcsValue = 2. * M_PI * (A + B) / 2.;

            std::cout << Q2Prim << " " << phi << " " << dvcsValue << " "
                    << ddvcsValue / NoverQPrim2 << " "
                    << 100. * fabs(dvcsValue - ddvcsValue / NoverQPrim2)
                            / dvcsValue
                    << " Q2Prim phi dvcs ddvcs_in_lim_of_dvcs rel.error(%)"
                    << std::endl;
        }
    }

    //CFF evaluation
    if (lim == 0 && cff == 1) {

        int total = 60.;
        for (int i = -1; i <= total + 10; i++) {

            double log10Q2PrimMin = -4.;
            double log10Q2PrimMax = 0.;
            double Q2Prim = pow(10.,
                    log10Q2PrimMin
                            + i * (log10Q2PrimMax - log10Q2PrimMin) / total);

            double Q2 = 1.;

            double xi = 0.4;
            double t = -0.15;
            double muF2 = Q2 + Q2Prim;
            double muR2 = Q2 + Q2Prim;

            if (i == -1) { //DVCS point

                PARTONS::DVCSConvolCoeffFunctionResult cffResult =
                        computeSingleKinematicsForDVCSComptonFormFactor(xi, t,
                                Q2, muF2, muR2);

                //Q2 vs real and imaginary parts of CFF_H
                std::cout << xi << ' ' << Q2 << ' ' << 0. << ' '
                        << cffResult.getResult(GPDforCFF).real() << ' '
                        << cffResult.getResult(GPDforCFF).imag() << ' '
                        << "xi Q2 Q2Prim Re(cff) Im(cff) dvcs_point"
                        << std::endl;
            } else {

                DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t, Q2,
                        Q2Prim, muF2, muR2);

                DDVCSConvolCoeffFunctionResult cffResult =
                        pDDVCSCFFModel->compute(thiscffKinematics, gpdTypes);

                //Q2 vs real and imaginary parts of CFF
                std::cout << xi << ' ' << Q2 << ' ' << Q2Prim << ' '
                        << cffResult.getResult(GPDforCFF).real() << ' '
                        << cffResult.getResult(GPDforCFF).imag() << ' '
                        << "xi Q2 Q2Prim Re(cff) Im(cff)" << std::endl;
            }

        }
    }

    // ******************************************************
    // TCS limit  *******************************************
    // ******************************************************

    //xsec evaluation
    if (lim == 1 && cff == 0) {

        double phiL;
        double xB = pow(10., -4.);
        double t = -0.25;
        double Q2 = pow(10., -2.);
        double Q2Prim = 33.;
        double E = 160.;
        double thetaL = 1.5 * M_PI / 4.;

        //tMin value
        double epsilon2 = pow(2. * xB * Constant::PROTON_MASS, 2.) / Q2;

        double tMin = -1. / (4. * xB * (1. - xB) + epsilon2);
        tMin *= (2. * ((1. - xB) * Q2 - xB * Q2Prim) + epsilon2 * (Q2 - Q2Prim)
                - 2. * sqrt(1. + epsilon2)
                        * sqrt(
                                pow((1. - xB) * Q2 - xB * Q2Prim, 2.)
                                        - epsilon2 * Q2 * Q2Prim));

        std::cout << tMin << ", " << t << " tMin vs t " << std::endl;

        double nu = Q2 / (2 * Constant::PROTON_MASS * xB);
        double y = nu / E;

        std::cout << "nu: " << nu << std::endl;
        std::cout << "y: " << y << std::endl;

        double Q2Min = pow(y * Constant::ELECTRON_MASS, 2.) / (1. - y);

        std::cout << "Q2Min: " << Q2Min << std::endl;

        double flux = Constant::FINE_STRUCTURE_CONSTANT
                * ((1. + pow(1. - y, 2)) / y - 2 * (1. - y) * Q2Min / (y * Q2));

        flux *= Q2 / (2 * E * Constant::PROTON_MASS * xB * xB);
        flux = flux / (2 * M_PI * Q2);

        int total = 35;
        for (size_t i = 0; i <= total; i++) {

            phiL = 0.001 + i * (2 * M_PI - 0.002) / total;

            double ddvcsValue = pDDVCSObservableModuleTCSLimit->compute(
                    DDVCSObservableKinematic(xB, t, Q2, Q2Prim, E, 0., phiL,
                            thetaL), gpdTypes).getValue().getValue();

            //TCS
            double thetaL_tcs = -thetaL; // bc in BDP01, dxsecBDP/(... dt dthetaL) but we use dxsec/(... d|t|dthetaL)

            double A = pTCSProcessModule->compute(1.,
                    NumA::Vector3D(0., 0., 0.),
                    TCSObservableKinematic(t, Q2Prim, nu, phiL, thetaL_tcs),
                    gpdTypes, VCSSubProcessType::INT).getValue().makeSameUnitAs(
                    PhysicalUnit::NB).getValue();
            double B = pTCSProcessModule->compute(-1.,
                    NumA::Vector3D(0., 0., 0.),
                    TCSObservableKinematic(t, Q2Prim, nu, phiL, thetaL_tcs),
                    gpdTypes, VCSSubProcessType::INT).getValue().makeSameUnitAs(
                    PhysicalUnit::NB).getValue();

            double tcsValue = 2. * M_PI * (A + B) / 2.;

            tcsValue = -tcsValue; // Jacobian coming from: dxsecBDP/(... dt dthetaL) -> dxsecBM2003/(... d|t|dthetaL)

            std::cout << Q2 << " " << phiL << " " << tcsValue << " "
                    << ddvcsValue / flux << " "
                    << 100. * fabs(tcsValue - ddvcsValue / flux) / tcsValue
                    << " Q2 phiL tcs ddvcs_in_lim_of_tcs rel.error(%)"
                    << std::endl;

            std::cout << Q2 << " " << thetaL << " " << tcsValue << " "
                    << ddvcsValue / flux << " "
                    << 100. * fabs(tcsValue - ddvcsValue / flux) / tcsValue
                    << " Q2 thetaL tcs ddvcs_in_lim_of_tcs rel.error(%)"
                    << std::endl;
        }
    }

    //CFF evaluation
    if (lim == 1 && cff == 1) {

        int total = 60;

        for (int i = -1; i <= total + 10; i++) {

            double log10Q2Min = -4.;
            double log10Q2Max = 0.;

            double Q2 = pow(10.,
                    log10Q2Min + i * (log10Q2Max - log10Q2Min) / total);

            double Q2Prim = 1.;

            double xi = 0.4;
            double t = -0.15;
            double muF2 = Q2 + Q2Prim;
            double muR2 = Q2 + Q2Prim;

            if (i == -1) { //TCS point

                PARTONS::TCSConvolCoeffFunctionResult cffResult =
                        computeSingleKinematicsForTCSComptonFormFactor(xi, t,
                                Q2Prim, muF2, muR2);

                //Q2 vs real and imaginary parts of CFF
                std::cout << xi << ' ' << 0. << ' ' << Q2Prim << ' '
                        << cffResult.getResult(GPDforCFF).real() << ' '
                        << cffResult.getResult(GPDforCFF).imag() << ' '
                        << "xi Q2 Q2Prim Re(cff) Im(cff) tcs_point"
                        << std::endl;
            } else {

                DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t, Q2,
                        Q2Prim, muF2, muR2);

                DDVCSConvolCoeffFunctionResult cffResult =
                        pDDVCSCFFModel->compute(thiscffKinematics, gpdTypes);

                //Q2 vs real and imaginary parts of CFF_H
                std::cout << xi << ' ' << Q2 << ' ' << Q2Prim << ' '
                        << cffResult.getResult(GPDforCFF).real() << ' '
                        << cffResult.getResult(GPDforCFF).imag() << ' '
                        << "xi Q2 Q2Prim Re(cff) Im(cff)" << std::endl;
            }
        }
    }

} //end compareLimit()

