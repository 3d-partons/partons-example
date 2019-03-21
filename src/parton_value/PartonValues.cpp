#include "../../include/parton_value/PartonValues.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <utility>


namespace PARTONS {
class GluonValue;
} /* namespace PARTONS */

namespace PARTONS {

PartonValues::PartonValues() :
		BaseObject("PartonValues") {
}

PartonValues::PartonValues(const PartonValues &other) :
		BaseObject(other) {
	m_gluonValue = other.m_gluonValue;
	m_quarkValues = other.m_quarkValues;
}

PartonValues::~PartonValues() {
}

std::string PartonValues::toString() const {
	ElemUtils::Formatter formatter;

	formatter << BaseObject::toString() << '\n';

	formatter << m_gluonValue.toString();

	for (std::map<QuarkFlavor::Type, QuarkValue>::const_iterator it =
			m_quarkValues.begin(); it != m_quarkValues.end(); it++) {
		formatter << (it->second).toString();
	}

	return formatter.str();
}

void PartonValues::addQuarkValue(QuarkValue& quarkValue) {
	m_quarkValues.insert(
			std::make_pair(quarkValue.getQuarkFlavor().getType(), quarkValue));
}

void PartonValues::addQuarkValue(const QuarkValue &quarkValue) {
	m_quarkValues.insert(
			std::make_pair(quarkValue.getQuarkFlavor(), quarkValue));
}

unsigned int PartonValues::getQuarkValuesSize() const {
	return m_quarkValues.size();
}

List<QuarkValue> PartonValues::getListOfQuarkValue() const {
	List<QuarkValue> result;

	for (std::map<QuarkFlavor::Type, QuarkValue>::const_iterator it =
			m_quarkValues.begin(); it != m_quarkValues.end(); it++) {
		result.add(it->second);
	}

	return result;
}

std::vector<QuarkFlavor::Type> PartonValues::getListOfQuarkFlavor() {
	std::vector<QuarkFlavor::Type> list;
	std::map<QuarkFlavor::Type, QuarkValue>::const_iterator it;

	if (m_quarkValues.size() != 0) {

		for (it = m_quarkValues.begin(); it != m_quarkValues.end(); ++it) {
			list.push_back(it->first);
		}
	}

	return list;
}

void PartonValues::setQuarkValues(
		const std::map<QuarkFlavor::Type, QuarkValue>& quarkValues) {
	m_quarkValues = quarkValues;
}

const std::map<QuarkFlavor::Type, QuarkValue>& PartonValues::getQuarkValues() const {
	return m_quarkValues;
}

const QuarkValue& PartonValues::getQuarkValue(
		QuarkFlavor::Type quarkFlavorType) const {

	std::map<QuarkFlavor::Type, QuarkValue>::const_iterator it =
			m_quarkValues.find(quarkFlavorType);

	if (it == m_quarkValues.end()) {
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter()
						<< "Unable to find QuarkValue object from QuarFlavorType = "
						<< QuarkFlavor(quarkFlavorType).getShortName());
	}

	return (it->second);
}

double PartonValues::getQuarkValue() {
	double result = 0.;

	for (std::map<QuarkFlavor::Type, QuarkValue>::const_iterator it =
			m_quarkValues.begin(); it != m_quarkValues.end(); it++) {
		result += (it->second).getQuarkValue();
	}

	return result;
}

void PartonValues::setGluonValue(const GluonValue& gluonValue) {
	m_gluonValue = gluonValue;
}

const GluonValue& PartonValues::getGluonValue() const {
	return m_gluonValue;
}

void PartonValues::compare(ComparisonReport &rootComparisonReport,
		const PartonValues &referenceObject,
		std::string parentObjectInfo) const {

	// compare gluon value
	this->m_gluonValue.compare(rootComparisonReport,
			referenceObject.getGluonValue(),
			ElemUtils::Formatter() << parentObjectInfo);

	for (std::map<QuarkFlavor::Type, QuarkValue>::const_iterator it =
			m_quarkValues.begin(); it != m_quarkValues.end(); it++) {
		(it->second).compare(rootComparisonReport,
				referenceObject.getQuarkValue((it->first)),
				ElemUtils::Formatter() << parentObjectInfo << " "
						<< QuarkFlavor(it->first).toString());
	}
}

} /* namespace PARTONS */
