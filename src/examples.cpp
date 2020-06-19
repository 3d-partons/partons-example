#include "../include/examples.h"

#include <ElementaryUtils/logger/LoggerManager.h>
#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <NumA/integration/one_dimension/QuadratureIntegrator1D.h>
#include <partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h>
#include <partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/KinematicUtils.h>
#include <partons/beans/List.h>
#include <partons/beans/observable/DVCS/DVCSObservableKinematic.h>
#include <partons/beans/observable/ObservableResult.h>
#include <partons/beans/PerturbativeQCDOrderType.h>
#include <partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsConstant.h>
#include <partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h>
#include <partons/modules/convol_coeff_function/DVCS/DVCSCFFStandard.h>
#include <partons/modules/evolution/gpd/GPDEvolutionVinnikov.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/modules/gpd/GPDGK16Numerical.h>
#include <partons/modules/observable/DVCS/asymmetry/DVCSAllMinus.h>
#include <partons/modules/process/DVCS/DVCSProcessGV08.h>
#include <partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h>
#include <partons/modules/running_alpha_strong/RunningAlphaStrongVinnikov.h>
#include <partons/modules/running_alpha_strong/RunningAlphaStrongApfel.h>
#include <partons/modules/scales/DVCS/DVCSScalesQ2Multiplier.h>
#include <partons/modules/xi_converter/DVCS/DVCSXiConverterXBToXi.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/services/ConvolCoeffFunctionService.h>
#include <partons/services/DVCSConvolCoeffFunctionService.h>
#include <partons/services/DVCSObservableService.h>
#include <partons/services/GPDService.h>
#include <partons/services/ObservableService.h>
#include <partons/ServiceObjectRegistry.h>
#include <partons/utils/type/PhysicalType.h>
#include <partons/utils/type/PhysicalUnit.h>
#include <apfel/apfelxx.h>

void computeRunningAlphaS()
{
  // Create alphaS module for the standard evolution
  PARTONS::RunningAlphaStrongModule* pRunningAlphaStrongModuleStd =
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(PARTONS::RunningAlphaStrongStandard::classId);

  // Create alphaS module for the Vinnikov evolution
  PARTONS::RunningAlphaStrongModule* pRunningAlphaStrongModuleVnk =
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(PARTONS::RunningAlphaStrongVinnikov::classId);

  // Create alphaS module for the APFEL++ evolution
  PARTONS::RunningAlphaStrongModule* pRunningAlphaStrongModuleApf =
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(PARTONS::RunningAlphaStrongApfel::classId);

  // Set scale
  const double Mu2 = 10;

  std::cout << "\nStandard Module: " << pRunningAlphaStrongModuleStd->compute(Mu2) << std::endl;
  std::cout << "Vinnikov Module: " << pRunningAlphaStrongModuleVnk->compute(Mu2) << std::endl;
  std::cout << "APFEL++ Module:  " << pRunningAlphaStrongModuleApf->compute(Mu2) << std::endl;
  std::cout << "\n";

  // Reset pointers
  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pRunningAlphaStrongModuleStd, 0);
  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pRunningAlphaStrongModuleVnk, 0);
  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pRunningAlphaStrongModuleApf, 0);
}

