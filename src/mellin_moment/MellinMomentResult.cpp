#include "../../../../MellinMomentResult.h"
#include "../../include/mellin_moment/MellinMomentResult.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <initializer_list>
#include <utility>

namespace PARTONS {


const std::string MellinMomentResult::MELLIN_MOMENT_RESULT_DB_TABLE_NAME = "gpd_result";

MellinMomentResult::MellinMomentResult() :
        Result("MellinMomentResult") {
}

MellinMomentResult::MellinMomentResult(const MellinMomentResult &other) :
        Result(other) {
    m_kinematic = other.m_kinematic;
    m_partonValues = other.m_partonValues;
}

MellinMomentResult::~MellinMomentResult() {
}

void MellinMomentResult::addPartonValues(GPDType::Type gpdType,
        PartonValues partonValues) {
    m_partonValues.insert(
            std::pair<GPDType::Type, PartonValues>(gpdType,
                    partonValues));
}

const PartonValues& MellinMomentResult::getPartonValues(
        GPDType::Type gpdType) const {

    std::map<GPDType::Type, PartonValues>::const_iterator it =
            m_partonValues.find(gpdType);

    if (it == m_partonValues.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot to find PartonValues object from GPDType = "
                        << GPDType(gpdType).toString());
    }

    return (it->second);
}

std::vector<GPDType> MellinMomentResult::getListOfGPDType() {
    std::vector<GPDType> list;

    std::map<GPDType::Type, PartonValues>::const_iterator it;

    if (m_partonValues.size() != 0) {

        for (it = m_partonValues.begin();
                it != m_partonValues.end(); ++it) {
            list.push_back(it->first);
        }
    }

    return list;
}

//TODO improve memory usage ; don't copy object parton distribution ; use reference or pointer
List<PartonValues> MellinMomentResult::getListOfPartonValues() const {
    List<PartonValues> partonValuesList;

    for (std::map<GPDType::Type, PartonValues>::const_iterator it =
            m_partonValues.begin(); it != m_partonValues.end();
            ++it) {
        partonValuesList.add(it->second);
    }

    return partonValuesList;
}

std::string MellinMomentResult::toString() const {
    ElemUtils::Formatter formatter;
    std::map<GPDType::Type, PartonValues>::const_iterator it;

    formatter << BaseObject::toString() << '\n';

    formatter << m_kinematic.toString() << '\n';

    formatter << "[PartonValuesList]" << '\n';

    for (it = m_partonValues.begin(); it != m_partonValues.end();
            it++) {
        //os << "ComputedBy: " << m_computedByGPDModuleId << std::endl;
        formatter << "GPD_" << GPDType(it->first).toString() << '\n';
        formatter << (it->second).toString();
        formatter << '\n';
    }

    return formatter.str();
}

const std::map<GPDType::Type, PartonValues>& MellinMomentResult::getPartonValues() const {
    return m_partonValues;
}

void MellinMomentResult::setPartonValues(
        const std::map<GPDType::Type, PartonValues>& partonValues) {
    m_partonValues = partonValues;
}

const MellinMomentKinematic& MellinMomentResult::getKinematic() const {
    return m_kinematic;
}

void MellinMomentResult::setKinematic(const MellinMomentKinematic& kinematic) {
    m_kinematic = kinematic;
}

void MellinMomentResult::compare(ComparisonReport &rootComparisonReport,
        const MellinMomentResult &referenceObject, std::string parentObjectInfo) const {

    //TODO faire un test pour valider la cinématique associée

    if (size() != referenceObject.size()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter()
                        << "Cannot perform comparison between parton distribution map because they are not equal in size ; With MellinMomentResult index id = "
                        << referenceObject.getIndexId() << '\n' << toString()
                        << '\n' << referenceObject.toString());
    }

    for (std::map<GPDType::Type, PartonValues>::const_iterator it =
            m_partonValues.begin(); it != m_partonValues.end();
            it++) {
        (it->second).compare(rootComparisonReport,
                referenceObject.getPartonValues((it->first)),
                ElemUtils::Formatter() << parentObjectInfo << " "
                        << this->getObjectInfo() << " "
                        << GPDType(it->first).toString());
    }
}

std::string MellinMomentResult::getObjectInfo() const {
    return ElemUtils::Formatter() << "GPD( " << m_kinematic.toString() << ")";
}

bool MellinMomentResult::isAvailable(const GPDType::Type& gpdType) const {
    bool result = false;

    m_it = m_partonValues.find(gpdType);

    if (m_it != m_partonValues.end()) {
        result = true;
    }

    return result;
}

PartonValues& MellinMomentResult::getLastAvailable() const {
    return const_cast<PartonValues&>(m_it->second);
}

size_t MellinMomentResult::size() const {
    return m_partonValues.size();
}

bool MellinMomentResult::operator <(const MellinMomentResult& other) const {
    return (m_kinematic < other.m_kinematic);
}

} /* namespace PARTONS */
