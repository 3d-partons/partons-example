/*
 * fftGPD.h
 *
 *  Created on: Jun 23, 2022
 *      Author: partons
 */

#ifndef INCLUDE_PRINTREPLICA_H_
#define INCLUDE_PRINTREPLICA_H_

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

class PrintReplica: public BaseObject {

public:

    /**
     * Default constructor.
     */
    PrintReplica();

    /**
     * Destructor.
     */
    virtual ~PrintReplica();

    /**
     * Calculate FT for given nu with numerical integration.
     */
    void GetReplicaPrinted(GPDModule* gpdModule, double xi, double t, double mu2, std::ofstream& Myoutputfile);

    /**
     * Used by GSL integration routine.
     */
    static double gslGetFTFunction(double x, void* params);
};


#endif /* INCLUDE_PRINTREPLICA_H_ */
