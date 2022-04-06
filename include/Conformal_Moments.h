#ifndef Conformal_Moments
#define Conformal_Moments

#include <ElementaryUtils/file_utils/FileUtils.h>
#include <fstream>
#include <sstream>

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

#include "/local/home/cmezrag/Téléchargements/boost_1_76_0/boost/math/special_functions/gegenbauer.hpp"

class ConformalMoments{

public:

	ConformalMoments() ;

	virtual ~ConformalMoments();

	void print2DConformalMoments(double mu2);
	double getAnomalousDimension();

	void setMomentOrder(double nOrder);
	void setXi(double xi);
	void setMu2(double mu2);


private:

double m_nGegenbauer;
double m_xi ;
double m_t;
double m_mu2 ;
double m_mu2ref ;
double m_ConfMom ;

std::vector<double> m_ModelPara ;

double ProductGegenbauerGPD(double x, std::vector<double> Para);

void ComputeConformalMoments();

//PARTONS Setter
void setPARTONS();

//Evolution Setter
void setEvolution();


//PARTONS GPD Declaration
PARTONS::GPDService* m_pGPDService ;
PARTONS::GPDModule* m_pGPDModel ;
PARTONS::GPDEvolutionModule* m_pGPDEvolutionModel ;
PARTONS::RunningAlphaStrongModule* m_pRunningAlphaStrongModule ;
PARTONS::ActiveFlavorsThresholdsModule* m_pActiveFlavorsThresholdsModule ;

//pointer toward GPD result
PARTONS::GPDResult* m_pGPDResult ;

// Declaration of functors for one dimension functions
NumA::FunctionType1D* m_pProductFunctor;

// Declaration of integrator
NumA::Integrator1D* m_pIntegrator;
NumA::IntegratorType1D::Type m_integratorType;



};

#endif /* INCLUDE_Conformal_Moments */
