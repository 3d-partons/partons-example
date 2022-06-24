/*
 * fftGPD.cpp
 *
 *  Created on: Jun 23, 2022
 *      Author: partons
 */

#include "../include/FTGPD.h"

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
#include <cmath>
#include <string>

using namespace PARTONS;

FTGPD::FTGPD() :
        BaseObject("FTGPD") {
}

FTGPD::~FTGPD() {
}

std::complex<double> FTGPD::getFT(GPDModule* gpdModule, double nu, double xi,
        double t, double mu2) {

    //create parameters
    GSLGetFTParameters par;

    par.m_gpdModule = gpdModule;
    par.m_xi = xi;
    par.m_t = t;
    par.m_mu2 = mu2;

    par.m_nu = nu;

    //gsl workspace and function
    gsl_integration_workspace * w = gsl_integration_workspace_alloc(1000);

    gsl_function F;
    F.function = &FTGPD::gslGetFTFunction;
    F.params = &par;

    double resultRe, errorRe;
    double resultIm, errorIm;

    //get real part
    par.m_isCosine = true;

//    gsl_integration_qags(&F, -1., 1., 0, 1e-3, 1000, w, &resultRe, &errorRe);
    resultRe = 0.;
    errorRe = 0.;

    //get imaginary part
    par.m_isCosine = false;

    gsl_integration_qags(&F, -1., 1, 0, 1e-1, 1000, w, &resultIm, &errorIm);

    //return
    return std::complex<double>(resultRe, resultIm);

}

double FTGPD::gslGetFTFunction(double x, void* p) {

    //get parameters
    GSLGetFTParameters* par = static_cast<GSLGetFTParameters*>(p);

    //modulation
    double modulation =
            (par->m_isCosine) ?
                    (cos(2 * M_PI * par->m_nu * x)) :
                    (-1 * sin(2 * M_PI * par->m_nu * x));

    double gpd = par->m_gpdModule->compute(
            GPDKinematic(x, par->m_xi, par->m_t, par->m_mu2,
                    par->m_mu2), GPDType::H).getQuarkDistribution(
            QuarkFlavor::STRANGE).getQuarkDistribution();

    if(std::isnan(gpd)) {
        std::cout << "warning: gpd gives NaN value for x = " << x << " (changing to 0)" << std::endl;
        gpd = 0.;
    }

    //return
    return modulation
            * gpd;
}

