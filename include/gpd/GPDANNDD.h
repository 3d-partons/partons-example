#ifndef GPDANNDD_H
#define GPDANNDD_H

#include <gsl/gsl_integration.h>
#include <partons/modules/gpd/GPDModule.h>
#include <stddef.h>
#include <string>
#include <utility>
#include <vector>

#include "GPDANNDDParameter.h"

namespace PARTONS {

class GPDANNDD: public GPDModule {

public:

    static const unsigned int classId;    ///< ID assigned by BaseObjectRegistry

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDANNDD(const std::string &className);

    /** Copy constructor.
     @param other Object to be copied.
     */
    GPDANNDD(const GPDANNDD& other);

    /**
     * Default destructor.
     */
    virtual ~GPDANNDD();

    virtual GPDANNDD* clone() const;

    virtual PartonDistribution computeH();

    /**
     * Get DD.
     */
    double getDD(double beta, double alpha, double t) const;

    /**
     * Perform fit.
     */
    void fitToGPD(size_t populationSize, size_t nIterations,
            const std::string& dumpPath, const std::string& readPath);

    /**
     * Print parameters to STDOUT.
     */
    void printParameters() const;

    /************************
     * Setters and Getters
     ************************/

    /**
     * Get number of neurons (equivalent to the number of parameters).
     */
    size_t getNNeurons() const;

    /**
     * Get parameters.
     */
    const std::vector<GPDANNDDParameter>& getParameters() const;

    /**
     * Set parameters.
     */
    void setParameters(const std::vector<GPDANNDDParameter>& parameters);

    /**
     * Set parameters.
     */
    void setParameters(const std::vector<double>& parameters);

    /**
     * Get PDF x^-p power.
     */
    double getPdfHighPower() const;

    /**
     * Set PDF x^-p power.
     */
    void setPdfHighPower(double pdfHighPower);

    /**
     * Get PDF (1-x)^p power.
     */
    double getPdfLowPower() const;

    /**
     * Set PDF (1-x)^p power.
     */
    void setPdfLowPower(double pdfLowPower);

    /**
     * Get PDF (GPD at x, 0, t)
     */
    double getPDF(double x) const;

private:

    /**
     * ANN neuron.
     */
    double annNeuron(double beta, double alpha, double weightBeta,
            double weightAlpha, double bias) const;

    double annNeuronGhost(double beta, double alpha, double weightBeta,
            double weightAlpha, double bias) const;

    /**
     * ANN neuron normalization.
     */
    double annNeuronNormalisation(double beta, double weightBeta,
            double weightAlpha, double biasNeuron) const;

    double annNeuronNormalisationGhost(double beta, double weightBeta,
            double weightAlpha, double biasNeuron) const;

    std::vector<GPDANNDDParameter> m_parametersANN; ///< Parameters.
    std::vector<double> m_parametersDTerm; ///< Parameters.

    double m_pdfLowPower; ///< PDF power behavior: x^-m_pdfLowPower.
    double m_pdfHighPower; ///< PDF power behavior: (1-x)^m_pdfHighPower.
    double m_shadowNorm; ///< Normalisation of shadow term

    gsl_integration_workspace* m_pWorkspace0; ///< Integration workspace 0.
    gsl_integration_workspace* m_pWorkspace1; ///< Integration workspace 1.
    gsl_integration_workspace* m_pWorkspace2; ///< Integration workspace 2.

    void loadParameters(); ///< Load parameters.

    std::pair<double, double> computeHs(); ///< Compute GPD and associated error.
    double getDTerm() const; ///< Compute D-term.
};

} /* namespace PARTONS */

#endif /* GPDANNDD_H */
