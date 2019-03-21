#include "../../include/parton_value/GluonValue.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/utils/compare/ComparisonData.h>
#include <partons/utils/compare/CompareUtils.h>

namespace PARTONS {


const std::string GluonValue::GLUON_VALUE_DB_COLUMN_NAME =
        "gluon_value";

const std::string GluonValue::GLUON_VALUE_PARAMETER_NAME_GLUON_VALUE =
        "gluon_value";

GluonValue::GluonValue() :
        BaseObject("GluonValue"), m_gluonValue(0.), m_nullObject(
                true) {
}

GluonValue::GluonValue(const GluonValue &other) :
        BaseObject(other) {
    m_gluonValue = other.m_gluonValue;
    m_nullObject = other.m_nullObject;
}

GluonValue::GluonValue(double gluonValue) :
        BaseObject("GluonValue"), m_gluonValue(gluonValue), m_nullObject(
                false) {
}

GluonValue::~GluonValue() {

}

std::string GluonValue::toString() const {
    return ElemUtils::Formatter() << "G = " << m_gluonValue << '\n';
}

double GluonValue::getGluonValue() const {
    return m_gluonValue;
}

void GluonValue::setGluonValue(double gluonValue) {
    m_gluonValue = gluonValue;
    m_nullObject = false;
}

bool GluonValue::isNullObject() const {
    return m_nullObject;
}

void GluonValue::setNullObject(bool nullObject) {
    m_nullObject = nullObject;
}

void GluonValue::compare(ComparisonReport &rootComparisonReport,
        const GluonValue &referenceObject,
        std::string parentObjectInfo) const {
    ComparisonData gluon_value_comparisonData =
            CompareUtils::compareDouble(
                    GluonValue::GLUON_VALUE_PARAMETER_NAME_GLUON_VALUE,
                    m_gluonValue, referenceObject.getGluonValue(),
                    rootComparisonReport.getTolerances(),
                    ElemUtils::Formatter() << parentObjectInfo << " "
                            << getClassName());

    rootComparisonReport.addComparisonData(gluon_value_comparisonData);
}

} /* namespace PARTONS */