void MyGPDEvolution()
{
  // Retrieve GPD service
  PARTONS::GPDService* pGPDService = PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

  // Create GPD module with the BaseModuleFactory
  PARTONS::GPDModule* pGPDModel = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(PARTONS::GPDGK16::classId);

  // Create GPD evolution module with the BaseModuleFactory
  PARTONS::GPDEvolutionModule* pGPDEvolutionModel =
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDEvolutionModule(PARTONS::GPDEvolutionVinnikov::classId);

  // Create alphaS module with the BaseModuleFactory
  PARTONS::RunningAlphaStrongModule* pRunningAlphaStrongModule =
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(PARTONS::RunningAlphaStrongVinnikov::classId);

  // Create active flavors thresholds module with the
  // BaseModuleFactory
  PARTONS::ActiveFlavorsThresholdsModule* pActiveFlavorsThresholdsModule =
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsThresholdsModule(PARTONS::ActiveFlavorsThresholdsConstant::classId);

  // ActiveFlavorsThresholdsConstant module allows you to set the
  // desired nf value that will be constant during performing the
  // evolution. Default value is nf = 3. You can change it in the
  // following way, but you must be sure that both used GPD model and
  // evolution routine can handle it.
  static_cast<PARTONS::ActiveFlavorsThresholdsConstant*>(pActiveFlavorsThresholdsModule)->setNFlavors(3);

  // Create parameters to configure later GPDEvolutionModule
  ElemUtils::Parameters parameters;

  // Number of steps in the factorization scale (i.e. set the number
  // of steps in the integration over factorization scale). One step
  // is a typical value for Vinnikov code
  parameters.add(NumA::QuadratureIntegrator1D::PARAM_NAME_N, 10);

  // PerturbativeQCD = LO
  parameters.add(PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PARTONS::PerturbativeQCDOrderType::LO);

  // Configure GPDEvolutionModule with previous parameters.
  pGPDEvolutionModel->configure(parameters);

  // Link modules (set physics assumptions of your computation)
  pGPDEvolutionModel->setRunningAlphaStrongModule(pRunningAlphaStrongModule);
  pGPDEvolutionModel->setActiveFlavorsModule(pActiveFlavorsThresholdsModule);
  pGPDModel->setEvolQcdModule(pGPDEvolutionModel);

  // Fixed values of skewness and the momentum transfer
  const double xi = 0.2;
  const double t  = -0.1;

  // initial scale
  const double mu0  = 2;
  const double mu02 = mu0 * mu0;

  // Alpha_s function
  std::function<double(double const&)> Alphas = [=] (double const& mu) -> double{ return pRunningAlphaStrongModule->compute(pow(mu, 2)); };

  // GPDs in the evolution basis
  std::function<std::map<int, double>(double const&, double const&)> GPDs = [=] (double const& x, double const& muF) -> std::map<int, double>
    {
      // Compute results with PARTONS
      const double muF2 = pow(muF, 2);
      const PARTONS::GPDResult gpdResult = pGPDService->computeSingleKinematic(PARTONS::GPDKinematic{x, xi, t, muF2, muF2}, pGPDModel);

      // Get H GPDs
      const PARTONS::PartonDistribution d = gpdResult.getPartonDistributions().at(PARTONS::GPDType::H);

      // Retrieve single distributions
      const double Gluon  = x * d.getGluonDistribution().getGluonDistribution();
      const double UPlus  = x * d.getListOfQuarkDistribution()[0].getQuarkDistributionPlus();
      const double UMinus = x * d.getListOfQuarkDistribution()[0].getQuarkDistributionMinus();
      const double DPlus  = x * d.getListOfQuarkDistribution()[1].getQuarkDistributionPlus();
      const double DMinus = x * d.getListOfQuarkDistribution()[1].getQuarkDistributionMinus();
      const double SPlus  = x * d.getListOfQuarkDistribution()[2].getQuarkDistributionPlus();
      const double SMinus = x * d.getListOfQuarkDistribution()[2].getQuarkDistributionMinus();

      // Construct evolution basis
      const double Singlet = UPlus + DPlus + SPlus;
      const double Valence = UMinus + DMinus + SMinus;
      const double T3      = UPlus - DPlus;
      const double V3      = UMinus - DMinus;
      const double T8      = UPlus + DPlus - 2 * SPlus;
      const double V8      = UMinus + DMinus - 2 * SMinus;

      // Fill in map in the QCD evolution basis.
      std::map<int, double> QCDEvMap;
      QCDEvMap[0]  = Gluon;
      QCDEvMap[1]  = Singlet;
      QCDEvMap[2]  = Valence;
      QCDEvMap[3]  = T3;
      QCDEvMap[4]  = V3;
      QCDEvMap[5]  = T8;
      QCDEvMap[6]  = V8;
      QCDEvMap[7]  = Singlet;
      QCDEvMap[8]  = Valence;
      QCDEvMap[9]  = Singlet;
      QCDEvMap[10] = Valence;
      QCDEvMap[11] = Singlet;
      QCDEvMap[12] = Valence;

      return QCDEvMap;
    };

  // Set verbosity level of APFEL++ to minimum
  apfel::SetVerbosityLevel(0);

  // APFEL++ x-space grid
  const apfel::Grid g{{apfel::SubGrid{200, 1e-3, 3}, apfel::SubGrid{200, 2e-1, 3}, apfel::SubGrid{200, 4e-1, 3}, apfel::SubGrid{200, 8e-1, 3}}};

  // Vector of thresholds (3-flavour scheme)
  const std::vector<double> Thresholds = {0, 0, 0};

  // Perturbative order
  const int PerturbativeOrder = 0;

  // Initialize GPD evolution objects
  const auto GpdObj = apfel::InitializeGpdObjects(g, Thresholds, xi);

  // Also initialise DGLAP objects for checking purposes
  const auto DglapObj = apfel::InitializeDglapObjectsQCD(g, Thresholds);

  // Construct the DGLAP objects
  const auto EvolvedGPDs = BuildDglap(GpdObj, GPDs, mu0, PerturbativeOrder, Alphas);
  const auto EvolvedPDFs = BuildDglap(DglapObj, GPDs, mu0, 1, Alphas);

  // Tabulate GPDs
  const apfel::TabulateObject<apfel::Set<apfel::Distribution>> TabulatedGPDs{*EvolvedGPDs, 50, 1, 10, 3};
  const apfel::TabulateObject<apfel::Set<apfel::Distribution>> TabulatedPDFs{*EvolvedPDFs, 50, 1, 10, 3};

  // Final scale
  const double mu  = 3;
  const double mu2 = pow(mu, 2);

  // Evolve GPDs and PDFs to the final Scale and rotate them to the physical
  // basis
  const std::map<int, apfel::Distribution> gpds = apfel::QCDEvToPhys(EvolvedGPDs->Evaluate(mu).GetObjects());
  const std::map<int, apfel::Distribution> pdfs = apfel::QCDEvToPhys(EvolvedPDFs->Evaluate(mu).GetObjects());

  // Test Values of x
  const int nx = 500;
  const double xmin = 1e-3;
  const double xmax = 0.9;
  const double xstp = exp( log(xmax / xmin) / ( nx - 1 ) );
  std::vector<double> xlha;

  // Initialise list of kinematic configurations
  PARTONS::List<PARTONS::GPDKinematic> gpdKinematicList;
  for (double x = xmin; x <= xmax; x *= xstp)
    {
      gpdKinematicList.add(PARTONS::GPDKinematic{x, xi, t, mu2, mu2});
      xlha.push_back(x);
    }

  // Run computation
  PARTONS::List<PARTONS::GPDResult> gpdResultList = pGPDService->computeManyKinematic(gpdKinematicList, pGPDModel);

  // Print results
  std::cout << std::scientific << "\n";
  for (int i = 0; i < (int) xlha.size(); i++)
    {
      // Get H GPDs
      const PARTONS::PartonDistribution f = gpdResultList[i].getPartonDistributions().at(PARTONS::GPDType::H);

      // Print results
      //std::cout.precision(1);
      std::cout << mu << "  ";
      std::cout << xlha[i] << "  ";
      //std::cout.precision(4);
      std::cout << pdfs.at(1).Evaluate(xlha[i]) - pdfs.at(-1).Evaluate(xlha[i])<< "  "
		<< gpds.at(1).Evaluate(xlha[i]) - gpds.at(-1).Evaluate(xlha[i])<< "  "
		<< xlha[i] * f.getListOfQuarkDistribution()[1].getQuarkDistributionMinus() << "  "
		<< pdfs.at(1).Evaluate(xlha[i]) + pdfs.at(-1).Evaluate(xlha[i])<< "  "
		<< gpds.at(1).Evaluate(xlha[i]) + gpds.at(-1).Evaluate(xlha[i])<< "  "
		<< xlha[i] * f.getListOfQuarkDistribution()[1].getQuarkDistributionPlus() << "  "
                << std::endl;
    }

  // Print results
  //PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__, gpdResultList.toString());

  // Remove pointer references. Module pointers are managed by PARTONS
  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pActiveFlavorsThresholdsModule, 0);
  pGPDModel = 0;

  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pRunningAlphaStrongModule, 0);
  pGPDModel = 0;

  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pGPDEvolutionModel, 0);
  pGPDModel = 0;

  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pGPDModel, 0);
  pGPDModel = 0;
}

