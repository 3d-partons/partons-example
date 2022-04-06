#include <ElementaryUtils/file_utils/FileUtils.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

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
#include <partons/modules/gpd/GPDGK11.h>
#include <partons/modules/gpd/GPDGK16.h>
#include <partons/modules/gpd/GPDGK16Numerical.h>


#include <partons/modules/gpd/pionGPD/Simple_RDDA.h>
//#include <partons/modules/gpd/pionGPD/Gegenbauer_Model.h>

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

#include <partons/modules/evolution/gpd/GPDEvolutionApfel.h>
#include <partons/modules/running_alpha_strong/RunningAlphaStrongApfel.h>

#include <apfel/apfelxx.h>
#include <apfel/betaqcd.h>
#include <math.h>

#include "include/Conformal_Moments.h"


ConformalMoments::ConformalMoments():
					m_nGegenbauer(4.),
					m_mu2(10.),
					m_mu2ref(1.),
					m_xi(1.),
					m_t(0.),
					m_ConfMom(-1.),
					m_pProductFunctor(0),
					m_pIntegrator(0),
					m_ModelPara(0),
					m_pGPDService(0),
					m_pGPDModel(0),
					m_pGPDResult(0),
					m_pGPDEvolutionModel(0),
					m_pActiveFlavorsThresholdsModule(0),
					m_pRunningAlphaStrongModule(0),
					m_integratorType(NumA::IntegratorType1D::GK21_ADAPTIVE)
				    {
						ConformalMoments::setPARTONS();

						ConformalMoments::setEvolution();

						//m_px2Function = new this();

						//m_px2Functor = NumA::Integrator1D::newIntegrationFunctor(m_px2Function,
						             //       &Evolution_of_DA::x2Function::getx2Function);
				        m_pProductFunctor = NumA::Integrator1D::newIntegrationFunctor(this,&ConformalMoments::ProductGegenbauerGPD);

						m_pIntegrator = NumA::Integrator1D::newIntegrator(m_integratorType);


				    }

ConformalMoments::~ConformalMoments(){

	//	delete m_px2Function;
	//	delete m_px2Functor;
		delete m_pIntegrator;
		delete m_pGPDResult;

        // Module pointers are managed by PARTONS
        PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                m_pActiveFlavorsThresholdsModule, 0);

        PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                m_pRunningAlphaStrongModule, 0);

        PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                m_pGPDEvolutionModel, 0);

        PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                m_pGPDModel, 0);

		delete m_pGPDService;

	}



void ConformalMoments::setPARTONS()
{
	//PARTONS Initialisation

	// Retrieve GPD service
	m_pGPDService = PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();
	//Create GPD module with the BaseModuleFactory
	m_pGPDModel =  PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
		                        PARTONS::simpleRDDAModel::classId);
	//Create GPD evolution module with the BaseModuleFactory
	m_pGPDEvolutionModel = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDEvolutionModule(
		                        PARTONS::GPDEvolutionApfel::classId);
	//Choose Alpha_Strong Module
	m_pRunningAlphaStrongModule = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newRunningAlphaStrongModule(
		                        PARTONS::RunningAlphaStrongApfel::classId);
	// Create active flavors thresholds module with the BaseModuleFactory
	m_pActiveFlavorsThresholdsModule = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newActiveFlavorsThresholdsModule(
		                        PARTONS::ActiveFlavorsThresholdsConstant::classId);

	// ActiveFlavorsThresholdsConstant module allows you to set the desired nf value that will be constant during performing the evolution.
	// Default value is nf = 3. You can change it in the following way, but you must be sure that both used GPD model and evolution routine can handle it.
	static_cast<PARTONS::ActiveFlavorsThresholdsConstant*>(m_pActiveFlavorsThresholdsModule)->setNFlavors(3);

		        // Set GPD type
		        PARTONS::List<PARTONS::GPDType> gpdTypeList;
		        gpdTypeList.add(PARTONS::GPDType::H);

}

