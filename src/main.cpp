#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <partons/Partons.h>
#include <partons/services/automation/AutomationService.h>
#include <partons/ServiceObjectRegistry.h>
#include <QtCore/qcoreapplication.h>
#include <string>
#include <vector>

/*
 * Parse XML scenarios.
 */
std::vector<std::string> parseArguments(int argc, char** argv) {
    if (argc <= 1) {
        throw ElemUtils::CustomException("main", __func__,
                "Missing argument, please provide one or more than one XML scenario file.");
    }

    std::vector<std::string> xmlScenarios;

    for (unsigned int i = 1; i != argc; i++) {
        xmlScenarios.push_back(argv[i]);
    }

    return xmlScenarios;
}

/*
 * Main function.
 */
int main(int argc, char** argv) {

    // Parse arguments to retrieve xml file path list.
    std::vector<std::string> xmlScenarioFilePathList = parseArguments(argc,
            argv);

    QCoreApplication a(argc, argv);
    PARTONS::Partons* pPartons = 0;

    try {

        // Init PARTONS application
        pPartons = PARTONS::Partons::getInstance();
        pPartons->init(argc, argv);

        // This you need to run XML scenarios indicated in arguments of the executable

        // Retrieve automation service parse scenario xml file and play it.
        PARTONS::AutomationService* pAutomationService =
                pPartons->getServiceObjectRegistry()->getAutomationService();

        for (unsigned int i = 0; i != xmlScenarioFilePathList.size(); i++) {
            PARTONS::Scenario* pScenario = pAutomationService->parseXMLFile(
                    xmlScenarioFilePathList[i]);
            pAutomationService->playScenario(pScenario);
        }

        // Here you can run your code based on PARTONS library, e.g.
        // computeSingleKinematicsForGPD();

        //TODO to run cpp one need to comment out XML part. But, if we relax argc <= 1 requirement they can be used in parallel.

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