void computeSingleKinematicsForGPD() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDGK16::classId);

    // Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
    PARTONS::GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 2., 2.);

    // Run computation
    PARTONS::GPDResult gpdResult = pGPDService->computeSingleKinematic(
            gpdKinematic, pGPDModel);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResult.toString());

    // Remove pointer references
    // Module pointers are managed by PARTONS
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}

void computeManyKinematicsForGPD() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDGK16::classId);

    // Load list of kinematics from file
    PARTONS::List<PARTONS::GPDKinematic> gpdKinematicList =
            PARTONS::KinematicUtils().getGPDKinematicFromFile(
                    "/home/partons/git/partons-example/data/examples/gpd/kinematics_gpd.csv");

    // Run computation
    PARTONS::List<PARTONS::GPDResult> gpdResultList =
            pGPDService->computeManyKinematic(gpdKinematicList, pGPDModel);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResultList.toString());

    // Remove pointer references
    // Module pointers are managed by PARTONS
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}

void computeSingleKinematicsForDVCSComptonFormFactor() {

    // Retrieve service
    PARTONS::DVCSConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getDVCSConvolCoeffFunctionService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDGK16::classId);

    // Create CFF module with the BaseModuleFactory
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
    ElemUtils::Parameters parameters(
            PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PARTONS::PerturbativeQCDOrderType::LO);

    // Configure DVCSCFFModule with previous parameters.
    pDVCSCFFModule->configure(parameters);

    // Link modules (set physics assumptions of your computation)
    pDVCSCFFModule->setGPDModule(pGPDModule);

    // Create kinematic
    PARTONS::DVCSConvolCoeffFunctionKinematic cffKinematic =
            PARTONS::DVCSConvolCoeffFunctionKinematic(0.01, -0.1, 4., 4., 4.);

    // Run computation
    PARTONS::DVCSConvolCoeffFunctionResult cffResult =
            pDVCSConvolCoeffFunctionService->computeSingleKinematic(
                    cffKinematic, pDVCSCFFModule);

    // Print results for DVCSCFFModule
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            cffResult.toString());

    // Remove pointer references
    // Module pointers are managed by PARTONS
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModule, 0);
    pDVCSCFFModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}