void ConformalMoments::setEvolution()
{
    // Create parameters to configure later GPDEvolutionModule
    ElemUtils::Parameters parameters;

    // Number of steps in the factorization scale (i.e. set the number of steps in the integration over factorization scale)
    // One step is a typical value for Vinnikov code
    parameters.add(NumA::QuadratureIntegrator1D::PARAM_NAME_N, 10);

    //Test scale
    std::cout << "ref scale = " << m_mu2ref << std::endl ;

    // PerturbativeQCD = LO
    parameters.add(
            PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PARTONS::PerturbativeQCDOrderType::LO);


    // Configure evolution module
            // Thresholds
            //parameters.add(PARTONS::GPDEvolutionApfel::PARAM_NAME_THRESHOLDS, "0 0 0" ); //"0 0 0 1.41 4 175"
            //parameters.add(PARTONS::GPDEvolutionApfel::PARAM_NAME_MASSES, "0 0 0" ); //"0 0 0 1.41 4 175"
            //std::cout << "Threshold set" << std::endl ;

            // Grid parameters
            parameters.add(PARTONS::GPDEvolutionApfel::PARAM_NAME_SUBGRID_NODES, "200 200 200 100" );
            parameters.add(PARTONS::GPDEvolutionApfel::PARAM_NAME_SUBGRID_LOWER_BOUNDS, "1e-8 9e-3 5e-1 9.9e-1" );
            parameters.add(PARTONS::GPDEvolutionApfel::PARAM_NAME_SUBGRID_INTER_DEGREES, "3 3 3 3");
            std::cout << "Grid Parameters Set" << std::endl;

            //
            parameters.add(PARTONS::GPDEvolutionApfel::PARAM_NAME_TAB_NODES, 100 );
            parameters.add(PARTONS::GPDEvolutionApfel::PARAM_NAME_TAB_LOWER_BOUND, 1. ); // to be adapted to the lowest value of the scale?
            parameters.add(PARTONS::GPDEvolutionApfel::PARAM_NAME_TAB_UPPER_BOUND, 100. );
            parameters.add(PARTONS::GPDEvolutionApfel::PARAM_NAME_TAB_INTER_DEGREE, 3 );
            std::cout << "Evolution Parameters set" << std::endl ;

    // Configure GPDEvolutionModule with previous parameters.
    m_pGPDEvolutionModel->configure(parameters);

    m_pGPDEvolutionModel->setRunningAlphaStrongModule(m_pRunningAlphaStrongModule);
    m_pGPDEvolutionModel->setActiveFlavorsModule(m_pActiveFlavorsThresholdsModule);
    m_pGPDModel->setEvolQcdModule(m_pGPDEvolutionModel);

    //set Alpha_S in Apfel
	parameters.add(PARTONS::RunningAlphaStrongApfel::PARAM_NAME_ALPHAS_REF, 0.513993) ;
	parameters.add(PARTONS::RunningAlphaStrongApfel::PARAM_NAME_MU_REF, 1.) ;
	parameters.add(PARTONS::RunningAlphaStrongApfel::PARAM_NAME_THRESHOLDS, "0 0 0") ;
    m_pRunningAlphaStrongModule->configure(parameters) ;

}

void ConformalMoments::setXi(double xi)
{
	m_xi = xi ;
}

void ConformalMoments::setMu2(double mu2)
{
	m_mu2 = mu2 ;
}

void ConformalMoments::print2DConformalMoments(double mu2)
{
	setMu2(mu2);

	ComputeConformalMoments();
}

double ConformalMoments::getAnomalousDimension()
{
    //Compute Anomalous Dimension
    double Vn = - 0.5 + 1. / ( m_nGegenbauer + 1 ) / ( m_nGegenbauer + 2 );
    for (int k = 2; k <= m_nGegenbauer + 1; k++)
      Vn -= 2. / k;
    Vn *= 2 * apfel::CF;
    const double b0 = apfel::beta0qcd(3);
    const double GammaERBL = pow(m_pRunningAlphaStrongModule->compute(m_mu2) / m_pRunningAlphaStrongModule->compute(m_mu2ref), - Vn / b0);
    return GammaERBL ;
}



double ConformalMoments::ProductGegenbauerGPD(double x, std::vector<double> Para)
{
	double GegenbauerTemp = pow(m_xi,m_nGegenbauer) * boost::math::gegenbauer(m_nGegenbauer, 1.5, x/m_xi);

	PARTONS::GPDResult gpdResult = m_pGPDService->computeSingleKinematic(PARTONS::GPDKinematic{x,m_xi,m_t,m_mu2,m_mu2}, m_pGPDModel);
	double GPDTemp = gpdResult.getPartonDistribution(PARTONS::GPDType::H).getQuarkDistribution(PARTONS::QuarkFlavor::UP).getQuarkDistributionMinus();

	if(isnan(GPDTemp )){
		std::cout << "Warning GPD evaluated to NaN for x = " << x << " and xi = " << m_xi << std::endl;
		std::cout << "NaN values appears for x larger than the highest Apfel value" << std::endl;
		std::cout << "In ConformalMoments::ProductGegenbauerGPD GPD set to 0" << std::endl ;
		GPDTemp = 0. ;
	}

	/*
	std::cout << "x = " <<  x << std::endl ;
	std::cout << "x / xi = " <<  x/m_xi << std::endl ;
	std::cout << "m_nGegenbauer = " << m_nGegenbauer << std::endl ;
	std::cout << "Gegenbauer = " << GegenbauerTemp << std::endl ;
	std::cout << "GPD = " << GPDTemp << std::endl ;
	*/
	if (isnan(GegenbauerTemp * GPDTemp ))
	{
		std::cout << "nan values found !! "<< std::endl ;
		std::cout << "x = " << x << std::endl;
		std::cout << "GPD = " << GPDTemp << std::endl ;
		std::cout << " Gegenbauer = " << GegenbauerTemp << std::endl ;
	}
	return GegenbauerTemp * GPDTemp ;
}


