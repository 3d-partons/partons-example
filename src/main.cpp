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
#include <partons/modules/convol_coeff_function/DDVCS/DDVCSCFFStandard.h>
#include <partons/modules/gpd/GPDGK16Numerical.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/modules/gpd/GPDVGG99.h>
#include <partons/modules/gpd/GPDMMS13.h>
#include <partons/modules/process/DDVCS/DDVCSProcessDMSW22.h>
#include <partons/modules/scales/DDVCS/DDVCSScalesVirtualitiesSum.h>
#include <partons/beans/Scales.h>
#include <partons/modules/xi_converter/DDVCS/DDVCSXiConverterTNeglected.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <QtCore/qcoreapplication.h>
#include <complex>
#include <iostream>
#include <string>
#include <vector>
#include <include/analysis.h>
//#include <include/leptonCMframe.h>
#include <partons/modules/observable/DDVCS/leptonCMframe.h>
#include <partons/FundamentalPhysicalConstants.h>

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
        //GPD model: GK16
        GPDModule* pGPDModule =
                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                        GPDGK16Numerical::classId);
//                        GPDGK16::classId);

        // Create GPDModule
        //GPD model: VGG99
//        GPDModule* pGPDModule =
//                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
//                        GPDVGG99::classId);
//        pGPDModule->configure(ElemUtils::Parameter(
//                "setName",
//                "MSTW2008nlo68cl"));

        // Create GPDModule
        //GPD model: MMS13
//        GPDModule* pGPDModule =
//                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
//                        GPDMMS13::classId);

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
        DDVCSXiConverterModule* pXiConverterModule =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSXiConverterModule(
                        DDVCSXiConverterTNeglected::classId);

        // Create ScalesModule
        DDVCSScalesModule* pScalesModule =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSScalesModule(
                        DDVCSScalesVirtualitiesSum::classId);

        // Create ProcessModule

        std::string method = "DDVCSProcessDMSW22"; //"DDVCSProcessDMSW22" for KS' spinor techniques; "DDVCSProcessTEST" for BM2003 formulae

        std::cout << "Method_for_computing_cross-section: " << method
                << std::endl;

        DDVCSProcessModule* pProcessModule =
                Partons::getInstance()->getModuleObjectFactory()->newDDVCSProcessModule(
                        method);

        // Link modules (set physics assumptions of your computation)
        pProcessModule->setScaleModule(pScalesModule);
        pProcessModule->setXiConverterModule(pXiConverterModule);
        pProcessModule->setConvolCoeffFunctionModule(pDDVCSCFFModel);
        pDDVCSCFFModel->setGPDModule(pGPDModule);

        // -----------------------------------------------

        // GPD list
//        List<GPDType> gpdTypes;
//        gpdTypes.add(GPDType::H);
//        gpdTypes.add(GPDType::E);
//        gpdTypes.add(GPDType::Ht);
//        gpdTypes.add(GPDType::Et);
//        gpdTypes.add(GPDType::HL);
//        gpdTypes.add(GPDType::EL);

        //DVCS and TCS limits of DDVCS, CFFs and asymmetries computations
        int lim = -1;
        int cff = 0;
        int asym = 0;
        int intXsec = -1;
        compareLimit(lim, cff, asym, intXsec);

        //-----------------------------------------------------------------------

        //computation of amplitudes:
