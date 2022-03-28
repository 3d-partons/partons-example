/*
 * GPDANNDDIntegration.h
 *
 *  Created on: Jan 22, 2021
 *      Author: partons
 */

#ifndef GPD_GPDANNDDINTEGRATION_H_
#define GPD_GPDANNDDINTEGRATION_H_

#include "GPDANNDD.h"

namespace PARTONS {

struct GPDANNDDIntegrateParameters0 {
	GPDANNDD* m_pClass;
	double m_x;
	double m_xi;
	double m_t;
};

double GPDANNDDIntegrateFunction0A(double beta, void* p) {

	GPDANNDDIntegrateParameters0* par =
			static_cast<GPDANNDDIntegrateParameters0*>(p);
	return par->m_pClass->getDD(beta, (par->m_x - beta) / par->m_xi, par->m_t)
			/ par->m_xi;
}
double GPDANNDDIntegrateFunction0B(double beta, void* p) {

	return GPDANNDDIntegrateFunction0A(beta, p)
			+ GPDANNDDIntegrateFunction0A(-beta, p);
}

}

#endif /* GPD_GPDANNDDINTEGRATION_H_ */
