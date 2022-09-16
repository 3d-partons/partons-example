/*
 * computefourier.h
 *
 *  Created on: Jun 23, 2022
 *      Author: partons
 */

#ifndef INCLUDE_COMPUTEFOURIER_H_
#define INCLUDE_COMPUTEFOURIER_H_

#include <partons/BaseObject.h>
#include <stddef.h>
#include <complex>
#include <utility>
#include <vector>
//#include "../include/gpd/xEqXi_with_pos_replicas.h"
#include "../include/statistics.h"
#include "../include/gpd/GPDANNDD.h"

namespace PARTONS {
class GPDModule;
} /* namespace PARTONS */

using namespace PARTONS;

class ComputeFourier: public BaseObject {

public:

    /**
     * Default constructor.
     */
    ComputeFourier();

    /**
     * Destructor.
     */
    virtual ~ComputeFourier();

    /**
     * Calculate nu with numerical integration.
     */
    std::vector<double> GetFourierComputed(std::vector<double> x_gpd, std::vector<double> params);

    /**
     * Used by GSL integration routine.
     */
    static double gslGetFTFunction(double x, void* params);
};


#endif /* INCLUDE_COMPUTEFOURIER_H_ */
