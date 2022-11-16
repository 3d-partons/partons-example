#ifndef LEPTONCMFRAME_H_
#define LEPTONCMFRAME_H_

//Function declarations

#include <utility>

class leptons {
public:
    //Functions
    std::pair<double, double> leptonCMconverter(double phil, double thetal);
    double jacobianLeptonCM(double phil, double thetal);
    void computeConverterVariables(double xB, double t, double Qcal2,
            double Mll2, double Mnucleon);

private:
    //Variables needed
    double Epsilon2;
    double W1;
    double W2;
    double Modv;
    double Zeta;
    double Q1z;
    double CGamma;
    double SGamma;
    double p2x;
    double p20;
    double p2z;
    double p2az;
    double modp2a;
    double s2;
    double c2;

};
//std::pair<double, double> leptonCMconverter(double phil, double thetal);
//void computeConverterVariables(double xB, double t, double Qcal2, double Mll2, double Mnucleon);

#endif /* INCLUDE_LEPTONCMFRAME_H_ */
