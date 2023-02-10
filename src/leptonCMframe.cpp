/*
 * This code implements the conversion between the azimuthal and polar angles in the
 * muon-antimuon CM frame wrt to BM2003 parametrization (this is, phil and thetal),
 * and the respective ones in BDP2001 (namely, phiBDP and thetaBDP).
 * phiBDP = \varphi, thetaBDP = \theta in BDP2001 paper; phil = \varphi_\ell, thetal = \theta_\ell in BM2003
 */
#include <include/leptonCMframe.h>
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

std::pair<double, double> leptons::leptonCMconverterToBDP01(double phil,
        double thetal) {

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

std::pair<double, double> leptons::leptonCMconverterToBM03(double phiBDP, double thetaBDP) {
    //same as leptonCMconverterToBDP01() up to changes s2 <-> -s2, phil <-> phiBDP, thetal <-> thetaBDP

    double phil, thetal; //returns; angles in lepton-CM frame in BM2003 paper

    //The relation between (phil, thetal) and (phiBDP, thetaBDP) is given by the tangents tg(phil) = num1/den1 and tg(thetal) = num2/den2:
    double num1 = sin(thetaBDP) * sin(phiBDP);
    double den1 = c2 * sin(thetaBDP) * cos(phiBDP) - s2 * cos(thetaBDP);
    double num2, den2;
    num2 = pow(c2 * sin(thetaBDP) * cos(phiBDP) - s2 * cos(thetaBDP), 2.)
            + pow(sin(thetaBDP) * sin(phiBDP), 2.);
    num2 = sqrt(num2);
    den2 = s2 * sin(thetaBDP) * cos(phiBDP) + c2 * cos(thetaBDP);

    //Determination of phil and thetal for given phiBDP and thetaBDP
    //phil:
    if (num1 > 0. && den1 > 0.) {
        phil = atan(num1 / den1);
    } else if (num1 > 0. && den1 < 0.) {
        phil = M_PI + atan(num1 / den1);
    } else if (num1 < 0. && den1 < 0.) {
        phil = M_PI + atan(num1 / den1);
    } else if (num1 < 0. && den1 > 0.) {
        phil = 2. * M_PI + atan(num1 / den1);
    } else if (num1 == 0. && den1 == 1.) {
        phil = 0.;
    } else if (num1 == 0. && den1 == -1.) {
        phil = M_PI;
    } else if (num1 == 1. && den1 == 0.) {
        phil = M_PI / 2.;
    } else if (num1 == -1. && den1 == 0.) {
        phil = 3. * M_PI / 2.;
    }

    //thetal (num2 >= 0 since sin(thetal) >= 0 for being thetal a polar angle):
    if (num2 > 0. && den2 > 0.) {
        thetal = atan(num2 / den2);
    } else if (num2 > 0. && den2 < 0.) {
        thetal = M_PI + atan(num2 / den2);
    } else if (num2 == 0. && den2 == 1.) {
        thetal = 0.;
    } else if (num2 == 0. && den2 == -1.) {
        thetal = M_PI;
    } else if (num2 == 1. && den2 == 0.) {
        thetal = M_PI / 2.;
    }

    return std::make_pair(phil, thetal);
}



double leptons::jacobianLeptonCM(double phil, double thetal) {

    double jac; //the return. jac's definition: d(xsec)/(... dthetal dphil) = jac * d(xsec)/(... dthetaBDP dphiBDP)

    double thetaThetaL; //dcos(thetaBDP)/dcos(thetal)
    double thetaPhiL; //dcos(thetaBDP)/dcos(phil)
    double phiThetaL; //dcos(phiBDP)/dcos(thetal)
    double phiPhiL; //dcos(phiBDP)/dcos(phil)

    double cotgThetal = cos(thetal) / sin(thetal);
    double sinTheta = sqrt(
            pow(c2 * sin(thetal) * cos(phil) + s2 * cos(thetal), 2.)
                    + pow(sin(thetal) * sin(phil), 2.));
    double sinPhi = sin(thetal)*sin(phil)/sinTheta;

    thetaThetaL = s2 * cos(phil) * cotgThetal + c2;
    thetaPhiL = -s2 * sin(thetal);
    phiThetaL = (-c2 * cotgThetal * cos(phil) + s2) / sinTheta;
    phiThetaL += (pow(cos(thetal), 2.) * (c2 * tan(thetal) * cos(phil) + s2)
            * (c2 * c2 * pow(cos(phil), 2.) + pow(sin(phil), 2.)
                    + s2 * c2
                            * (cotgThetal * cos(phil) - tan(thetal) * cos(phil))
                    - s2 * s2)) / pow(sinTheta, 3.);
    phiPhiL = c2 * sin(thetal) / sinTheta;
    phiPhiL -= (pow(sin(thetal), 2.)
            * (c2 * sin(thetal) * cos(phil) + s2 * cos(thetal))
            * (c2 * c2 * cos(phil) + s2 * c2 * cotgThetal - cos(phil)))
            / pow(sinTheta, 3.);

    jac = thetaThetaL * phiPhiL - thetaPhiL * phiThetaL;

    jac = fabs(jac)*sin(thetal)*sin(phil)/(sinTheta*sinPhi);

    return jac;
}

void leptons::computeConverterVariables(double xB, double t, double Qcal2,
        double Mll2, double Mnucleon) {

    //Computation of tMin:
    Epsilon2 = 4. * pow(xB * Mnucleon, 2.) / Qcal2;

    double tMin = -1. / (4. * xB * (1. - xB) + Epsilon2);
    tMin *= (2. * ((1. - xB) * Qcal2 - xB * Mll2)
                + Epsilon2 * (Qcal2 - Mll2)
                - 2. * sqrt(1. + Epsilon2)
                        * sqrt(
                                pow((1. - xB) * Qcal2 - xB * Mll2, 2.)
                                        - Epsilon2 * Qcal2 * Mll2));

    W1 = sqrt(Qcal2 / Epsilon2); //eq 11 BM2003
    W2 = sqrt(Qcal2 / Epsilon2) + t / (2. * Mnucleon); //eq 17 BM2003
    Modv = sqrt(1. - Mll2 / pow(W2, 2.)); //eq 17 BM2003

    Zeta = atanh(Modv); //definition of Lorentz-rapidity
    Q1z = sqrt(Qcal2 * (1. + Epsilon2) / Epsilon2); //eq 11 BM2003
    CGamma = -(sqrt(Epsilon2) * (Qcal2 - Mll2 + t) + 2. * sqrt(Qcal2) * W2)
            / (2. * sqrt(Qcal2) * W2 * Modv * sqrt(1. + Epsilon2)); //eq 18 BM2003
    SGamma = sqrt(1. - CGamma * CGamma);

    if (t == tMin) {
        SGamma = 0.;//eq 21 in BM2003
    }

    p2x = Q1z * SGamma; // p2^{x'}: x'-component of outgoing-proton momentum from the POV of TRF-II frame (BM2003)
    p20 = Mnucleon + W1 - W2; // p2^{0}
    p2z = -Q1z * CGamma - W2 * Modv; // p2^{z'}
    p2az = -sinh(Zeta) * p20 + cosh(Zeta) * p2z; //p2^{* z'}: z'-component of outgoing proton momentum seen in the lepton-CM frame obtained by boosting with rapidity Zeta (along direction of z'-axis) the TRF-II frame from BM2003

    //Since p2^{* x'} = p2^{x'} and p2^{y'} = 0, then:
    modp2a = sqrt(p2x * p2x + p2az * p2az); //modulus of the 3-momentum component of p2^{*}

    s2 = p2x / modp2a; // sin(phi2); phi2 is the angle between lepton-CM frame obtained by boosting TRF-II (BM2003), along z'-axis with rapidity Zeta, and the lepton-CM frame described in BDP2001
    c2 = -p2az / modp2a; // cos(phi2)

}

