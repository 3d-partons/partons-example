#ifndef GLUON_VALUE_H
#define GLUON_VALUE_H

/**
 * @file QuarkValue.h
 * @author: Arkadiusz P. Trawiński (DPhN/CEA-Saclay & CPHT/École polytechnique)
 * @date 21 March 2019
 * @version 1.0
 */

#include <partons/BaseObject.h>
#include <partons/utils/compare/ComparisonReport.h>
#include <string>

namespace PARTONS {

class ComparisonReport;

/**
 * @class GluonValue
 *
 * @brief Container to store value for gluons.
 *
 * This class represents a value computed for gluons at a single kinematic point. For example, it can be a gluon Mellin Moments of a given type at some Mellin Moment kinematics.
 */
class GluonValue: public BaseObject {

public:

    /**
     * Name of field in the database storing value of gluon Value.
     */
    static const std::string GLUON_VALUE_DB_COLUMN_NAME;

    /**
     *
     */
    static const std::string GLUON_VALUE_PARAMETER_NAME_GLUON_VALUE;

    /**
     * Default constructor.
     */
    GluonValue();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GluonValue(const GluonValue &other);

    /**
     * Assignment constructor.
     * @param gluonValue Value to be assigned.
     */
    GluonValue(double gluonValue);

    /**
     * Destructor.
     */
    virtual ~GluonValue();

    virtual std::string toString() const;

    /**
     * Compare to other GluonValue object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const GluonValue &referenceObject,
            std::string parentObjectInfo = "") const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get value of gluon Value.
     */
    double getGluonValue() const;

    /**
     * Set value of gluon Value.
     */
    void setGluonValue(double gluonValue);

    /**
     * Check if any value of gluon Value has been set.
     */
    bool isNullObject() const;

    /**
     * Set GluonValue::m_nullObject.
     */
    void setNullObject(bool nullObject);

private:

    /**
     * Value of gluon Value.
     */
    double m_gluonValue;

    /**
     * Variable to check if any value of gluon Value has been set.
     */
    bool m_nullObject;
};

} /* namespace PARTONS */

#endif /* GLUON_VALUE_H */