void computeManyKinematicsForDVCSComptonFormFactor() {

    // Retrieve service
    PARTONS::DVCSConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getDVCSConvolCoeffFunctionService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDGK16::classId);

    // Create CFF module with the BaseModuleFactory
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
    ElemUtils::Parameters parameters(
            PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PARTONS::PerturbativeQCDOrderType::LO);

    // Configure DVCSCFFModule with previous parameters.
    pDVCSCFFModule->configure(parameters);

    // Link modules (set physics assumptions of your computation)
    pDVCSCFFModule->setGPDModule(pGPDModule);

    // Load list of kinematics from file
    PARTONS::List<PARTONS::DVCSConvolCoeffFunctionKinematic> cffKinematicList =
            PARTONS::KinematicUtils().getDVCSCCFKinematicFromFile(
                    "/home/partons/git/partons-example/data/examples/cff/kinematics_dvcs_cff.csv");

    // Run computation
    PARTONS::List<PARTONS::DVCSConvolCoeffFunctionResult> cffResultList =
            pDVCSConvolCoeffFunctionService->computeManyKinematic(
                    cffKinematicList, pDVCSCFFModule);

    // Print results for DVCSCFFModule
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            cffResultList.toString());

    // Remove pointer references
    // Module pointers are managed by PARTONS
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModule, 0);
    pDVCSCFFModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}

void computeSingleKinematicsForDVCSObservable() {

    // Retrieve Observable service
    PARTONS::DVCSObservableService* pObservableService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getDVCSObservableService();

    // Create GPDModule
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDGK16::classId);

    // Create CFF module
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Set its PerturbativeQCDOrder
    pDVCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    // Create XiConverterModule
    PARTONS::DVCSXiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSXiConverterModule(
                    PARTONS::DVCSXiConverterXBToXi::classId);

    // Create ScalesModule
    PARTONS::DVCSScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSScalesModule(
                    PARTONS::DVCSScalesQ2Multiplier::classId);

    // Set its lambda parameter, so MuF2 = MuR2 = lambda * Q2
    pScalesModule->configure(
            ElemUtils::Parameter(
                    PARTONS::DVCSScalesQ2Multiplier::PARAMETER_NAME_LAMBDA,
                    1.));

    // Create ProcessModule
    PARTONS::DVCSProcessModule* pProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(
                    PARTONS::DVCSProcessGV08::classId);

    // Create Observable
    PARTONS::DVCSObservable* pObservable =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSObservable(
                    PARTONS::DVCSAllMinus::classId);

    // Link modules (set physics assumptions of your computation)
    pObservable->setProcessModule(pProcessModule);
    pProcessModule->setScaleModule(pScalesModule);
    pProcessModule->setXiConverterModule(pXiConverterModule);
    pProcessModule->setConvolCoeffFunctionModule(pDVCSCFFModel);
    pDVCSCFFModel->setGPDModule(pGPDModule);

    // Load list of kinematics from file
    PARTONS::DVCSObservableKinematic observableKinematic =
            PARTONS::DVCSObservableKinematic(0.2, -0.1, 2., 6., 0.);

    // Create kinematic
    PARTONS::DVCSObservableResult observableResult =
            pObservableService->computeSingleKinematic(observableKinematic,
                    pObservable);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            observableResult.toString());

    // Remove pointer references
    // Module pointers are managed by PARTONS
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModel, 0);
    pDVCSCFFModel = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pXiConverterModule, 0);
    pXiConverterModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pScalesModule, 0);
    pScalesModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pObservable, 0);
    pObservable = 0;
}

