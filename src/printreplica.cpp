/*
 * printreplica.cpp
 *
 *  Created on: Jun 23, 2022
 *      Author: partons
 */

#include "../include/printreplica.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_math.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/parton_distribution/QuarkDistribution.h>
#include <partons/beans/QuarkFlavor.h>
#include <partons/modules/gpd/GPDModule.h>
#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/parton_distribution/PartonDistribution.h>
#include <partons/beans/parton_distribution/QuarkDistribution.h>
#include <partons/beans/QuarkFlavor.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <partons/services/GPDService.h>
#include <partons/ServiceObjectRegistry.h>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>

#include "../include/gpd/xEqXi_with_pos_replicas.h"

using namespace PARTONS;

PrintReplica::PrintReplica() :
        BaseObject("PrintReplica") {
}

PrintReplica::~PrintReplica() {
}

void PrintReplica::GetReplicaPrinted(GPDModule* gpdModule, double xi, double t, double mu2, std::ofstream& Myoutputfile){

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    //Initialising PARTONS objects
    PARTONS::GPDResult gpdResult;

	//Storage of GPDs replica for printing and plotting
    std::vector<double> xkinematic(101) ;
    std::vector< std::vector<double> > ReplicaData(c_nRep_x_eq_xi_with_pos);

    //Initiallising the values of x used to plot the replicas
    for(size_t i = 0; i < xkinematic.size(); i++){
    	xkinematic.at(i) = -1.+ i/50.;
    	std::cout << "x = " << xkinematic.at(i) << std::endl ;
    }



    //loop over replicas
    for (size_t j = 0; j < c_nRep_x_eq_xi_with_pos; j++) {

    	std::cout << "Treating replica n° " << j << std::endl ;

        //loop over parameters
        std::vector<double> parameters(c_nPar_x_eq_xi_with_pos);

        for (size_t k = 0; k < c_nPar_x_eq_xi_with_pos; k++) {
            parameters.at(k) = c_par_x_eq_xi_with_pos[j][k];
        }

        //load
        static_cast<GPDANNDD*>(gpdModule)->setParameters(parameters);

        for(size_t i = 0; i< xkinematic.size();i++){
        	gpdResult = pGPDService->computeSingleKinematic(PARTONS::GPDKinematic{xkinematic.at(i),xi,t,mu2,mu2}, gpdModule);
        	ReplicaData[j].push_back( gpdResult.getPartonDistribution(PARTONS::GPDType::H).getQuarkDistribution(PARTONS::QuarkFlavor::STRANGE).getQuarkDistributionPlus() );
        	std::cout << "For x = " << xkinematic.at(i) << " H(x) = " << ReplicaData[j][i] << std::endl;
        }
        for (const auto &e : ReplicaData[j]) Myoutputfile << e << " ";
        Myoutputfile << std::endl;

    }

}
