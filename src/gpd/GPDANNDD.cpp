#include "../../include/gpd/GPDANNDD.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_gegenbauer.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/parton_distribution/GluonDistribution.h>
#include <partons/beans/parton_distribution/QuarkDistribution.h>
#include <partons/beans/QuarkFlavor.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/modules/gpd/GPDGK16Numerical.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/Partons.h>
#include <cmath>
#include <iostream>
#include <iterator>

#include "../../include/gpd/GPDANNDDIntegration.h"

namespace PARTONS {

const unsigned int GPDANNDD::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GPDANNDD("GPDANNDD"));

GPDANNDD::GPDANNDD(const std::string &className) :
        GPDModule(className), m_pWorkspace0(0), m_pWorkspace1(0), m_pWorkspace2(
                0) {

    //type
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));

    //initial scale
    m_MuF2_ref = 2.;

    //load parameters
    loadParameters();

    m_shadowNorm = 1.;
}

GPDANNDD::GPDANNDD(const GPDANNDD& other) :
        GPDModule(other), m_pWorkspace0(0), m_pWorkspace1(0), m_pWorkspace2(0), m_parametersANN(other.m_parametersANN), m_parametersDTerm(
                other.m_parametersDTerm), m_pdfLowPower(other.m_pdfLowPower), m_pdfHighPower(
                other.m_pdfHighPower), m_shadowNorm(other.m_shadowNorm){
}

GPDANNDD::~GPDANNDD() {

    if (m_pWorkspace0) {
        gsl_integration_workspace_free(m_pWorkspace0);
    }

    if (m_pWorkspace1) {
        gsl_integration_workspace_free(m_pWorkspace1);
    }

    if (m_pWorkspace2) {
        gsl_integration_workspace_free(m_pWorkspace2);
    }
}

GPDANNDD* GPDANNDD::clone() const {
    return new GPDANNDD(*this);
}

double GPDANNDD::getDTerm() const {

    if (fabs(m_x) >= m_xi)
        return 0.;

    double result = 0.;
    double z = m_x / m_xi;

    for (size_t i = 0; i < m_parametersDTerm.size(); i++) {
        result += m_parametersDTerm.at(i) * (1. - pow(z, 2))
                * gsl_sf_gegenpoly_n(1 + 2 * i, 3 / 2., z);
    }

    return result;
}

std::pair<double, double> GPDANNDD::computeHs() {

    //absolute value of x
    double absX = fabs(m_x);

    //range of integration
    double betaA;
    double betaB;

    if (absX < m_xi) {
        betaA = -((-m_x + m_xi) / (1. + m_xi));
        betaB = -((-m_x - m_xi) / (1. + m_xi));
    } else {
        if (m_x > 0.) {
            betaA = -((m_x - m_xi) / (-1. + m_xi));
            betaB = -((-m_x - m_xi) / (1. + m_xi));
        } else {
            betaA = -((-m_x + m_xi) / (1. + m_xi));
            betaB = -((m_x + m_xi) / (-1. + m_xi));
        }
    }

    //workspace
    if (m_pWorkspace0 == 0) {
        m_pWorkspace0 = gsl_integration_workspace_alloc(1000);
    }

    //parameters
    GPDANNDDIntegrateParameters0 par0;

    //functions
    gsl_function fun0A;
    gsl_function fun0B;

    //set
    fun0A.function = &GPDANNDDIntegrateFunction0A;
    fun0B.function = &GPDANNDDIntegrateFunction0B;

    fun0A.params = &par0;
    fun0B.params = &par0;

    //set parameters
    par0.m_pClass = this;
    par0.m_x = m_x;
    par0.m_xi = m_xi;
    par0.m_t = m_t;

    //errors
    const double errAbs = 0.;
    const double errRel = 1.E-1;

    //integrate
    double result[2], error[2];

    double resultTotal;
    double errorTotal;

    if (absX < m_xi) {

        if (fabs(betaA) < betaB) {

            gsl_integration_qag(&fun0B, 0., fabs(betaA), errAbs, errRel, 1000,
                    GSL_INTEG_GAUSS61, m_pWorkspace0, &result[0], &error[0]);

            gsl_integration_qag(&fun0A, fabs(betaA), betaB, errAbs, errRel,
                    1000, GSL_INTEG_GAUSS61, m_pWorkspace0, &result[1],
                    &error[1]);
        } else {

            gsl_integration_qag(&fun0B, 0., betaB, errAbs, errRel, 1000,
                    GSL_INTEG_GAUSS61, m_pWorkspace0, &result[0], &error[0]);

            gsl_integration_qag(&fun0A, betaA, -betaB, errAbs, errRel, 1000,
                    GSL_INTEG_GAUSS61, m_pWorkspace0, &result[1], &error[1]);
        }

        resultTotal = result[0] + result[1] + getDTerm();
        errorTotal = sqrt(pow(error[0], 2) + pow(error[1], 2));
    } else {

        gsl_integration_qag(&fun0A, betaA, betaB, errAbs, errRel, 1000,
                GSL_INTEG_GAUSS61, m_pWorkspace0, &result[0], &error[0]);

        resultTotal = result[0];
        errorTotal = error[0];
    }

    //return
    return std::make_pair(resultTotal, errorTotal);
}

