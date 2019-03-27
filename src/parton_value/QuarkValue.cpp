#include "../../include/parton_value/QuarkValue.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <partons/utils/compare/CompareUtils.h>
#include <partons/utils/compare/ComparisonData.h>

#include <sstream>

namespace PARTONS {

const std::string QuarkValue::QUARK_VALUE_DB_COLUMN_NAME_QUARK_VALUE =
		"quark_distribution";

QuarkValue::QuarkValue(const QuarkValue &other) :
		BaseObject(other) {
	m_quarkFlavor = other.m_quarkFlavor;
	m_quarkValue = other.m_quarkValue;

}

QuarkValue::QuarkValue(QuarkFlavor::Type quarkFlavor, double quarkValue) :
		BaseObject("QuarkValue"), m_quarkFlavor(quarkFlavor), m_quarkValue(
				quarkValue){
}

QuarkValue::~QuarkValue() {
}

std::string QuarkValue::toString() const {
 	return ElemUtils::Formatter() << QuarkFlavor(m_quarkFlavor).getShortName() << " = " << m_quarkValue << '\n';
}

double QuarkValue::getQuarkValue() const {
	return m_quarkValue;
}

void QuarkValue::setQuarkValue(double quarkValue) {
	m_quarkValue = quarkValue;
}

void QuarkValue::setQuarkFlavor(QuarkFlavor quarkFlavorType) {
	m_quarkFlavor = quarkFlavorType;
}


QuarkFlavor QuarkValue::getQuarkFlavor() const{
	return m_quarkFlavor;
}


void QuarkValue::compare(ComparisonReport &rootComparisonReport,
		const QuarkValue &referenceObject, std::string parentObjectInfo) const {
	ComparisonData quark_distribution_value_comparisonData =
			CompareUtils::compareDouble(
					QuarkValue::QUARK_VALUE_DB_COLUMN_NAME_QUARK_VALUE,
					m_quarkValue, referenceObject.getQuarkValue(),
					rootComparisonReport.getTolerances(),
					ElemUtils::Formatter() << parentObjectInfo << " "
							<< getClassName());

	rootComparisonReport.addComparisonData(
			quark_distribution_value_comparisonData);
}

} /* namespace PARTONS */
