/*
 * statistics.h
 *
 *  Created on: Nov 5, 2020
 *      Author: Pawel Sznajder (NCBJ)
 */

#ifndef INCLUDE_STATISTICS_H_
#define INCLUDE_STATISTICS_H_

#include <stddef.h>
#include <vector>

namespace Statistics {

/**
 * Get mean.
 */
double getMean(const std::vector<double>& v);

/**
 * Get sigma.
 */
double getSigma(const std::vector<double>& v);

/**
 * Remove outliers (3sigma method).
 */
size_t removeOutliers3Sigma(std::vector<double>& v);

}

#endif /* INCLUDE_STATISTICS_H_ */
