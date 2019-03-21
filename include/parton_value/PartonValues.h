#ifndef PARTON_VALUES_H
#define PARTON_VALUES_H

/**
 * @file PartonValues.h
 * @author: Arkadiusz P. Trawiński (DPhN/CEA-Saclay & CPHT/École polytechnique)
 * @date 21 March 2019
 * @version 1.0
 */

#include <partons/beans/List.h>
#include <partons/beans/QuarkFlavor.h>
#include <partons/utils/compare/ComparisonReport.h>
#include <map>
#include <string>
#include <vector>

#include "GluonValue.h"
#include "QuarkValue.h"

namespace PARTONS {

class ComparisonReport;

/**
 * @class PartonValues
 *
 * @brief Parton values for single physics object.
 *
 * This class is used to store parton values for a single physics object (e.g. Mellin Moment of a given type). It contains quark values (QuarkValue objects) and a gluon value (GluonValue object). Each quark value is defined for a different quark flavor.
 *
 */
class PartonValues: public BaseObject {
public:

	/**
	 * Default constructor.
	 */
	PartonValues();

	/**
	 * Copy constructor.
	 * @param other Object to be copied.
	 */
	PartonValues(const PartonValues &other);

	/**
	 * Destructor.
	 */
	virtual ~PartonValues();

	/**
	 * Add quark value associated to given quark flavor that is defined in the object to be added.
	 * @param quarkValue Reference to quark value to be added.
	 */
	void addQuarkValue(const QuarkValue &quarkValue);

	/**
	 * Add quark value associated to given quark flavor that is defined in the object to be added.
	 * @param quarkValue Reference to quark value to be added.
	 */
	void addQuarkValue(QuarkValue &quarkValue);

	/**
	 * Get reference to quark value associated to given quark flavor.
	 * @param quarkFlavorType Quark flavor associated to quark value to be selected.
	 * @return Reference to selected quark value.
	 */
	const QuarkValue& getQuarkValue(QuarkFlavor::Type quarkFlavorType) const;

	/**
	 * Get list of quark flavors associated to stored quark values.
	 * @return Vector of associated flavors.
	 */
	std::vector<QuarkFlavor::Type> getListOfQuarkFlavor();

	/**
	 * Get number of quark values stored in this object.
	 * @return Number of quark values stored in this object.
	 */
	unsigned int getQuarkValuesSize() const;

	/**
	 * Get list of stored quark values.
	 * @return Retrieved List of QuarkValue objects.
	 */
	List<QuarkValue> getListOfQuarkValue() const;

	/**
	 * Get sum of all stored quark values.
	 * Return<br>
	 * \f$\sum_q F^{q}\f$ <br>
	 * where F is physics object (e.g. GPD) for which values are defined.<br>
	 * <br>
	 * Note that sum elements are not weighted here by square of corresponding quark charges.
	 * @return Sum of quark values.
	 */
	double getQuarkValue();

	virtual std::string toString() const;

	/**
	 * Compare to other PartonValue object and store comparison result in given comparison report.
	 * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
	 * @param referenceObject Reference to object to be compared.
	 * @param parentObjectInfo Addition information coming from the parent object (if needed).
	 */
	void compare(ComparisonReport &rootComparisonReport,
			const PartonValues &referenceObject, std::string parentObjectInfo =
					"") const;

	//********************************************************
	//*** SETTERS AND GETTERS ********************************
	//********************************************************

	/**
	 * Get reference to map containing stored quark values distinguished by associated quark flavors.
	 */
	const std::map<QuarkFlavor::Type, QuarkValue>& getQuarkValues() const;

	/**
	 * Set map containing stored quark values distinguished by associated quark flavors.
	 */
	void setQuarkValues(
			const std::map<QuarkFlavor::Type, QuarkValue>& quarkValues);

	/**
	 * Get reference to gluon value.
	 */
	const GluonValue& getGluonValue() const;

	/**
	 * Set gluon value.
	 */
	void setGluonValue(const GluonValue &gluonValue);

private:

	/**
	 * Map containing stored quark values distinguished by associated quark flavors.
	 */
	std::map<QuarkFlavor::Type, QuarkValue> m_quarkValues;

	/**
	 * Gluon value.
	 */
	GluonValue m_gluonValue;
};

} /* namespace PARTONS */

#endif /* PARTON_VALUES_H */