//        double XI = 0.135; //Zhao's xi (same as xi in our paper and -eta in BM2003)
//        double XIprime = 0.06; //Zhao's xi' (same as rho in our paper and xi in BM2003)
//
//        double xB;
//        double t = -0.15;
//        double Q2 = 1.25;
//        double Q2Prim;
//        double E = 11.;
//        double phi;
//        double phiL = M_PI / 3.;
//        double thetaL = M_PI / 6.;
//
//        Q2Prim = Q2 * (XI - XIprime) + XI * t / 2.;
//        Q2Prim /= (XIprime + XI);
//
//        xB = (XI + XIprime) * Q2;
//        xB /= (Q2 + t / 4.) * (1. + XI) - (XI + XIprime) * t / 2.;
//
//        //tMin value
//        double epsilon2 = pow(2. * xB * Constant::PROTON_MASS, 2.) / Q2;
//
//        double tMin = -1. / (4. * xB * (1. - xB) + epsilon2);
//        tMin *= (2. * ((1. - xB) * Q2 - xB * Q2Prim) + epsilon2 * (Q2 - Q2Prim)
//                - 2. * sqrt(1. + epsilon2)
//                        * sqrt(
//                                pow((1. - xB) * Q2 - xB * Q2Prim, 2.)
//                                        - epsilon2 * Q2 * Q2Prim));
//        double tMax = -1. / (4. * xB * (1. - xB) + epsilon2);
//        tMax *= (2. * ((1. - xB) * Q2 - xB * Q2Prim) + epsilon2 * (Q2 - Q2Prim)
//                + 2. * sqrt(1. + epsilon2)
//                        * sqrt(
//                                pow((1. - xB) * Q2 - xB * Q2Prim, 2.)
//                                        - epsilon2 * Q2 * Q2Prim));
//
//        double tVal = t;
//        std::cout << tMin << ", " << tVal << ", " << tMax
//                << " tMin, tVal, tMax " << std::endl;
//
//        int total = 36;
//        for (int i = 0; i <= total; i++) {
//
//            phi = i * 10.; //phi in deg
//            phi = phi * M_PI / 180.; //phi in rad
//
//            double xsec =
//                    pProcessModule->compute(1, -1, NumA::Vector3D(0., 0., 0.),
//                            DDVCSObservableKinematic(xB, tVal, Q2, Q2Prim, E,
//                                    phi, phiL, thetaL), gpdTypes,
//                            VCSSubProcessType::DDVCS).getValue().makeSameUnitAs(
//                            PhysicalUnit::NB).getValue();
//        }

        //------------------------------------------------

        //differential xsec calculation

