/*
 * statistics.cpp
 *
 *  Created on: Nov 5, 2020
 *      Author: Pawel Snajder (NCBJ)
 */

#include "../include/statistics.h"

#include <stddef.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>

namespace Statistics {

double getMean(const std::vector<double>& v) {

	//check size
	if (v.size() == 0) {

		std::cout << __func__ << ": error: vector size is zero" << std::endl;
		exit(0);
	}

	//evaluate
	double mean = 0.;
	size_t n = 0;

	std::vector<double>::const_iterator it;

	for (it = v.begin(); it != v.end(); it++) {

		mean += (*it);
		n++;
	}

	//return
	return mean / double(n);
}

double getSigma(const std::vector<double>& v) {

	//check size
	if (v.size() == 0) {

		std::cout << __func__ << ": error: vector size is zero" << std::endl;
		exit(0);
	}

	//get mean
	double mean = getMean(v);

	//evaluate
	double sigma = 0.;
	size_t n = 0;

	std::vector<double>::const_iterator it;

	for (it = v.begin(); it != v.end(); it++) {

		sigma += pow(mean - (*it), 2);
		n++;
	}

	//return
	return sqrt(sigma / double(n));
}

size_t removeOutliers3Sigma(std::vector<double>& v) {

	//check size
	if (v.size() == 0) {

		std::cout << __func__ << ": error: vector size is zero" << std::endl;
		exit(0);
	}

	//get mean and sigma
	double meanData = getMean(v);
	double sigmaData = getSigma(v);

	if (sigmaData == 0.) {

		std::cout << __func__ << ": warning: sigma is zero" << std::endl;
		return 0;
	}

	//evaluate
	size_t nRemoved = 0;

	std::vector<double>::iterator it;

	for (it = v.begin(); it != v.end(); it++) {

		if (fabs((*it) - meanData) / sigmaData > 3.) {

			v.erase(it);
			nRemoved++;
		}
	}

	if (nRemoved != 0)
		nRemoved += removeOutliers3Sigma(v);

	//return
	return nRemoved;
}

}
