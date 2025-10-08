#include <string>
#include <vector>
#include <limits>
#include <cmath>

#include <H5Cpp.h>

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>

#include <partons/Partons.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/BaseObjectRegistry.h>

/*
 * Make hdf5 file.
 */
int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "[EE] usage: " << argv[0] << " output_file.hdf5 (quick)" << std::endl;
        return 1;
    }

    PARTONS::Partons* pPartons = 0;

    try {

        // Init PARTONS application
        pPartons = PARTONS::Partons::getInstance();
        pPartons->init(argc, argv);

        // File name
        H5::H5File file(argv[1], H5F_ACC_TRUNC);

        // Mode
        bool quickMode = false;

        if (argc > 2) {
            if (std::string(argv[2]) == "quick") {
                quickMode = true;
            }
        }

        // Get list of modules
        PARTONS::List<std::string> r = PARTONS::Partons::getInstance()->getBaseObjectRegistry()->getBaseObjectClassNames();

        // Loop
        for (int i = 0; i < r.size(); i++) {

            // Skip services
            if (r[i].find("Service") != std::string::npos) {
                continue;
            }

            // Skip certain modules
            if (r[i].find("GPDMPSSW13") != std::string::npos) continue;
            if (r[i].find("GPDSelectOnePartonType") != std::string::npos) continue;
            if (r[i].find("GPDVGG99") != std::string::npos) continue;

            if (r[i].find("DVCSCFFDispersionRelation") != std::string::npos) continue;

            if (r[i].find("TCSCFFFromDVCS") != std::string::npos) continue;
            if (r[i].find("TCSCrossSectionTotal") != std::string::npos) continue;

            // Info
            std::cout << "[II] working for: " << r[i] << std::endl;

            // Result
            std::vector<double> result;

            // In a quick mode, skip certain channels
            if (quickMode &&
                (
                    (r[i].find("DDVCS") != std::string::npos) ||
                    (r[i].find("DVMP") != std::string::npos)
                )
            ) {

                    std::cout << "[II] skipped in quick mode " << std::endl;
                    result.push_back(std::numeric_limits<double>::quiet_NaN());

            } else {

                // Get
                PARTONS::ModuleObject* pModule =
                    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newModuleObject(r[i]);

                // Run
                result = pModule->test();

                // Check if good values
                for (size_t j = 0; j < result.size(); j++) {
                    if (! std::isfinite(result[j])) {

                        std::cout << "[EE] non finite value " << std::endl;

                        throw
                            ElemUtils::CustomException("main", __func__,"NaN or Inf value detected");
                    }
                }
            }

            // Save
            hsize_t dims[1] = { result.size() };
            H5::DataSpace dataspace(1, dims);
            H5::DataSet dataset = file.createDataSet(r[i], H5::PredType::NATIVE_DOUBLE, dataspace);
            dataset.write(result.data(), H5::PredType::NATIVE_DOUBLE);
        }
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

        return 1;
    }
    // In a case of standard exception.
    catch (const std::exception &e) {

        // Display what happened
        pPartons->getLoggerManager()->error("main", __func__, e.what());

        // Close PARTONS application properly
        if (pPartons) {
            pPartons->close();
        }

        return 1;
    }

    // Close PARTONS application properly
    if (pPartons) {
        pPartons->close();
    }

    return 0;
}
