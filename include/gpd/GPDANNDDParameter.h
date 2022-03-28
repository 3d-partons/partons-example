/*
 * GPDANNDDParameter.h
 *
 *  Created on: Jan 22, 2021
 *      Author: partons
 */

#ifndef GPD_GPDANNDDPARAMETER_H_
#define GPD_GPDANNDDPARAMETER_H_

namespace PARTONS {

class GPDANNDDParameter {

public:

	GPDANNDDParameter() :
			m_weightBeta(0.), m_weightAlpha(0.), m_biasNeuron(0.), m_weightOutput(
					0.) {
	}

	GPDANNDDParameter(double weightBeta, double weightAlpha, double biasNeuron,
			double weightOutput) :
			m_weightBeta(weightBeta), m_weightAlpha(weightAlpha), m_biasNeuron(
					biasNeuron), m_weightOutput(weightOutput) {
	}

	GPDANNDDParameter(const GPDANNDDParameter& other) :
			m_weightBeta(other.m_weightBeta), m_weightAlpha(
					other.m_weightAlpha), m_biasNeuron(other.m_biasNeuron), m_weightOutput(
					other.m_weightOutput) {
	}

	inline double getBiasNeuron() const {
		return m_biasNeuron;
	}

	inline void setBiasNeuron(double biasNeuron) {
		m_biasNeuron = biasNeuron;
	}

	inline double getWeightAlpha() const {
		return m_weightAlpha;
	}

	inline void setWeightAlpha(double weightAlpha) {
		m_weightAlpha = weightAlpha;
	}

	inline double getWeightBeta() const {
		return m_weightBeta;
	}

	inline void setWeightBeta(double weightBeta) {
		m_weightBeta = weightBeta;
	}

	inline double getWeightOutput() const {
		return m_weightOutput;
	}

	inline void setWeightOutput(double weightOutput) {
		m_weightOutput = weightOutput;
	}

private:

	double m_weightBeta;
	double m_weightAlpha;
	double m_biasNeuron;
	double m_weightOutput;
};

}

#endif /* GPD_GPDANNDDPARAMETER_H_ */