void computeManyKinematicsForDVCSObservable() {

    // Retrieve Observable service
    PARTONS::DVCSObservableService* pObservableService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getDVCSObservableService();

    // Create GPDModule
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDGK16::classId);

    // Create CFF module
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Set its PerturbativeQCDOrder
    pDVCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::LO));

    // Create XiConverterModule
    PARTONS::DVCSXiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSXiConverterModule(
                    PARTONS::DVCSXiConverterXBToXi::classId);

    // Create ScalesModule
    PARTONS::DVCSScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSScalesModule(
                    PARTONS::DVCSScalesQ2Multiplier::classId);

    // Set its lambda parameter, so MuF2 = MuR2 = lambda * Q2
    pScalesModule->configure(
            ElemUtils::Parameter(
                    PARTONS::DVCSScalesQ2Multiplier::PARAMETER_NAME_LAMBDA,
                    1.));

    // Create ProcessModule
    PARTONS::DVCSProcessModule* pProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(
                    PARTONS::DVCSProcessGV08::classId);

    // Create Observable
    PARTONS::DVCSObservable* pObservable =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSObservable(
                    PARTONS::DVCSAllMinus::classId);

    // Link modules (set physics assumptions of your computation)
    pObservable->setProcessModule(pProcessModule);
    pProcessModule->setScaleModule(pScalesModule);
    pProcessModule->setXiConverterModule(pXiConverterModule);
    pProcessModule->setConvolCoeffFunctionModule(pDVCSCFFModel);
    pDVCSCFFModel->setGPDModule(pGPDModule);

    // Load list of kinematics from file
    PARTONS::List<PARTONS::DVCSObservableKinematic> observableKinematicList =
            PARTONS::KinematicUtils().getDVCSObservableKinematicFromFile(
                    "/home/partons/git/partons-example/data/examples/observable/kinematics_dvcs_observable.csv");

    // Run computation
    PARTONS::List<PARTONS::DVCSObservableResult> observableResultList =
            pObservableService->computeManyKinematic(observableKinematicList,
                    pObservable);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            observableResultList.toString());

    // Remove pointer references
    // Module pointers are managed by PARTONS
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModel, 0);
    pDVCSCFFModel = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pXiConverterModule, 0);
    pXiConverterModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pScalesModule, 0);
    pScalesModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pObservable, 0);
    pObservable = 0;
}

void changeIntegrationRoutine() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDGK16Numerical::classId);

    // Change integration routine
    pGPDModel->configure(
            ElemUtils::Parameter(
                    PARTONS::MathIntegratorModule::PARAM_NAME_INTEGRATOR_TYPE,
                    NumA::IntegratorType1D::GK21_ADAPTIVE));

    // Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
    PARTONS::GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 2., 2.);

    // Run computation
    PARTONS::GPDResult gpdResult = pGPDService->computeSingleKinematic(
            gpdKinematic, pGPDModel);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResult.toString());

    // Remove pointer references
    // Module pointers are managed by PARTONS
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}

