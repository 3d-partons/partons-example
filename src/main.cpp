#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/GAM2/GAM2ConvolCoeffFunctionKinematic.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/List.h>
#include <partons/beans/PerturbativeQCDOrderType.h>
#include <partons/beans/PolarizationType.h>
#include <partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h>
#include <partons/modules/convol_coeff_function/GAM2/GAM2CFFStandard.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
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
    Partons* pPartons = 0;

    try {

        // Init PARTONS application
        pPartons = Partons::getInstance();
        pPartons->init(argc, argv);

        // Create GPD module with the BaseModuleFactory
        GPDModule* pGPDModule =
                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                        GPDGK16::classId);

        // Create CFF module with the BaseModuleFactory
        GAM2ConvolCoeffFunctionModule* pGAM2CFFModule =
                static_cast<GAM2ConvolCoeffFunctionModule*>(Partons::getInstance()->getModuleObjectFactory()->newModuleObject(
                        GAM2CFFStandard::classId));

        // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
        ElemUtils::Parameters parameters(
                PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                PerturbativeQCDOrderType::LO);

        // Configure DVCSCFFModule with previous parameters.
        pGAM2CFFModule->configure(parameters);

        // Link modules (set physics assumptions of your computation)
        pGAM2CFFModule->setGPDModule(pGPDModule);

        // Create kinematic
        GAM2ConvolCoeffFunctionKinematic cffKinematic =
                GAM2ConvolCoeffFunctionKinematic(0.01, -0.1, -0.2, 2., 2., 2.,
                        PolarizationType::LIN_TRANS_X_PLUS,
                        PolarizationType::LIN_TRANS_X_MINUS,
                        PolarizationType::LIN_TRANS_Y_PLUS);

        // Run computation
        List<GPDType> gpdList;
        gpdList.add(GPDType::H);
        gpdList.add(GPDType::Ht);

        GAM2ConvolCoeffFunctionResult cffResult = pGAM2CFFModule->compute(
                cffKinematic, gpdList);

        // Print results for DVCSCFFModule
        Partons::getInstance()->getLoggerManager()->info("main", __func__,
                cffResult.toString());

        // Remove pointer references
        // Module pointers are managed by PARTONS
        Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                pGAM2CFFModule, 0);
        pGAM2CFFModule = 0;

        Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                pGPDModule, 0);
        pGPDModule = 0;

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
