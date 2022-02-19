#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/linear_algebra/vector/Vector3D.h>
#include <partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/DDVCS/DDVCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/List.h>
#include <partons/beans/observable/DDVCS/DDVCSObservableKinematic.h>
#include <partons/beans/observable/ObservableResult.h>
#include <partons/beans/process/VCSSubProcessType.h>
#include <partons/beans/PerturbativeQCDOrderType.h>
#include <partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h>
#include <partons/modules/convol_coeff_function/DDVCS/DDVCSCFFTEST.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/modules/process/DDVCS/DDVCSProcessTEST.h>
#include <partons/modules/scales/DDVCS/DDVCSScalesTEST.h>
#include <partons/modules/xi_converter/DDVCS/DDVCSXiConverterTEST.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/beans/Scales.h>
#include <partons/Partons.h>
#include <partons/utils/type/PhysicalType.h>
#include <QtCore/qcoreapplication.h>
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
//        PhysicalType<double> xiConverterResult_eta =
//                pXiConverterModule->computeEta(processKinematic);

//        Partons::getInstance()->getLoggerManager()->info("main", __func__,
//                ElemUtils::Formatter() << "xi converter test: xi: "
//                        << xiConverterResult_xi.toString() << " eta: "
//                        << xiConverterResult_eta.toString());

        // -----------------------------------------------

        // Test scales converter
        Scales scalesResult = pScalesModule->compute(processKinematic);

        Partons::getInstance()->getLoggerManager()->info("main", __func__,
                ElemUtils::Formatter() << "scales test: "
                        << scalesResult.toString());

        // -----------------------------------------------

        //CFF kinematics
//        DDVCSConvolCoeffFunctionKinematic cffKinematics(
//                xiConverterResult_xi.getValue(),
//                xiConverterResult_eta.getValue(),
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

        int total = 100;
        for (int i = 0; i <= total; i++) {
            double xiMin = 0.01;
            double xiMax = 0.5;
            double thisXi = xiMin + i * (xiMax - xiMin) / total;
            double thisEta = thisXi * (1.25 - 0.428846154 - 0.15 / 2)
                    / (1.25 + 0.428846154);
            double thisXb = ((thisEta + thisXi) * 0.5
                    * (1.25 - 0.428846154 - 0.15 / 2) - thisEta * (-0.15) / 4.)
                    / ((1. + thisXi) * 0.5 * (1.25 - 0.428846154 - 0.15 / 2)
                            - thisEta * (-0.15) / 2.);

            DDVCSObservableKinematic thisprocessKinematic(thisXb, -0.15, 1.25,
                    0.428846154, 11., 0.1, 0.2, 0.3);

            PhysicalType<double> xiConverterResult_xi =
                    pXiConverterModule->compute(thisprocessKinematic);
            PhysicalType<double> xiConverterResult_eta =
                    pXiConverterModule->computeEta(thisprocessKinematic);

            DDVCSConvolCoeffFunctionKinematic thiscffKinematics(
                    xiConverterResult_xi.getValue(),
                    xiConverterResult_eta.getValue(),
                    thisprocessKinematic.getT().getValue(),
                    thisprocessKinematic.getQ2().getValue(),
                    thisprocessKinematic.getQ2Prim().getValue(),
                    scalesResult.getMuF2().getValue(),
                    scalesResult.getMuR2().getValue());

            DDVCSConvolCoeffFunctionResult cffResult = pDDVCSCFFModel->compute(
                    thiscffKinematics, gpdTypes);

            //xi vs real and imaginary parts of CFF_H
            std::cout << thisXi << " " << cffResult.getResult(GPDType::H).real()
                    << " " << "xi" << " realCFF_H" << std::endl;

            std::cout << thisXi << " " << cffResult.getResult(GPDType::H).imag()
                                << " " << "xi" << " imagCFF_H" << std::endl;

//            Partons::getInstance()->getLoggerManager()->info("main", __func__,
//                    ElemUtils::Formatter() << "cff test: "
//                            << cffResult.toString());

        }

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