//        //BEGIN Orsay kin
//        double phi;
//        double Q2Prim;
//        double XI = 0.135; //Zhao's xi (same as xi in our paper and -eta in BM2003)
//        double XIprime = 0.06; //Zhao's xi' (same as rho in our paper and xi in BM2003)
//
//        double phiL = M_PI / 3.;
//        phiL += M_PI; // "apparent" correction wrt to Orsay's
//        double thetaL = M_PI / 6.;
//        double E = 11.;
//        double t = -0.15;
//        double Q2 = 1.25;
//
//        double ml = 0.;
//        double Ebeam = 11.;
//
//        Q2Prim = Q2 * (XI - XIprime) + XI * t / 2.;
//        Q2Prim /= (XIprime + XI);
//
//        std::cout << Q2Prim << " =Q2Prim = 0.428846154 GeV^2" << std::endl;
//
//        double Qbar2 = 0.5 * (Q2 - Q2Prim + t / 2.);
//        double xB = (XIprime + XI) * Qbar2 - XIprime * t / 4.; //using Zhao/Orsay notation
//        xB /= ((1. + XI) * Qbar2 - XIprime * t / 2.);
//
//        std::cout << xB << " =xB = 0.175257269" << std::endl;
//
//        double eps2 = pow(2. * xB * Constant::PROTON_MASS, 2.) / Q2;
//        double y = sqrt(Q2 / eps2) * (1. / E);
//        double w2 = sqrt(Q2 / eps2) + t / (2. * Constant::PROTON_MASS); //eq 17 in BM2003
//        double v = sqrt(1. - Q2Prim / pow(w2, 2.));
//        double yt = 1. + v * cos(thetaL);
//        yt = yt / 2.;
//        double a0vcsOverVVdagger = 0.5 * (2. - 2. * y + y * y)
//                * (2. - 2. * yt + yt * yt);
//
//        std::cout << y << " =y; " << yt << " =yt; " << a0vcsOverVVdagger
//                << " =a_0^VCS/(VVdagger) for GPD H only" << std::endl;
//        //END Orsay kin
//
//        //Computation of the 7-fold xsec:
//        int total = 36.;
//
//        double Mnucleon = Constant::PROTON_MASS; // 0.938272081 GeV
//        std::cout << Mnucleon << " =Mnucleon = 0.938272081 GeV" << std::endl;
//        //        double xB = 0.175257269;
//        //        double t = -0.15;
//        //        double Qcal2 = 1.25;
//        //        double Mll2 = 0.428846154;
//        //        double phil = M_PI / 3.;
//        //        double thetal = M_PI / 6.;
//        //        double phi;
//        double xsec7;
//        //
//        //        //DEBUG Pawel's kin
//        //        Ebeam = 12.;
//        //        xB = 0.1 / 10. / 100. * 5;
//        //        Qcal2 = 0.1 / 100. * 5;
//        //        Mll2 = 1.;
//        //        thetal = M_PI/4.;
//        //        t = -0.4;
//        //        //END DEBUG
//
//        for (int i = 0; i <= total; i++) {
//
//            //phi = -M_PI + 2. * M_PI * i / total; //Trento's value
//
//            double phi_deg = 10. * i; // in degrees and Trento convention bc later in the code it will be changed to the BM2003 convention
//            phi = phi_deg * M_PI / 180.; // in rad
//
//            //            phi = M_PI / 7.; //DEBUG
//            //            phi_deg = phi * 180. / M_PI; //DEBUG
//
//            //phiL = 2.*M_PI*i/total;
//
//            //DEBUG getting BDP2001 phi and theta from BM2003's phiL and thetaL
//            //            leptons lep;
//            //            lep.computeConverterVariables(xB, t, Q2, Q2Prim, Mnucleon);
//            //            double phiBDP, thetaBDP;
//            //            phiBDP = lep.leptonCMconverter(phiL, thetaL).first;
//            //            thetaBDP = lep.leptonCMconverter(phiL, thetaL).second;
//            //
//            //            double Jac = lep.jacobianLeptonCM(phiL, thetaL); //jac's definition: d(xsec)/(... dthetal dphil) = jac * d(xsec)/(... dthetaBDP dphiBDP)
//            //
//            //            std::cout << phiL << " " << thetaL << " " << phiBDP << " " << thetaBDP << " " << Jac << " phiL, thetaL, phiBDP, thetaBDP, Jacobian" << std::endl;
//            //END DEBUG
//
//            //phiL = -phiL; // to get xsec wrt to t instead of |t|
//
//            xsec7 =
//                    pProcessModule->compute(1, -1, NumA::Vector3D(0., 0., 0.),
//                            DDVCSObservableKinematic(xB, t, Q2, Q2Prim, E, phi,
//                                    phiL, thetaL), gpdTypes,
//                            VCSSubProcessType::DDVCS).getValue().makeSameUnitAs(
//                            PhysicalUnit::NB).getValue();
//
//            xsec7 = xsec7 / sin(thetaL); //differential xsec wrt lepton solid angle, xB, Qcal2, Mll2, phi and t (if phiL = -phiL is used) or |t| (if not).
//
//            //            if (phi_deg >= 0. && phi_deg <= 180.) {
//            //                phi_deg += 180.;
//            //            } else if (phi_deg > 180. && phi_deg <= 360.) {
//            //                phi_deg -= 180;
//            //            }
//
//            std::cout << phi_deg << " " << xsec7 << " phi(deg;Trento) xsec7"
//                    << std::endl;
//
//        }

        //------------------------------------------------

        //Computation of CFF:

        //Choose GPD
