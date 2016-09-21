#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <partons/Partons.h>
#include <partons/services/automation/AutomationService.h>
#include <partons/ServiceObjectRegistry.h>
#include <QtCore/qcoreapplication.h>
#include <string>
#include <vector>

std::vector<std::string> parseArguments(int argc, char** argv) {
    if (argc <= 1) {
        throw std::runtime_error(
                "Missing argument, please provide one or more than one XML scenario file.");
    }

    std::vector<std::string> xmlScenarios;

    for (unsigned int i = 1; i != argc; i++) {
        xmlScenarios.push_back(argv[i]);
    }

    return xmlScenarios;
}

int main(int argc, char** argv) {

    // Parse arguments to retrieve xml file path list.
    std::vector<std::string> xmlScenarioFilePathList = parseArguments(argc,
            argv);

    QCoreApplication a(argc, argv);
    Partons* pPartons = 0;

    try {
        // Init PARTONS application
        pPartons = Partons::getInstance();
        pPartons->init(argc, argv);

        // Retrieve automation service parse scenario xml file and play it.
        AutomationService* pAutomationService =
                pPartons->getServiceObjectRegistry()->getAutomationService();

        for (unsigned int i = 0; i != xmlScenarioFilePathList.size(); i++) {
            Scenario* pScenario = pAutomationService->parseXMLFile(
                    xmlScenarioFilePathList[i]);
            pAutomationService->playScenario(pScenario);
        }
        // If there is an exception
    } catch (const ElemUtils::CustomException &e) {
        pPartons->getLoggerManager()->error(e);
    } catch (const std::exception &e) {
        pPartons->getLoggerManager()->error("main", __func__, e.what());
    }

    // Close PARTONS application propertly
    if (pPartons) {
        pPartons->close();
        delete pPartons;
        pPartons = 0;
    }

    return 0;
}
