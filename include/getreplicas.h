/*
 * fftGPD.h
 *
 *  Created on: Jun 23, 2022
 *      Author: partons
 */

#ifndef INCLUDE_GETREPLICAS_H_
#define INCLUDE_GETREPLICAS_H_

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

class GetReplicas: public BaseObject {

public:

    /**
     * Default constructor.
     */
    GetReplicas();

    /**
     * Destructor.
     */
    virtual ~GetReplicas();

    /**
     * Calculate x with numerical integration.
     */
    std::vector<std::vector<double>> ReplicasGetter(GPDModule* gpdModule, double xi, double t, double mu2);

    /**
     * Used by GSL integration routine.
     */
    static double gslGetFTFunction(double x, void* params);
};


#endif /* INCLUDE_GETREPLICAS_H_ */
