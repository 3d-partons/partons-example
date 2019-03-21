#ifndef QUARK_VALUE_H
#define QUARK_VALUE_H

/**
 * @file QuarkValue.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 April 2015
 * @version 1.0
 */

#include <partons/beans/QuarkFlavor.h>
#include <partons/BaseObject.h>
#include <partons/utils/compare/ComparisonReport.h>
#include <string>

namespace PARTONS {

/**
 * @class QuarkValue
 *
 * @brief Container to store values for one type of quark.
 *
 * This class represents a value of one quark at a single kinematic point. For example, it can be a quark Mellin Moments of a given type at some Mellin Moment kinematics.
 */
class QuarkValue: public BaseObject {
public:

    static const std::string QUARK_VALUE_DB_COLUMN_NAME_QUARK_VALUE;
    static const std::string QUARK_VALUE_DB_COLUMN_NAME_QUARK_VALUE_PLUS;
    static const std::string QUARK_VALUE_DB_COLUMN_NAME_QUARK_VALUE_MINUS;

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    QuarkValue(const QuarkValue &other);

    /**
     * Assignment constructor.
     * @param quarkFlavor Quark flavor to be assigned.
     * @param quarkValue Value for quark to be assigned.
     */
    QuarkValue(QuarkFlavor::Type quarkFlavor = QuarkFlavor::UNDEFINED,
            double quarkValue = 0.);

    /**
     * Destructor.
     */
    virtual ~QuarkValue();

    virtual std::string toString() const;

    /**
     * Compare to other QuarkValue object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const QuarkValue &referenceObject,
            std::string parentObjectInfo = "") const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get quark flavor assigned to this object.
     */
    QuarkFlavor getQuarkFlavor() const;

    /**
     * Set quark flavor assigned to this object.
     */
    void setQuarkFlavor(QuarkFlavor quarkFlavorType);

    /**
     * Get value for quark.
     */
    double getQuarkValue() const;

    /**
     * Set value for quark.
     */
    void setQuarkValue(double quarkValue);


private:

    /**
     * Quark flavor assigned to this object.
     */
    QuarkFlavor m_quarkFlavor;

    /**
     * Value for quark.
     */
    double m_quarkValue;
};

} /* namespace PARTONS */

#endif /* QUARK_VALUE_H */
