#ifndef MELLIN_MOMENT_RESULT_H
#define MELLIN_MOMENT_RESULT_H

/**
 * @file MellinMomentResult.h
 * @author: Arkadiusz P. Trawiński (DPhN/CEA-Saclay & CPHT/École polytechnique)
 * @date 21 March 2019
 * @version 1.0
 */

#include <partons/beans/gpd/GPDKinematic.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/List.h>
#include <partons/beans/parton_distribution/PartonDistribution.h>
#include <partons/beans/Result.h>
#include <partons/utils/compare/ComparisonReport.h>
#include <stddef.h>
#include <map>
#include <string>
#include <vector>

#include "../parton_value/PartonValues.h"
#include "MellinMomentKinematic.h"

namespace PARTONS {

/**
 * @class MellinMomentResult
 *
 * @brief Class representing single result of Mellin Moment computation.
 *
 * This class is used to store results of a single Mellin Moment computation. In other words, the class stores a collection of PartonValues objects, with each of them containing a part of the result defined for a specific GPD type.
 */
class MellinMomentResult: public Result {
public:

    /**
     * Name of table in the database corresponding to this class.
     */
    static const std::string MELLIN_MOMENT_RESULT_DB_TABLE_NAME;

    /**
     * Default constructor.
     */
    MellinMomentResult();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    MellinMomentResult(const MellinMomentResult &other);

    /**
     * Destructor.
     */
    virtual ~MellinMomentResult();

    /**
     * Add parton values associated to given GPD type.
     * @param gpdType GPD type of parton values to be inserted.
     * @param partonValues Parton values to be added.
     */
    void addPartonValues(GPDType::Type gpdType,
            PartonValues partonValues);

    /**
     * Get reference to parton values associated to given GPD type.
     * @param gpdType GPD type associated to parton values to be selected.
     * @return Reference to selected parton values.
     */
    const PartonValues& getPartonValues(
            GPDType::Type gpdType) const;

    /**
     * Get list of stored parton values.
     * @return Retrieved List of PartonValues objects.
     */
    List<PartonValues> getListOfPartonValues() const;

    /**
     * Get list of GPD types associated to stored parton values.
     * @return Vector of associated types.
     */
    std::vector<GPDType> getListOfGPDType();

    virtual std::string toString() const;

    /**
     * Get string containing information on stored data.
     * @return String with returned information.
     */
    virtual std::string getObjectInfo() const;

    /**
     * Compare to other MellinMomentResult object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const MellinMomentResult &referenceObject,
            std::string parentObjectInfo = "") const;

    /**
     * Check if the object stores parton distribution of given GPD type.
     * @param gpdType GPD type to be checked.
     * @return True if the object stores parton values of given GPD type, otherwise false.
     * @see MellinMomentResult::getLastAvailable()
     */
    bool isAvailable(const GPDType::Type &gpdType) const;

    /**
     * Get reference to parton value marked by the last call of MellinMomentResult::isAvailible() function.
     * @return Reference to selected parton values.
     * @see MellinMomentResult::isAvailible()
     */
    PartonValues& getLastAvailable() const;

    /**
     * Get number of stored parton values.
     * @return Number of stored parton values.
     */
    size_t size() const;

    /**
     * Relation operator that checks if the value of left operand is less than the value of right operand (in this case returned is this->m_kinematic < other.m_kinematic).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const MellinMomentResult &other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get reference to map containing stored parton distributions distinguished by associated GPD types.
     */
    const std::map<GPDType::Type, PartonValues>& getPartonValues() const;

    /**
     * Set map containing stored parton values distinguished by associated GPD types.
     */
    void setPartonValues(
            const std::map<GPDType::Type, PartonValues>& partonValues);

    /**
     * Get reference to GPD kinematics associated to this result.
     */
    const MellinMomentKinematic& getKinematic() const;

    /**
     * Set GPD kinematics associated to this result.
     */
    void setKinematic(const MellinMomentKinematic& kinematic);

private:

    /**
     * Map containing stored parton values distinguished by associated GPD types.
     */
    std::map<GPDType::Type, PartonValues> m_partonValues;

    /**
     * Iterator used to mark a specific entry in MellinMomentResult::m_partonValues.
     */
    mutable std::map<GPDType::Type, PartonValues>::const_iterator m_it;

    /**
     * GPD kinematics associated to this result.
     */
    GPDKinematic m_kinematic;
};

} /* namespace PARTONS */

#endif /* MELLIN_MOMENT_RESULT_H */
