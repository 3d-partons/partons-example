#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

#include <H5Cpp.h>

/*
 * Get data set names.
 */
std::vector<std::string> getDataSetNames(const H5::H5File& file) {

    // Result
    std::vector<std::string> result;

    // Main group
    H5::Group group = file.openGroup("/");

    // Loop over objects
    for (hsize_t i = 0; i < group.getNumObjs(); i++) {
        if (group.getObjTypeByIdx(i) == H5G_DATASET) {
            result.push_back(group.getObjnameByIdx(i));
        }
    }

    return result;
}


/*
 * Compare with precision.
 */
bool nearlyEqual(double a, double b, double relTol = 1E-9, double absTol = 1E-12){

    double diff = std::fabs(a - b);

    return diff <= std::max(absTol, relTol * std::max(std::fabs(a), std::fabs(b)));
}

/*
 * Compare two data sets.
 */
void compareTwoDataSets(const std::vector<double>& dataA, const std::vector<double>& dataB) {

    // NaN indicates a quick mode
    for (size_t i = 0; i < dataA.size(); i++) {
        if (std::isnan(dataA[i])) return;
    }

    for (size_t i = 0; i < dataB.size(); i++) {
        if (std::isnan(dataB[i])) return;
    }

    //Check sizes
    if (dataA.size() != dataB.size()) {
        std::cerr << "[EE] different sizes of data sets" << std::endl;
        exit(1);
    }

    //Compare
    for (size_t i = 0; i < dataA.size(); i++) {
        if (! nearlyEqual(dataA[i], dataB[i])) {

            std::cerr << "[EE] different results" << std::endl;
            exit(1);
        }
    }
}

/*
 * Get data
 */
std::vector<double> getData(const H5::H5File& file, const std::string& name) {

    H5::Group group = file.openGroup("/");
    H5::DataSet dset = group.openDataSet(name);

    H5::DataSpace space = dset.getSpace();
    int rank = space.getSimpleExtentNdims();
    std::vector<hsize_t> dims(rank);
    space.getSimpleExtentDims(dims.data());

    if (rank != 1) {
        std::cerr << "[EE] dataset is not 1D (rank = " << rank << ")" << std::endl;
        exit(1);
    }

    std::vector<double> data(dims[0]);

    dset.read(data.data(), H5::PredType::NATIVE_DOUBLE);

    return data;
}

/*
 * Compare hdf5 files.
 */
int main(int argc, char** argv) {

    if (argc != 3) {
        std::cerr << "[EE] usage: " << argv[0] << " output_file.hdf5 reference_file.hdf5" << std::endl;
        return 1;
    }

    try {

        // File name
        H5::H5File fileA(argv[1], H5F_ACC_RDONLY);
        H5::H5File fileB(argv[2], H5F_ACC_RDONLY);

        // Get names
        std::vector<std::string> namesA = getDataSetNames(fileA);
        std::vector<std::string> namesB = getDataSetNames(fileB);

        // Compare lists
        std::sort(namesA.begin(), namesA.end());
        std::sort(namesB.begin(), namesB.end());

        auto itA = namesA.begin();
        auto itB = namesB.begin();

        while (itA != namesA.end() || itB != namesB.end()) {

            if (itA != namesA.end() && (itB == namesB.end() || *itA < *itB)) {

                std::cout << "[WW] files made for different lists of modules (" << (*itA) << "only in file A)" << std::endl;

                ++itA;

            } else if (itB != namesB.end() && (itA == namesA.end() || *itB < *itA)) {

                std::cout << "[WW] files made for different lists of modules (" << (*itB) << "only in file B)" << std::endl;

                ++itB;

            } else {

                std::cout << "[II] working for: " << (*itA) << std::endl;

                std::vector<double> dataA = getData(fileA, *itA);
                std::vector<double> dataB = getData(fileB, *itA);

                compareTwoDataSets(dataA, dataB);

                ++itA;
                ++itB;
            }
        }
    }
    // In a case of standard exception.
    catch (const std::exception &e) {

        // Display what happened
        std::cerr << "[EE] " << e.what() << std::endl;

        return 1;
    }


    return 0;
}