//        GPDType::Type currentGPD;
//        currentGPD = GPDType::H;
//
//        int total = 25;
//
//        //Choose CFF evaluation with DVCS (point = 0) or TCS (point = 1) point
//        int point;
//        point = 0;
//
//        //with DVCS point
//
//        if (point == 0) {
//            for (int i = -1; i <= total; i++) {
//
//                double log10Q2PrimMin = -4;
//                double log10Q2PrimMax = 0.;
//
//                double Q2Prim = pow(10.,
//                        log10Q2PrimMin
//                                + i * (log10Q2PrimMax - log10Q2PrimMin)
//                                        / total);
//                double Q2 = 1.;
//
//                double xi = 0.1;
//                double t = -0.1;
//                double muF2 = Q2 + Q2Prim;
//                double muR2 = Q2 + Q2Prim;
//
//                if (i == -1) { //DVCS point
//
//                    PARTONS::DVCSConvolCoeffFunctionResult cffResult =
//                            computeSingleKinematicsForDVCSComptonFormFactor(xi,
//                                    t, Q2, muF2, muR2);
//
//                    //Q2 vs real and imaginary parts of CFF_H
//                    std::cout << Q2 << ' ' << 0. << ' '
//                            << cffResult.getResult(currentGPD).real() << ' '
//                            << cffResult.getResult(currentGPD).imag() << ' '
//                            << "Q2 Q2Prim Re(cff) Im(cff) dvcs_point" << std::endl;
//                } else {
//
//                    DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t,
//                            Q2, Q2Prim, muF2, muR2);
//
//                    DDVCSConvolCoeffFunctionResult cffResult =
//                            pDDVCSCFFModel->compute(thiscffKinematics,
//                                    gpdTypes);
//
//                    //Q2 vs real and imaginary parts of CFF_H
//                    std::cout << Q2 << ' ' << Q2Prim << ' '
//                            << cffResult.getResult(currentGPD).real() << ' '
//                            << cffResult.getResult(currentGPD).imag() << ' '
//                            << "Q2 Q2Prim Re(cff) Im(cff)" << std::endl;
//                }
//            }
//        }
//
//        //with TCS point
//
//        if (point == 1) {
//            for (int i = -1; i <= total; i++) {
//
//                double log10Q2Min = -4.;
//                double log10Q2Max = 0.;
//
//                double Q2 = pow(10.,
//                        log10Q2Min + i * (log10Q2Max - log10Q2Min) / total);
//                double Q2Prim = 3.;
//
//                double xi = 0.1;
//                double t = -0.1;
//                double muF2 = Q2 + Q2Prim;
//                double muR2 = Q2 + Q2Prim;
//
//                if (i == -1) { //TCS point
//
//                    PARTONS::TCSConvolCoeffFunctionResult cffResult =
//                            computeSingleKinematicsForTCSComptonFormFactor(xi,
//                                    t, Q2Prim, muF2, muR2);
//
//                    //Q2 vs real and imaginary parts of CFF_H
//                    std::cout << 0. << ' ' << Q2Prim << ' '
//                            << cffResult.getResult(currentGPD).real() << ' '
//                            << cffResult.getResult(currentGPD).imag() << ' '
//                            << "Q2 Q2Prim Re(cff) Im(cff) tcs_point" << std::endl;
//                } else {
//
//                    DDVCSConvolCoeffFunctionKinematic thiscffKinematics(xi, t,
//                            Q2, Q2Prim, muF2, muR2);
//
//                    DDVCSConvolCoeffFunctionResult cffResult =
//                            pDDVCSCFFModel->compute(thiscffKinematics,
//                                    gpdTypes);
//
//                    //Q2 vs real and imaginary parts of CFF_H
//                    std::cout << Q2 << ' ' << Q2Prim << ' '
//                            << cffResult.getResult(currentGPD).real() << ' '
//                            << cffResult.getResult(currentGPD).imag() << ' '
//                            << "Q2 Q2Prim Re(cff) Im(cff)" << std::endl;
//                }
//            }
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

    }        //end of 'try'

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
