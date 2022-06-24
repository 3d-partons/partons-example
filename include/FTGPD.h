/*
 * fftGPD.h
 *
 *  Created on: Jun 23, 2022
 *      Author: partons
 */

#ifndef INCLUDE_FTGPD_H_
#define INCLUDE_FTGPD_H_

#include <partons/BaseObject.h>
#include <stddef.h>
#include <complex>
#include <utility>
#include <vector>

namespace PARTONS {
class GPDModule;
} /* namespace PARTONS */

using namespace PARTONS;

class FTGPD: public BaseObject {

public:

    /**
     * Default constructor.
     */
    FTGPD();

    /**
     * Destructor.
     */
    virtual ~FTGPD();

    /**
     * Calculate FT for given nu with numerical integration.
     */
    std::complex<double> getFT(GPDModule* gpdModule, double nu, double xi,
            double t, double mu2);

    /**
     * Used by GSL integration routine.
     */
    static double gslGetFTFunction(double x, void* params);
};

struct GSLGetFTParameters {

    GPDModule* m_gpdModule;
    double m_xi;
    double m_t;
    double m_mu2;

    double m_nu;
    bool m_isCosine;
};

#endif /* INCLUDE_FTGPD_H_ */
