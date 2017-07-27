#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <partons/Partons.h>
#include <partons/services/automation/AutomationService.h>
#include <partons/ServiceObjectRegistry.h>
#include <QtCore/qcoreapplication.h>
#include <string>
#include <vector>

/*
 * Some declarations from examples.cpp
 * Uncomment the one you want to use.
 */
//void computeSingleKinematicsForGPD();
//void computeManyKinematicsForGPD();
//void computeSingleKinematicsForDVCSComptonFormFactor();
//void computeManyKinematicsForDVCSComptonFormFactor();
//void computeSingleKinematicsForDVCSObservable();
//void computeManyKinematicsForDVCSObservable();
//void changeIntegrationRoutine();

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

    QCoreApplication a(argc, argv);
    PARTONS::Partons* pPartons = 0;

    try {

        // Init PARTONS application
        pPartons = PARTONS::Partons::getInstance();
        pPartons->init(argc, argv);

        if (argc <= 1) {
            // If you want to run your C++ code based on PARTONS library, comment out this exception:
            throw ElemUtils::CustomException("main", __func__,
                    "Missing argument, please provide one or more than one XML scenario file.");

            // And include your code here, e.g. this function in examples.cpp:
            // computeSingleKinematicsForGPD(); // It has to be declared before, uncomment the declaration too.

            // The program will then run the code here when there are no XML scenarios in argument.

        } else {
            // You need this to run XML scenarios indicated in arguments of the executable.

            // Parse arguments to retrieve xml file path list.
            std::vector<std::string> xmlScenarioFilePathList = parseArguments(
                    argc, argv);

            // Retrieve automation service parse scenario xml file and play it.
            PARTONS::AutomationService* pAutomationService =
                    pPartons->getServiceObjectRegistry()->getAutomationService();

            for (unsigned int i = 0; i < xmlScenarioFilePathList.size(); i++) {
                PARTONS::Scenario* pScenario = pAutomationService->parseXMLFile(
                        xmlScenarioFilePathList[i]);
                pAutomationService->playScenario(pScenario);
            }
        }

        // If there is an exception
    } catch (const ElemUtils::CustomException &e) {
        pPartons->getLoggerManager()->error(e);
        // Close PARTONS application properly
        if (pPartons) {
            pPartons->close();
        }
    } catch (const std::exception &e) {
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