void ConformalMoments::ComputeConformalMoments()
{
	    	        std::cout << "LO is supposed to be set, let us try to run alpha_s" << std::endl ;
	    	        std::cout << "alpha_s(1.) = " << m_pRunningAlphaStrongModule->compute(1.) << std::endl ;
	    	        std::cout << "alpha_s(10.) = " << m_pRunningAlphaStrongModule->compute(10.) << std::endl ;


	        //set xi and t
	        const double t = 0.;

	        //Exporting results
	        const int nmu = 2 ;
	        const double mu2min = 1. ;
	        const double mu2max = m_mu2 ;
	        const double mu2stp = exp( log(mu2max / mu2min) / (nmu));

	        const int nxi = 2;
	        const double ximin = 5e-2;
	        const double ximax = 1.;
	        const double xistp = exp( log(ximax / ximin) / (nxi -1 ));
	        std::vector<double> refConfMom = {};
	        double dummyConfMom ;

	        double GammaERBL = getAnomalousDimension();

	        std::cout << "alpha_s (m_mu2) = " << m_pRunningAlphaStrongModule->compute(m_mu2) << " alpha_s(m_mu2ref) = " << m_pRunningAlphaStrongModule->compute(m_mu2ref) << std::endl ;

	        //Computation of Conformal Moments

	        std::vector<double> err_Para = {1e-6} ;// last element of parameters is  target error of  integration routine
	        double integrationResult = m_pIntegrator->integrate(m_pProductFunctor,0., 1.,err_Para);
	        m_ConfMom =  integrationResult ;

			//Preparing output file

			ElemUtils::FileUtils* outputFile;
	        std::ofstream evolutionOutputFile_gegenbauerModel;
	        outputFile->open( evolutionOutputFile_gegenbauerModel,
	        		"/local/home/cmezrag/git/partons-example/results/evolution/ConformalMomentC4.dat", std::ios_base::out );


			//Running computations

	        //Reference scale Moment
	        m_mu2 = m_mu2ref  ;
    		for ( double m_txi = ximin; m_txi <= ximax; m_txi *= xistp )
    		{
    			m_xi = m_txi ;
    			refConfMom.push_back( ( m_pIntegrator->integrate(m_pProductFunctor,0. , 1.,err_Para) )   );

    		}

    	/*	double GPDTemp ;
    		for (double x = 0.99; x <=1 ; x+= 0.001 ){
    			PARTONS::GPDResult gpdResult = m_pGPDService->computeSingleKinematic(PARTONS::GPDKinematic{x,0.001 ,0. ,2.,2.}, m_pGPDModel);
    			GPDTemp = gpdResult.getPartonDistribution(PARTONS::GPDType::H).getQuarkDistribution(PARTONS::QuarkFlavor::UP).getQuarkDistributionMinus();
    			std::cout <<"x = " << x << "  GPD = " << GPDTemp <<std::endl ;
    		}

    		m_mu2 = 2  ;
    		for ( double m_txi = ximin; m_txi <= ximax; m_txi *= xistp )
    		{
    			m_xi = m_txi ;
    			std::cout << "Conf Mom = " <<  m_pIntegrator->integrate(m_pProductFunctor, 0. , 1.,err_Para) << std::endl ;

    		}
    		*/

    		//Evolution
	        for ( double m_tmu2 = mu2min ; m_tmu2 <= mu2max ; m_tmu2 *= mu2stp)
	        {
	        	m_mu2 = m_tmu2 ;
	        	GammaERBL = getAnomalousDimension() ;
	        	std::cout << "mu2 = " << m_mu2 << std::endl ;

	        		// Starting Timer
	        		auto start = std::chrono::high_resolution_clock::now();

	        		int iConfMom = 0;
	        		for ( double m_txi = ximin; m_txi <= ximax; m_txi *= xistp )
	        		{
	        			m_xi = m_txi ;
	        			std::cout << "xi = " << m_xi << std::endl ;
	        			// Run computation

	        			dummyConfMom =    ( m_pIntegrator->integrate(m_pProductFunctor,0., 1.,err_Para) ) ;
	        			// Print results via logger
	        			// PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__, gpdResult.toString());

	        			// Print results to outputfile (xi - mu2 - C_m(mu) - C-m(mu_0) - Error - GammaERBL)
	        			outputFile->write( evolutionOutputFile_gegenbauerModel, ElemUtils::Formatter() << std::to_string(m_xi) << " " << std::to_string(m_mu2)
	        					<< " " << dummyConfMom
								<< " " << refConfMom.at(iConfMom)
								<< " " << 1. - dummyConfMom / (GammaERBL * refConfMom.at(iConfMom) )
								<< " " << GammaERBL
								<< "\n");
	        			iConfMom += 1;
	        		}
	        		auto stop = std::chrono::high_resolution_clock::now();
	        		auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

	        		std::cout << "Time taken by function: "
	        				<< duration.count() << " seconds" << std::endl;

	        }


	        outputFile->close( evolutionOutputFile_gegenbauerModel );

// 0.854131 1224.744871 0.0336283685081517 0.105854618791548 -0.00896462595239123 0.314861851654019
}
