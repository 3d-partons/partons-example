/*
 * This code implements the conversion between the azimuthal and polar angles in the
 * muon-antimuon CM frame wrt to BM2003 parametrization (this is, phil and thetal),
 * and the respective ones in BDP2001 (namely, phiBDP and thetaBDP).
 * phiBDP = \varphi, thetaBDP = \theta in BDP2001 paper; phil = \varphi_\ell, thetal = \theta_\ell in BM2003
 */
#include "leptonCMframe.h"
#include <cmath>
#include <complex>
#include <iostream>
#include <string>
#include <vector>

//Variables needed
//double Epsilon2;
//double W1;
//double W2;
//double Modv;
//double Zeta;
//double Q1z;
//double CGamma;
//double SGamma;
//double p2x;
//double p20;
//double p2z;
//double p2az;
//double modp2a;
//double s2;
//double c2;

std::pair<double, double> leptons::leptonCMconverter(double phil, double thetal) {

    double phiBDP, thetaBDP; //returns; angles in lepton-CM frame in BDP2001 paper

    //The relation between (phil, thetal) and (phiBDP, thetaBDP) is given by the tangents tg(phiBDP) = num1/den1 and tg(thetaBDP) = num2/den2:
    double num1 = sin(thetal) * sin(phil);
    double den1 = c2 * sin(thetal) * cos(phil) + s2 * cos(thetal);
    double num2, den2;
    num2 = pow(c2 * sin(thetal) * cos(phil) + s2 * cos(thetal), 2.)
            + pow(sin(thetal) * sin(phil), 2.);
    num2 = sqrt(num2);
    den2 = -s2 * sin(thetal) * cos(phil) + c2 * cos(thetal);

    //Determination of phiBDP and thetaBDP for given phil and thetal
    //phiBDP:
    if (num1 > 0. && den1 > 0.) {
        phiBDP = atan(num1 / den1);
    } else if (num1 > 0. && den1 < 0.) {
        phiBDP = M_PI + atan(num1 / den1);
    } else if (num1 < 0. && den1 < 0.) {
        phiBDP = M_PI + atan(num1 / den1);
    } else if (num1 < 0. && den1 > 0.) {
        phiBDP = 2. * M_PI + atan(num1 / den1);
    } else if (num1 == 0. && den1 == 1.) {
        phiBDP = 0.;
    } else if (num1 == 0. && den1 == -1.) {
        phiBDP = M_PI;
    } else if (num1 == 1. && den1 == 0.) {
        phiBDP = M_PI / 2.;
    } else if (num1 == -1. && den1 == 0.) {
        phiBDP = 3. * M_PI / 2.;
    }

    //thetaBDP (num2 >= 0 since sin(thetaBDP) >= 0 for being thetaBDP a polar angle):
    if (num2 > 0. && den2 > 0.) {
        thetaBDP = atan(num2 / den2);
    } else if (num2 > 0. && den2 < 0.) {
        thetaBDP = M_PI + atan(num2 / den2);
    } else if (num2 == 0. && den2 == 1.) {
        thetaBDP = 0.;
    } else if (num2 == 0. && den2 == -1.) {
        thetaBDP = M_PI;
    } else if (num2 == 1. && den2 == 0.) {
        thetaBDP = M_PI / 2.;
    }

    return std::make_pair(phiBDP, thetaBDP);

}

void leptons::computeConverterVariables(double xB, double t, double Qcal2, double Mll2, double Mnucleon) {

    Epsilon2 = 4. * pow(xB * Mnucleon, 2.) / Qcal2;
    W1 = sqrt(Qcal2 / Epsilon2); //eq 11 BM2003
    W2 = sqrt(Qcal2 / Epsilon2) + t / (2. * Mnucleon);//eq 17 BM2003
    Modv = sqrt(1. - Mll2 / pow(W2, 2.));//eq 17 BM2003
    Zeta = atanh(Modv);//definition of Lorentz-rapidity
    Q1z = sqrt(Qcal2 * (1. + Epsilon2) / Epsilon2);//eq 11 BM2003
    CGamma = -(sqrt(Epsilon2) * (Qcal2 - Mll2 + t)
            + 2. * sqrt(Qcal2) * W2)
            / (2. * sqrt(Qcal2) * W2 * Modv * sqrt(1. + Epsilon2));//eq 18 BM2003
    SGamma = sqrt(1. - CGamma * CGamma);

    p2x = Q1z * SGamma; // p2^{x'}: x'-component of outgoing-proton momentum from the POV of TRF-II frame (BM2003)
    p20 = Mnucleon + W1 - W2; // p2^{0}
    p2z = -Q1z * CGamma - W2 * Modv; // p2^{z'}
    p2az = -sinh(Zeta) * p20 + cosh(Zeta) * p2z; //p2^{* z'}: z'-component of outgoing proton momentum seen in the lepton-CM frame obtained by boosting with rapidity Zeta (along direction of z'-axis) the TRF-II frame from BM2003

    //Since p2^{* x'} = p2^{x'} and p2^{y'} = 0, then:
    modp2a = sqrt(p2x * p2x + p2az * p2az); //modulus of the 3-momentum component of p2^{*}

    s2 = p2x / modp2a; // sin(phi2); phi2 is the angle between lepton-CM frame obtained by boosting TRF-II (BM2003), along z'-axis with rapidity Zeta, and the lepton-CM frame described in BDP2001
    c2 = -p2az / modp2a; // cos(phi2)

    //DEBUG
    std::cout << s2 << " " << c2 << " sin(phi2) cos(phi2)" << std::endl;
    //END DEBUG
}

