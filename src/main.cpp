#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/services/GPDService.h>
#include <partons/ServiceObjectRegistry.h>
#include <QtCore/qcoreapplication.h>

using namespace PARTONS;

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

        // Retrieve GPD service
        GPDService* pGPDService =
                Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

        // Create GPD module with the BaseModuleFactory
        GPDModule* pGPDModel =
                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                        GPDGK16::classId);

        // Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
        GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 2., 2.);

        // Run computation
        GPDResult gpdResult = pGPDService->computeSingleKinematic(gpdKinematic,
                pGPDModel);

        // Print results
        Partons::getInstance()->getLoggerManager()->info("main", __func__,
                gpdResult.toString());

        // Print something more specific
        std::cout << "GPD H for up quarks is: "
                << gpdResult.getPartonDistribution(GPDType::H).getQuarkDistribution(
                        QuarkFlavor::UP).getQuarkDistribution() << std::endl;

        // Remove pointer references
        // Module pointers are managed by PARTONS
        Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                pGPDModel, 0);
        pGPDModel = 0;

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
