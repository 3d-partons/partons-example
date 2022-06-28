#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include <random>

/*
	This function evaluates chi2.
	The points are assumed to be uncorrelated.
*/
double getChi2(const std::vector<double>& replicaValues, const std::vector<std::pair<double, double> >& newValues){

	//check size
	if(replicaValues.size() != newValues.size()){

		std::cout << "error: " << __func__ << ": input vectors have different sizes" << std::endl;
		exit(0);
	}

	//init
	double chi2 = 0.;

	//loop
	for(size_t i = 0; i < replicaValues.size(); i++){
		chi2 += pow((replicaValues.at(i) - newValues.at(i).first)/newValues.at(i).second, 2);
	}

	//return
	return chi2;
}

/*
	This function returns a vector of weights for each replica.
	'replicasValues[iReplica][iNewPoint]' containes value that 'iReplica' gives at 'iNewPoint'
	'newValues[iNewPoint]' containes value (first) and uncertainty (second) observed in new point of index 'iNewPoint'
*/
std::vector<double> makeReweighting(const std::vector<std::vector<double> >& replicasValues, const std::vector<std::pair<double, double> >& newValues){

	//init
	std::vector<double> result(replicasValues.size());
	double sum = 0.;

	//loop
	for(size_t i = 0; i < replicasValues.size(); i++){

		double thisChi2 = getChi2(replicasValues.at(i), newValues);

		result.at(i) = pow(thisChi2, (newValues.size() - 1) / 2.) * exp(-1 * thisChi2 / 2.);
		sum += result.at(i);
	}

	//normalise
	for(size_t i = 0; i < replicasValues.size(); i++){
		result.at(i) /= sum;
	}

	//return 
	return result;
}

/*
	This function returnes effective number of replicas.
*/
double getEffectiveNumberOfReplicas(const std::vector<double>& weights){

	//init
	double sum = 0.;

	//loop
	for(size_t i = 0; i < weights.size(); i++){
		sum += weights.at(i) * log(weights.at(i));
	}

	//return
	return exp(-1 * sum);
}

/*
	Evaluate mean for given point.	
*/
double getMean(const std::vector<std::vector<double> >& replicasValues, size_t iNewPoint, const std::vector<double>& weights = std::vector<double>()){

	//check size
	if(weights.size() > 0 && replicasValues.size() != weights.size()){

		std::cout << "error: " << __func__ << ": input vectors have different sizes" << std::endl;
		exit(0);
	}

	//check point
	if(iNewPoint >= replicasValues.at(0).size()){

		std::cout << "error: " << __func__ << ": wrong point index" << std::endl;
		exit(0);
	}

	//init
	double result = 0.;
	double sumOfWeights = 0.;

	//loop
	for(size_t i = 0; i < replicasValues.size(); i++){

		double weight = 1.;
		if(weights.size() > 0) weight = weights.at(i);

		result += weight * replicasValues.at(i).at(iNewPoint);
		sumOfWeights += weight;
	}

	//return
	return result / sumOfWeights;
}

/*
	Evaluate standard deviation for given point.	
*/
double getSigma(const std::vector<std::vector<double> >& replicasValues, size_t iNewPoint, const std::vector<double>& weights = std::vector<double>()){

	//init
	double result = 0;
	double sumOfWeights2 = 0.;

	//get mean
	double mean = getMean(replicasValues, iNewPoint, weights);

	//loop
	for(size_t i = 0; i < replicasValues.size(); i++){

		double weight = (weights.size() == 0) ? (1 / double(replicasValues.size())) : (weights.at(i));

		result += weight * pow(mean - replicasValues.at(i).at(iNewPoint), 2);
		sumOfWeights2 += pow(weight, 2);
	}

	//return
	return sqrt(result / (1. - sumOfWeights2));
}

int main(){

	//test
	//all values are asssumed to be zero

	//random generator
	std::default_random_engine generator;

	//number of replicas
	size_t nReplicas = 100;

	// uncertainty of replicas
	std::vector<double> replicaUncertainties;

	replicaUncertainties.push_back(0.1);
	replicaUncertainties.push_back(0.2);
	replicaUncertainties.push_back(0.3);

	//number of new points
	size_t nNewPoints = replicaUncertainties.size();

	//uncertainties of new points (also giving the number of points)
	std::vector<double> newPointsUncertainties(nNewPoints);

	for(size_t i = 0; i < nNewPoints; i++){
		newPointsUncertainties.at(i) = replicaUncertainties.at(i) / 4.;
	}

	//generate replica values
	std::vector<std::vector<double> > replicasValues(nReplicas, std::vector<double>(nNewPoints));

	for(size_t i = 0; i < nReplicas; i++){
		for(size_t j = 0; j < nNewPoints; j++){

			std::normal_distribution<double> distribution(0., replicaUncertainties.at(j));
			replicasValues.at(i).at(j) = distribution(generator);
		}
	}

	//generate points
	std::vector<std::pair<double, double> > newValues(nNewPoints);

	for(size_t i = 0; i < nNewPoints; i++){

		std::normal_distribution<double> distribution(0., newPointsUncertainties.at(i));
		newValues.at(i) = std::make_pair(distribution(generator), newPointsUncertainties.at(i));
	}

	//get weights
	std::vector<double> weights = makeReweighting(replicasValues, newValues);

	//print
	for(size_t i = 0; i < weights.size(); i++){
		std::cout << "weight: " << i << ' ' << weights.at(i) << std::endl;
	}

	std::cout << "effective number of replicas: " << getEffectiveNumberOfReplicas(weights) << std::endl;

	for(size_t i = 0; i < nNewPoints; i++){

		std::cout << "point: " << i << 
			" before: " << getMean(replicasValues, i) << " +- " << getSigma(replicasValues, i) << 
			" after: " <<  getMean(replicasValues, i, weights) << " +- " << getSigma(replicasValues, i, weights) << 
			std::endl;
	}
	
	return 0;
}