void makeUseOfGPDEvolution()
{
  // Retrieve GPD service
  PARTONS::GPDService* pGPDService = PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

  // Create GPD module with the BaseModuleFactory
  PARTONS::GPDModule* pGPDModel = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(PARTONS::GPDGK16::classId);

  // Create GPD evolution module with the BaseModuleFactory
  PARTONS::GPDEvolutionModule* pGPDEvolutionModel =
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDEvolutionModule(PARTONS::GPDEvolutionVinnikov::classId);

  // Create alphaS module with the BaseModuleFactory
  PARTONS::RunningAlphaStrongModule* pRunningAlphaStrongModule =
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(PARTONS::RunningAlphaStrongVinnikov::classId);

  // Create active flavors thresholds module with the
  // BaseModuleFactory
  PARTONS::ActiveFlavorsThresholdsModule* pActiveFlavorsThresholdsModule =
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsThresholdsModule(PARTONS::ActiveFlavorsThresholdsConstant::classId);

  // ActiveFlavorsThresholdsConstant module allows you to set the
  // desired nf value that will be constant during performing the
  // evolution. Default value is nf = 3. You can change it in the
  // following way, but you must be sure that both used GPD model and
  // evolution routine can handle it.
  static_cast<PARTONS::ActiveFlavorsThresholdsConstant*>(pActiveFlavorsThresholdsModule)->setNFlavors(3);

  // Create parameters to configure later GPDEvolutionModule
  ElemUtils::Parameters parameters;

  // Number of steps in the factorization scale (i.e. set the number
  // of steps in the integration over factorization scale). One step
  // is a typical value for Vinnikov code
  parameters.add(NumA::QuadratureIntegrator1D::PARAM_NAME_N, 10);

  // PerturbativeQCD = LO
  parameters.add(PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PARTONS::PerturbativeQCDOrderType::LO);

  // Configure GPDEvolutionModule with previous parameters.
  pGPDEvolutionModel->configure(parameters);

  // Link modules (set physics assumptions of your computation)
  pGPDEvolutionModel->setRunningAlphaStrongModule(pRunningAlphaStrongModule);
  pGPDEvolutionModel->setActiveFlavorsModule(pActiveFlavorsThresholdsModule);
  pGPDModel->setEvolQcdModule(pGPDEvolutionModel);

  // Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
  PARTONS::GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 20., 20.);

  // Run computation
  PARTONS::GPDResult gpdResult = pGPDService->computeSingleKinematic(gpdKinematic, pGPDModel);

  // Print results
  PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__, gpdResult.toString());

  // Remove pointer references. Module pointers are managed by PARTONS
  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pActiveFlavorsThresholdsModule, 0);
  pGPDModel = 0;

  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pRunningAlphaStrongModule, 0);
  pGPDModel = 0;

  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pGPDEvolutionModel, 0);
  pGPDModel = 0;

  PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pGPDModel, 0);
  pGPDModel = 0;
}

void selectSpecificGPDTypes() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDGK16::classId);

    // Create a list of GPD types you want to compute for
    PARTONS::List<PARTONS::GPDType> gpdTypelist;

    gpdTypelist.add(PARTONS::GPDType::E);
    gpdTypelist.add(PARTONS::GPDType::Et);

    // Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
    PARTONS::GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 2., 2.);

    // Run computation
    PARTONS::GPDResult gpdResult = pGPDService->computeSingleKinematic(
            gpdKinematic, pGPDModel, gpdTypelist);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResult.toString());

    // Remove pointer references
    // Module pointers are managed by PARTONS
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}

void demonstrateUnits() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDGK16::classId);

    // Kinematics
    PARTONS::PhysicalType<double> x(0.1, PARTONS::PhysicalUnit::NONE);
    PARTONS::PhysicalType<double> xi(0.2, PARTONS::PhysicalUnit::NONE);
    PARTONS::PhysicalType<double> t(-0.1, PARTONS::PhysicalUnit::GEV2);
    PARTONS::PhysicalType<double> muF2(2., PARTONS::PhysicalUnit::GEV2);
    PARTONS::PhysicalType<double> muR2(2., PARTONS::PhysicalUnit::GEV2);

    PARTONS::PhysicalType<double> tInMeV2_a = t.makeSameUnitAs(
            PARTONS::PhysicalUnit::MEV2);

    PARTONS::PhysicalType<double> tInMeV2_b = t;
    tInMeV2_b.makeSameUnitAs(tInMeV2_a);

    // Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
    PARTONS::GPDKinematic gpdKinematic(x, xi, tInMeV2_a, muF2, muF2);

    // Run computation
    PARTONS::GPDResult gpdResult = pGPDService->computeSingleKinematic(
            gpdKinematic, pGPDModel);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResult.toString());

    // Remove pointer references
    // Module pointers are managed by PARTONS
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}
