#ifndef MELLIN_MOMENT_SERVICE_H
#define MELLIN_MOMENT_SERVICE_H

/**
 * @file MellinMomentResult.h
 * @author: Arkadiusz P. Trawiński (DPhN/CEA-Saclay & CPHT/École polytechnique)
 * @date 27 March 2019
 * @version 1.0
 */

#include <partons/beans/gpd/GPDResult.h>
#include <partons/beans/gpd/GPDType.h>
#include <partons/beans/List.h>
#include <partons/ServiceObjectTyped.h>
#include <string>

#include "MellinMomentKinematic.h"
#include "MellinMomentResult.h"

namespace PARTONS {
class MellinMoment;
} /* namespace PARTONS */

namespace PARTONS {
class GPDModule;
class MellinMomentResult;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class MellinMomentService
 *
 * @brief Singleton to handle and compute Mellin Moments.
 *
 *
 * This service is used to call Mellin Moment module, which compute Mellin moments at the given kinematics using different GPD models.
 * It also takes care of the evolution of GPDs.
 */
class MellinMomentService: public ServiceObjectTyped<MellinMomentKinematic, MellinMomentResult> {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string MELLIN_MOMENT_SERVICE_N; ///<  Name of the Mellin Moment index parameter;
    static const std::string MELLIN_MOMENT_SERVICE_COMPUTE_GPD_MODEL_WITH_EVOLUTION; ///< Name of the XML task used to compute a GPD with evolution.
    static const std::string MELLIN_MOMENT_SERVICE_COMPUTE_ONE_KINEMATIC; ///< Name of the XML task used to compute a GPD at given kinematics.
    static const std::string MELLIN_MOMENT_SERVICE_COMPUTE_MANY_KINEMATIC; ///< Name of the XML task used to compute GPDs for a list of kinematics.
    static const std::string FUNCTION_NAME_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

    /**
     * Default constructor used by the registry. Do not use directly!
     */
    MellinMomentService(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~MellinMomentService();

    /**
     * See parent class for details.
     */
    virtual void resolveObjectDependencies();

    /**
     * Computes n-th Mellin moment of GPD model at specific kinematic.
     *
     * @param n index of Mellin Moment.
     * @param kinematic Mellin Moment kinematic.
     * @param pGPDModule GPDModule to use.
     * @param gpdType List of GPDType to compute. Default: all the GPDTypes available with the GPDModule.
     * @return GPDResult
     */
    MellinMomentResult compute(int n, const MellinMomentKinematic &kinematic,
            GPDModule* pGPDModule, const List<GPDType> & gpdType =
                    List<GPDType>()) const;

    /**
     * Computes n-th Mellin moments of GPD model for a list of kinematics.
     *
     * @param n index of Mellin Moment.
     * @param kinematicList List of GPDKinematics.
     * @param pGPDModule GPDModule to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with the GPDModule.
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of GPDResults.
     */
    List<MellinMomentResult> computeManyKinematic(int n,
            const List<MellinMomentKinematic> &kinematicList, GPDModule* pGPDModule,
            const List<GPDType> &gpdTypeList = List<GPDType>(),
            const bool storeInDB = 0);

    /**
     * Method used in automation to compute given tasks.
     * @param task Automation task to compute.
     */
    virtual void computeTask(Task &task);

    int getNFromTask(const Task& task) const;

    /**
     * Devises the GPDModule from an automation task.
     *
     * @param task Automation task.
     * @return GPDModule pointer.
     */
    GPDModule* newGPDModuleFromTask(const Task &task) const;

    // GPDModule* configureGPDModule(GPDModule* pGPDModule) const;

    /**
     * Devises the GPD kinematics from an automation task.
     *
     * @param task Automation task.
     * @return GPD kinematics.
     */
    MellinMomentKinematic newKinematicFromTask(const Task &task) const;
    /**
     * Devises the GPD kinematics from an automation task.
     *
     * @param task Automation task.
     * @return List of GPD kinematics.
     */
    List<MellinMomentKinematic> newListOfKinematicFromTask(const Task &task) const;

private:
    GPDModule* m_pGPDModule; ///< Pointer to the currently used GPD module.

    /**
     * Method used in the automated interface to compute GPD.
     * @param task Automated XML task.
     * @return GPDResult object.
     */
    MellinMomentResult computeMellinMomentTask(Task &task);
    /**
     * Method used in the automated interface to compute GPDs for a list of kinematics.
     * @param task Automated XML task.
     * @return List of GPD results.
     */
    List<MellinMomentResult> computeManyKinematicTask(Task &task);
    /**
     * Method used in the automated interface to generate a data file ready for plotting.
     * @param task Automated XML task.
     */
    void generatePlotFileTask(Task &task);

    /**
     * Method used to derive an intersection of available GPD types from the various underlying modules.
     * @param pGPDModule GPDModule used for the computation.
     * @param gpdTypeList List of desired GPD types to compute.
     * @return List of GPD types.
     */
    List<GPDType> getFinalGPDTypeList(GPDModule* pGPDModule,
            const List<GPDType> &gpdTypeList) const;
};

} /* namespace PARTONS */

#endif /* MELLIN_MOMENT_SERVICE_H */
