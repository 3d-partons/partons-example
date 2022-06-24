#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/parton_distribution/PartonDistribution.h>
#include <partons/beans/parton_distribution/QuarkDistribution.h>
#include <partons/beans/QuarkFlavor.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/services/GPDService.h>
#include <partons/ServiceObjectRegistry.h>
#include <QtCore/qcoreapplication.h>
#include <stddef.h>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "../include/gpd/GPDANNDD.h"
#include "../include/gpd/xEqXi_with_pos_replicas.h"

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
        GPDModule* pGPDModelGK =
                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                        GPDGK16::classId);

        GPDModule* pGPDModelANN =
                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                        GPDANNDD::classId);

        // Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
        GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 2., 2.);

        // Run computation
        GPDResult gpdResultGK = pGPDService->computeSingleKinematic(
                gpdKinematic, pGPDModelGK);

        // Print results
        Partons::getInstance()->getLoggerManager()->info("main", __func__,
                gpdResultGK.toString());

        // Print something  specific
        std::cout << "GPD H for up quarks is: "
                << gpdResultGK.getPartonDistribution(GPDType::H).getQuarkDistribution(
                        QuarkFlavor::UP).getQuarkDistribution() << std::endl;

        // Set replica for ANN
        size_t iReplica = 0;

        std::vector<double> parameters(c_nPar_x_eq_xi_with_pos);

        if (!(iReplica < c_nRep_x_eq_xi_with_pos)) {

            std::cout << "error: " << __func__ << ": wrong replica index, "
                    << iReplica << " (must be smaller than "
                    << c_nRep_x_eq_xi_with_pos << ")" << std::endl;
            exit(0);
        }

        for (size_t i = 0; i < c_nPar_x_eq_xi_with_pos; i++) {
            parameters.at(i) = c_par_x_eq_xi_with_pos[iReplica][i];
            std::cout << "Parameter at " << i << " = " << parameters.at(i) << std::endl ;
        }

        static_cast<GPDANNDD*>(pGPDModelANN)->setParameters(parameters);

        std::cout << "Computing results for ANN replica" << std::endl;

        // Run computation
        GPDResult gpdResultANN = pGPDService->computeSingleKinematic(
                gpdKinematic, pGPDModelANN);

        std::cout << "Printing results for ANN replica" << std::endl;

        // Print results
        Partons::getInstance()->getLoggerManager()->info("main", __func__,
                gpdResultANN.toString());

        std::cout << "End of result printing" << std::endl;


        // Remove pointer references
        // Module pointers are managed by PARTONS
        Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                pGPDModelGK, 0);
        pGPDModelGK = 0;

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

void PrintReplica(){

	  std::ofstream myfile;
	  myfile.open ("./results/Replica.dat");



	  myfile << "Writing this to a file.\n";
	  myfile.close();
	  return 0;

}
