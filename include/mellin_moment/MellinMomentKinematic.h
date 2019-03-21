#ifndef MELLIN_MOMENT_KINEMATIC_H
#define MELLIN_MOMENT_KINEMATIC_H

/**
 * @file MellinMomentKinematic.h
 * @author: Arkadiusz P. Trawiński (DPhN/CEA-Saclay & CPHT/École polytechnique)
 * @date 21 March 2019
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <partons/beans/kinematic/KinematicType.h>
#include <partons/beans/Kinematic.h>
#include <string>

namespace ElemUtils {
class Packet;
class Parameters;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class MellinMomentKinematic
 *
 * @brief Class representing single Mellin Moment kinematics.
 *
 * This class represents a single Mellin Moment kinematics (n, xi, t, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$) to be used in computations of Mellin Moments of GPDs.
  */
class MellinMomentKinematic: public Kinematic {

public:
    /**
     * Parameter name to set variable \f$n\f$ via configuration methods.
     */
    static const std::string MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_N;

    /**
     * Parameter name to set variable \f$\xi\f$ via configuration methods.
     */
    static const std::string MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_XI;

    /**
     * Parameter name to set variable \f$\mu_{F}^{2}\f$ via configuration methods.
     */
    static const std::string MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_MUF2;

    /**
     * Parameter name to set variable \f$\mu_{R}^{2}\f$ via configuration methods.
     */
    static const std::string MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_MUR2;

    /**
     * Default constructor.
     */
    MellinMomentKinematic();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    MellinMomentKinematic(const MellinMomentKinematic &other);

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_X, GPDKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_XI, ObservableKinematic::PARAMETER_NAME_T, GPDKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_MUR2.
     */
    MellinMomentKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Assignment constructor.
     *
     * @param n index of Mellin Moment, where \f$n\f$-th means \f$f_n = \int dx x^{n-1} f(x) \f$-th.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    MellinMomentKinematic(int n, double xi, double t, double MuF2, double MuR2);

    /**
     * Assignment constructor.
     *
     * @param n index of Mellin Moment, where \f$n\f$-th means \f$f_n = \int dx x^{n-1} f(x) \f$-th.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    MellinMomentKinematic(const ElemUtils::GenericType &n,
            const ElemUtils::GenericType &xi, const ElemUtils::GenericType &t,
            const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Destructor.
     */
    virtual ~MellinMomentKinematic();

    virtual std::string toString() const;

    /**
     * Serialize into given Packet.
     * @param packet Target Packet.
     */
    void serialize(ElemUtils::Packet &packet) const;

    /**
     * Retrieve data from given Packet.
     * @param packet Input Packet.
     */
    void unserialize(ElemUtils::Packet &packet);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get kinematics type.
     */
    KinematicType::Type getKinematicType() const;

    /**
     * Set kinematics type.
     */
    void setKinematicType(KinematicType::Type kinematicType);

    /**
     * Get longitudinal momentum fraction of active parton.
     */
    double getN() const;

    /**
     * Set longitudinal momentum fraction of active parton.
     */
    void setN(int x);

    /**
     * Get skewness variable.
     */
    double getXi() const;

    /**
     * Set skewness variable.
     */
    void setXi(double xi);

    /**
     * Get four-momentum transfer squared of hadron target.
     */
    double getT() const;

    /**
     * Set four-momentum transfer squared of hadron target.
     */
    void setT(double t);

    /**
     * Get factorization scale squared.
     */
    double getMuF2() const;

    /**
     * Set factorization scale squared.
     */
    void setMuF2(double muF2);

    /**
     * Get renormalization scale squared.
     */
    double getMuR2() const;

    /**
     * Set renormalization scale squared.
     */
    void setMuR2(double muR2);

protected:

    virtual void updateHashSum() const;

private:

    /**
     * Kinematics type.
     */
    KinematicType::Type m_kinematicType;

    /**
     * Longitudinal momentum fraction of active parton.
     */
    double m_n;

    /**
     * Skewness variable.
     */
    double m_xi;

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    double m_t;

    /**
     * Factorization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuF2;

    /**
     * Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuR2;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
		MellinMomentKinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
		MellinMomentKinematic& kinematic);

} /* namespace PARTONS */

#endif /* MELLIN_MOMENT_KINEMATIC_H */
