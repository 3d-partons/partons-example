/*
 * computefourier.cpp
 *
 *  Created on: Sept. 13, 2022
 *      Author: partons
 */

#include "../include/computefourier.h"

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

ComputeFourier::ComputeFourier() :
        BaseObject("ComputeFourier") {
}

ComputeFourier::~ComputeFourier() {
}

std::vector<double> ComputeFourier::GetFourierComputed(std::vector<double> x_gpd, std::vector<double> params){

    //Storage of GPDs replica for printing and plotting
    std::vector<double> nukinematic(params[2] + 1);
    std::vector<double> fourier_transform(params[2] + 1);
    std::vector<double> xkinematic(x_gpd.size());

    //Initiallising the values of nu used to plot the replicas
    for(size_t i = 0; i < nukinematic.size(); i++){
    	nukinematic.at(i) = params[0] + i*(params[1] - params[0])/params[2];
    	//std::cout << "nu = " << nukinematic.at(i) << std::endl;
    }

    //Initiallising the values of x used
    for(size_t i = 0; i < x_gpd.size(); i++){
    	xkinematic.at(i) = -1.+ 2.*((double) i)/((double) (x_gpd.size()-1));
    	//std::cout << "x = " << xkinematic.at(i) << std::endl;
    }

    for(size_t i = 0; i < nukinematic.size(); i++){
        double ft_val = 0;
        int nan_count = 0;
        for(size_t j = 1; j < x_gpd.size()-1; j++){
            ft_val += std::sin(2*M_PI*nukinematic[i]*xkinematic[j]) * x_gpd[j] * 2 / (x_gpd.size() - 1);
            if(std::isnan(-ft_val)) {
                nan_count += 1;
            }
        }
        if(nan_count == 0) {
            fourier_transform.at(i) = ft_val;
        }
        //std::cout << "For nu = " << nukinematic.at(i) << " H(nu) = " << fourier_transform.at(i) << std::endl;
    }
    return fourier_transform;
}
