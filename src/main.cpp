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
#include <partons/modules/process/GAM2/GAM2ProcessGPSSW21.h>
#include <partons/modules/xi_converter/GAM2/GAM2XiConverterExact.h>
#include <partons/modules/scales/GAM2/GAM2ScalesMgg2Multiplier.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <QtCore/qcoreapplication.h>
#include <string>
#include <vector>
#include <tuple>

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

        // Create XiConverterModule
        PARTONS::GAM2XiConverterModule* pXiConverterModule =
                PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGAM2XiConverterModule(
                        PARTONS::GAM2XiConverterExact::classId);

        // Create ScalesModule
        PARTONS::GAM2ScalesModule* pScalesModule =
                PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGAM2ScalesModule(
                        PARTONS::GAM2ScalesMgg2Multiplier::classId);

        // Set its lambda parameter, so MuF2 = MuR2 = lambda * Mgg2
        pScalesModule->configure(
                ElemUtils::Parameter(
                        PARTONS::GAM2ScalesMgg2Multiplier::PARAMETER_NAME_LAMBDA,
                        1.));

        // Create ProcessModule
        PARTONS::GAM2ProcessModule* pProcessModule =
                PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGAM2ProcessModule(
                        PARTONS::GAM2ProcessGPSSW21::classId);

        // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
        ElemUtils::Parameters parameters(
                PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                PerturbativeQCDOrderType::LO);

        // Configure DVCSCFFModule with previous parameters.
        pGAM2CFFModule->configure(parameters);

        // Link modules (set physics assumptions of your computation)
        pGAM2CFFModule->setGPDModule(pGPDModule);
        pProcessModule->setScaleModule(pScalesModule);
        pProcessModule->setXiConverterModule(pXiConverterModule);
        pProcessModule->setConvolCoeffFunctionModule(pGAM2CFFModule);

        //GPD
        List<GPDType> gpdList;
        gpdList.add(GPDType::H);
        gpdList.add(GPDType::E);
        gpdList.add(GPDType::Ht);

        //polarization states
        // X and Y PLUS polarizations !
        std::vector<
                std::tuple<PolarizationType::Type, PolarizationType::Type,
                        PolarizationType::Type> > polarizations;

        polarizations.push_back(
                std::make_tuple(PolarizationType::LIN_TRANS_X_PLUS,
                        PolarizationType::LIN_TRANS_X_PLUS,
                        PolarizationType::LIN_TRANS_X_PLUS));
        polarizations.push_back(
                std::make_tuple(PolarizationType::LIN_TRANS_X_PLUS,
                        PolarizationType::LIN_TRANS_Y_MINUS,
                        PolarizationType::LIN_TRANS_X_PLUS));
        polarizations.push_back(
                std::make_tuple(PolarizationType::LIN_TRANS_X_PLUS,
                        PolarizationType::LIN_TRANS_X_PLUS,
                        PolarizationType::LIN_TRANS_Y_MINUS));
        polarizations.push_back(
                std::make_tuple(PolarizationType::LIN_TRANS_X_PLUS,
                        PolarizationType::LIN_TRANS_Y_MINUS,
                        PolarizationType::LIN_TRANS_Y_MINUS));
        polarizations.push_back(
                std::make_tuple(PolarizationType::LIN_TRANS_Y_MINUS,
                        PolarizationType::LIN_TRANS_X_PLUS,
                        PolarizationType::LIN_TRANS_X_PLUS));
        polarizations.push_back(
                std::make_tuple(PolarizationType::LIN_TRANS_Y_MINUS,
                        PolarizationType::LIN_TRANS_Y_MINUS,
                        PolarizationType::LIN_TRANS_X_PLUS));
        polarizations.push_back(
                std::make_tuple(PolarizationType::LIN_TRANS_Y_MINUS,
                        PolarizationType::LIN_TRANS_X_PLUS,
                        PolarizationType::LIN_TRANS_Y_MINUS));
        polarizations.push_back(
                std::make_tuple(PolarizationType::LIN_TRANS_Y_MINUS,
                        PolarizationType::LIN_TRANS_Y_MINUS,
                        PolarizationType::LIN_TRANS_Y_MINUS));


        size_t n = 20;
        double min = -3.;
        double max = -1.;

        double capS = 20.;
        double capMgg2 = 4.; 
        double capM = 0.938272013;

        double xi = (-pow(capM,2) + capS + sqrt(pow(capM,4) + pow(capMgg2 - capS,2) - 2*pow(capM,2)*(capMgg2 + capS)))/(2*pow(capM,2) - capMgg2 + 2*capS);
        xi = -((pow(capM,2) - capS + sqrt(pow(capM,4) + pow(capMgg2 - capS,2) - 2*pow(capM,2)*(capMgg2 + capS)))/(2*pow(capM,2) - capMgg2 + 2*capS));
        double s = ((pow(capM,2) - capS)*(-pow(capM,2) + capMgg2 - 3*capS + sqrt(pow(capM,4) + pow(capMgg2 - capS,2) - 2*pow(capM,2)*(capMgg2 + capS))))/(4.*capS);
        s = -((pow(capM,2) - capS)*(pow(capM,2) - capMgg2 + 3*capS + sqrt(pow(capM,4) + pow(capMgg2 - capS,2) - 2*pow(capM,2)*(capMgg2 + capS))))/(4.*capS);

        double t = - pow(2*xi*capM, 2)/(1. - pow(xi, 2));
        double E = (s - pow(capM, 2)) / (2*capM);

        // Simplified kinematics
//        double xi = capMgg2 / (2. * ( capS - std::pow(capM, 2) ) - capMgg2);
//        double t = - 4. * std::pow( xi * capM , 2) / ( 1. - xi*xi );
//        double E = ( capS - std::pow(capM, 2) ) / 2. / capM;

        std::cout << "xi: " << xi << std::endl;
        std::cout << "t: " << t << std::endl;
        std::cout << "E: " << E << std::endl;

        for (size_t i = 0; i <= n; i++) {

            double uPrim = min + i * (max - min) / double(n);

            double Mgg2 = 4.;

            double result = 0.;

            for (size_t j = 0; j < polarizations.size(); j++) {

                result += 
                pProcessModule->compute(
                        std::get<0>(polarizations.at(j)),
                        std::get<1>(polarizations.at(j)),
                        std::get<2>(polarizations.at(j)),
                        NumA::Vector3D(0., 0., 0.),
                        GAM2ObservableKinematic(t, uPrim, capMgg2, E, 0., 0.), gpdList).getValue().makeSameUnitAs(PhysicalUnit::PB).getValue();
            }

            std::cout << uPrim << "\t" << result/2. << std::endl; // over 2, not 8
        }


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
