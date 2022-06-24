#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/parton_distribution/PartonDistribution.h>
#include <partons/beans/parton_distribution/QuarkDistribution.h>
#include <partons/beans/QuarkFlavor.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/modules/gpd/GPDMMS13.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/services/GPDService.h>
#include <partons/ServiceObjectRegistry.h>
#include <QtCore/qcoreapplication.h>
#include <stddef.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <gsl/gsl_errno.h>

#include "../include/gpd/GPDANNDD.h"
#include "../include/FTGPD.h"
#include "../include/gpd/xEqXi_with_pos_replicas.h"
#include "../include/statistics.h"

using namespace PARTONS;

/*
 * Main function.
 */
int main(int argc, char** argv) {

    // Init Qt4
    QCoreApplication a(argc, argv);
    Partons* pPartons = 0;

    try {

        //disable gsl error handler
        gsl_set_error_handler_off();

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

        GPDModule* pGPDModelMMS =
                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                        GPDMMS13::classId);

        GPDModule* pGPDModelANN =
                Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                        GPDANNDD::classId);

        // Values of xi, t, and mu2 for which the evaluation will be done
        double xi = 0.5;
        double t = -0.1;
        double mu2 = 2.;

        // Create object for the evaluation of FT
        FTGPD ftGPD;

        // Loop over nu
        double nuMin = 0.;
        double nuMax = 2.;
        size_t nuN = 4;

        for (size_t i = 0; i <= nuN; i++) {

            //this nu
            double nu = nuMin + i * (nuMax - nuMin) / double(nuN);

            //GK
            std::complex<double> resultGK = ftGPD.getFT(pGPDModelGK, nu, xi, t,
                    mu2);

            //MMS
//            std::complex<double> resultMMS = ftGPD.getFT(pGPDModelMMS, nu, xi,
//                    t, mu2);

            //ANN
            std::vector<double> resultANNRe(c_nRep_x_eq_xi_with_pos);
            std::vector<double> resultANNIm(c_nRep_x_eq_xi_with_pos);

            //loop over replicas
            for (size_t j = 0; j < c_nRep_x_eq_xi_with_pos; j++) {

                //loop over parameters
                std::vector<double> parameters(c_nPar_x_eq_xi_with_pos);

                for (size_t k = 0; k < c_nPar_x_eq_xi_with_pos; k++) {
                    parameters.at(k) = c_par_x_eq_xi_with_pos[j][k];
                }

                //load
                static_cast<GPDANNDD*>(pGPDModelANN)->setParameters(parameters);

                //evaluate
                std::complex<double> resultANN = ftGPD.getFT(pGPDModelANN, nu, xi,
                        t, mu2);

                std::cout << nu << ' ' << resultANN << std::endl;

                //copy
                resultANNRe.at(j) = resultANN.real();
                resultANNIm.at(j) = resultANN.imag();
            }

            //remove outliers
            Statistics::removeOutliers3Sigma(resultANNRe);
            Statistics::removeOutliers3Sigma(resultANNIm);

            //print
            std::cout << nu << ' ';
            std::cout << resultGK.imag() << ' ' ;
//            std::cout << resultMMS.imag() << ' ';
            std::cout << Statistics::getMean(resultANNIm) << ' ' << Statistics::getSigma(resultANNIm) << ' ';
            std::cout << std::endl;
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
