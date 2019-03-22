#include "../../include/mellin_moment/MellinMomentKinematic.h"

#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/beans/observable/ObservableKinematic.h>
#include <partons/services/hash_sum/CryptographicHashService.h>
#include <partons/Partons.h>
#include <partons/ServiceObjectRegistry.h>


namespace PARTONS {

const std::string MellinMomentKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_XI = "xi";
const std::string MellinMomentKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_MUF2 = "MuF2";
const std::string MellinMomentKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_MUR2 = "MuR2";

MellinMomentKinematic::MellinMomentKinematic() :
        Kinematic("MellinMomentKinematic"), m_kinematicType(KinematicType::THEO), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(0.) {
}

MellinMomentKinematic::MellinMomentKinematic(const MellinMomentKinematic& other) :
        Kinematic(other) {
    m_kinematicType = other.m_kinematicType;
    m_xi = other.m_xi;
    m_t = other.m_t;
    m_MuF2 = other.m_MuF2;
    m_MuR2 = other.m_MuR2;
}

MellinMomentKinematic::MellinMomentKinematic(double xi, double t, double MuF2,
        double MuR2) :
        Kinematic("MellinMomentKinematic"), m_kinematicType(KinematicType::THEO), m_xi(
                xi), m_t(t), m_MuF2(MuF2), m_MuR2(MuR2) {
}

MellinMomentKinematic::MellinMomentKinematic(const ElemUtils::GenericType& xi, const ElemUtils::GenericType& t,
        const ElemUtils::GenericType& MuF2, const ElemUtils::GenericType& MuR2) :
        Kinematic("MellinMomentKinematic"), m_kinematicType(KinematicType::THEO), m_xi(xi.toDouble()), m_t(t.toDouble()), m_MuF2(
                MuF2.toDouble()), m_MuR2(MuR2.toDouble()) {
}

MellinMomentKinematic::MellinMomentKinematic(const ElemUtils::Parameters &parameters) :
        Kinematic("MellinMomentKinematic"), m_kinematicType(KinematicType::THEO), m_xi(0.), m_t(0.), m_MuF2(0.), m_MuR2(0.) {

    if (parameters.isAvailable(MellinMomentKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_XI)) {
        m_xi = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(MellinMomentKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_XI);
    }
    if (parameters.isAvailable(ObservableKinematic::PARAMETER_NAME_T)) {
        m_t = parameters.getLastAvailable().toDouble();
    } else {
        errorMissingParameter(ObservableKinematic::PARAMETER_NAME_T);
    }

    //TODO remove from kinematic
    m_MuF2 =
            parameters.get(MellinMomentKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_MUF2).toDouble();
    m_MuR2 =
            parameters.get(MellinMomentKinematic::MELLIN_MOMENT_KINEMATIC_PARAMETER_NAME_MUR2).toDouble();
}

MellinMomentKinematic::~MellinMomentKinematic() {
}

std::string MellinMomentKinematic::toString() const {
    return ElemUtils::Formatter() << Kinematic::toString() << "\n" << "m_n = "
            << " m_xi = " << m_xi << " m_t = " << m_t << " m_MuF2 = "
            << m_MuF2 << "(Gev2) m_MuR2 = " << m_MuR2 << "(Gev2)";
}


void MellinMomentKinematic::updateHashSum() const {
    setHashSum(
            Partons::getInstance()->getServiceObjectRegistry()->getCryptographicHashService()->generateSHA1HashSum(
                    ElemUtils::Formatter() << m_xi << m_t << m_MuF2
                            << m_MuR2));
}

// ##### GETTERS & SETTERS #####

double MellinMomentKinematic::getMuF2() const {
    return m_MuF2;
}

void MellinMomentKinematic::setMuF2(double muF2) {
    m_MuF2 = muF2;
    updateHashSum();
}

double MellinMomentKinematic::getMuR2() const {
    return m_MuR2;
}

void MellinMomentKinematic::setMuR2(double muR2) {
    m_MuR2 = muR2;
    updateHashSum();
}

double MellinMomentKinematic::getXi() const {
    return m_xi;
}

void MellinMomentKinematic::setXi(double xi) {
    m_xi = xi;
    updateHashSum();
}

double MellinMomentKinematic::getT() const {
    return m_t;
}

void MellinMomentKinematic::setT(double t) {
    m_t = t;
    updateHashSum();
}

KinematicType::Type MellinMomentKinematic::getKinematicType() const {
    return m_kinematicType;
}

void MellinMomentKinematic::setKinematicType(KinematicType::Type kinematicType) {
    m_kinematicType = kinematicType;
}

void MellinMomentKinematic::serialize(ElemUtils::Packet &packet) const {
    Kinematic::serialize(packet);
    packet << m_xi << m_t << m_MuF2 << m_MuR2;
}

void MellinMomentKinematic::unserialize(ElemUtils::Packet &packet) {
    Kinematic::unserialize(packet);

    packet >> m_xi;
    packet >> m_t;
    packet >> m_MuF2;
    packet >> m_MuR2;
}

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        MellinMomentKinematic& kinematic) {
    kinematic.serialize(packet);
    return packet;
}
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        MellinMomentKinematic& kinematic) {
    kinematic.unserialize(packet);
    return packet;
}

} /* namespace PARTONS */