PartonDistribution GPDANNDD::computeH() {

    //Hs
    double Hs = computeHs().first;

    //store
    PartonDistribution partonDistribution;

    //g
    GluonDistribution gluonDistribution(0.);

    // u quark
    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);

    quarkDistribution_u.setQuarkDistribution(0.);
    quarkDistribution_u.setQuarkDistributionPlus(0.);
    quarkDistribution_u.setQuarkDistributionMinus(0.);

    // d quark
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);

    quarkDistribution_d.setQuarkDistribution(0.);
    quarkDistribution_d.setQuarkDistributionPlus(0.);
    quarkDistribution_d.setQuarkDistributionMinus(0.);

    // s quark
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    quarkDistribution_s.setQuarkDistribution(Hs);
    quarkDistribution_s.setQuarkDistributionPlus(2 * Hs);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

double GPDANNDD::getDD(double beta, double alpha, double t) const {

    //variables
    double absBeta = fabs(beta);
    int signBeta = (beta < 0.) ? (-1) : (1);

    //cluster
    double clusterResultA = 0.;
    double clusterResultB = 0.;
    double clusterResultC = 0.;
    double clusterResultD = 0.;

    //iterator
    std::vector<GPDANNDDParameter>::const_iterator it;

    //evaluate
    for (it = m_parametersANN.begin(); it != m_parametersANN.end(); it++) {

        if (size_t(m_parametersANN.begin() - it) % 2 == 0) {

            clusterResultA += it->getWeightOutput()
                    * annNeuron(absBeta, alpha, it->getWeightBeta(),
                            it->getWeightAlpha(), it->getBiasNeuron());

            clusterResultB += it->getWeightOutput()
                    * annNeuronNormalisation(absBeta, it->getWeightBeta(),
                            it->getWeightAlpha(), it->getBiasNeuron());
        } else {

            clusterResultC += it->getWeightOutput()
                    * annNeuronGhost(absBeta, alpha, it->getWeightBeta(),
                            it->getWeightAlpha(), it->getBiasNeuron());

            clusterResultD += it->getWeightOutput()
                    * annNeuronNormalisationGhost(absBeta, it->getWeightBeta(),
                            it->getWeightAlpha(), it->getBiasNeuron());
        }
    }

    //return
    double resultAB = 0.;
    resultAB = signBeta * getPDF(absBeta) * clusterResultA / clusterResultB
            * (1. - pow(m_x, 2)) / (1. - pow(beta, 2));

    double resultCD = 0.;
    resultCD =
            signBeta * getPDF(absBeta) * (pow(m_x, 2) - pow(m_xi, 2))
                    * (clusterResultA / clusterResultB
                            - clusterResultC / clusterResultD);

    return resultAB + m_shadowNorm * resultCD;
}

void GPDANNDD::printParameters() const {

    std::cout << __func__ << ": info: parameters" << std::endl;
    std::cout << std::scientific;
    std::cout << "\tlow power: " << m_pdfLowPower << std::endl;
    std::cout << "\thigh power: " << m_pdfHighPower << std::endl;

    for (std::vector<GPDANNDDParameter>::const_iterator it =
            m_parametersANN.begin(); it != m_parametersANN.end(); it++) {
        std::cout
                << "\tneuron (weightBeta, weightAlpha, biasNeuron, weightOutput): "
                << it->getWeightBeta() << "\t" << it->getWeightAlpha() << "\t"
                << it->getBiasNeuron() << "\t" << it->getWeightOutput()
                << std::endl;
    }

    std::cout.unsetf(std::ios_base::floatfield);
}

double GPDANNDD::annNeuron(double beta, double alpha, double weightBeta,
        double weightAlpha, double biasNeuron) const {
    return -tanh(biasNeuron - weightAlpha + beta * weightBeta)
            - tanh(biasNeuron + weightAlpha + beta * weightBeta)
            + tanh(
                    biasNeuron - (alpha * weightAlpha) / (1 - beta)
                            + beta * weightBeta)
            + tanh(
                    biasNeuron + (alpha * weightAlpha) / (1 - beta)
                            + beta * weightBeta);
}

double GPDANNDD::annNeuronNormalisation(double beta, double weightBeta,
        double weightAlpha, double biasNeuron) const {

    if (fabs(biasNeuron - weightAlpha + beta * weightBeta) > 710.
            || fabs(biasNeuron + weightAlpha + beta * weightBeta) > 710.) {
        return 1.E6;
    }

    return (2 * (-1 + beta)
            * (log(cosh(biasNeuron - weightAlpha + beta * weightBeta))
                    - log(cosh(biasNeuron + weightAlpha + beta * weightBeta))
                    + weightAlpha
                            * tanh(biasNeuron - weightAlpha + beta * weightBeta)
                    + weightAlpha
                            * tanh(biasNeuron + weightAlpha + beta * weightBeta)))
            / weightAlpha;
}

double GPDANNDD::annNeuronGhost(double beta, double alpha, double weightBeta,
        double weightAlpha, double biasNeuron) const {
    return annNeuron(beta, alpha, weightBeta, weightAlpha, biasNeuron);
}

double GPDANNDD::annNeuronNormalisationGhost(double beta, double weightBeta,
        double weightAlpha, double biasNeuron) const {
    return annNeuronNormalisation(beta, weightBeta, weightAlpha, biasNeuron);
}

void GPDANNDD::loadParameters() {

    m_parametersANN.clear();

    std::vector<double> par;

    //DEFAULT PARAMETERS
    //setParameters(par);

    m_pdfLowPower = 1.1;
    m_pdfHighPower = 5.;
}

size_t GPDANNDD::getNNeurons() const {
    return m_parametersANN.size();
}

const std::vector<GPDANNDDParameter>& GPDANNDD::getParameters() const {
    return m_parametersANN;
}

void GPDANNDD::setParameters(const std::vector<GPDANNDDParameter>& parameters) {
    m_parametersANN = parameters;
}

void GPDANNDD::setParameters(const std::vector<double>& parameters) {

    //check size
    if (((parameters.size() - 1) % 9) != 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "Wrong number of parameters");
    }

    //check if set
    m_parametersANN = std::vector<GPDANNDDParameter>(
            (parameters.size() - 1) / 9 * 2);
    m_parametersDTerm = std::vector<double>((parameters.size() - 1) / 9);

    //iterator
    std::vector<double>::const_iterator it = parameters.begin();

    //set parameters
    for (size_t neuron = 0; neuron < (parameters.size() - 1) / 9; neuron++) {

        double weightBetaR = (*(it++));
        double weightAlphaR = (*(it++));
        double biasR = (*(it++));
        double weightOutputR = (*(it++));

        double weightBetaG = (*(it++));
        double weightAlphaG = (*(it++));
        double biasG = (*(it++));
        double weightOutputG = (*(it++));

        double dTerm = (*(it++));

        m_parametersANN.at(2 * neuron + 0).setWeightBeta(weightBetaR);
        m_parametersANN.at(2 * neuron + 0).setWeightAlpha(weightAlphaR);
        m_parametersANN.at(2 * neuron + 0).setBiasNeuron(biasR);
        m_parametersANN.at(2 * neuron + 0).setWeightOutput(weightOutputR);

        m_parametersANN.at(2 * neuron + 1).setWeightBeta(weightBetaG);
        m_parametersANN.at(2 * neuron + 1).setWeightAlpha(weightAlphaG);
        m_parametersANN.at(2 * neuron + 1).setBiasNeuron(biasG);
        m_parametersANN.at(2 * neuron + 1).setWeightOutput(weightOutputG);

        m_parametersDTerm.at(neuron) = dTerm;
    }

    m_shadowNorm = (*(it++));
}

double GPDANNDD::getPdfHighPower() const {
    return m_pdfHighPower;
}

void GPDANNDD::setPdfHighPower(double pdfHighPower) {
    m_pdfHighPower = pdfHighPower;
}

double GPDANNDD::getPdfLowPower() const {
    return m_pdfLowPower;
}

void GPDANNDD::setPdfLowPower(double pdfLowPower) {
    m_pdfLowPower = pdfLowPower;
}

double GPDANNDD::getPDF(double x) const {
    return pow(x, -m_pdfLowPower) * pow(1. - x, m_pdfHighPower)
            * (0.123 + -0.327 * pow(x, 0.5) + 0.692 * x - 0.486 * pow(x, 1.5));
}

} /* namespace PARTONS */
