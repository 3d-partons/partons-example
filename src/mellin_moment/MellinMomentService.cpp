#include "../../include/mellin_moment/MellinMomentService.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>
#include <partons/beans/automation/BaseObjectData.h>
#include <partons/beans/automation/Task.h>
#include <partons/BaseObjectRegistry.h>
#include <partons/modules/gpd/GPDModule.h>
#include <partons/ModuleObjectFactory.h>
#include <partons/utils/VectorUtils.h>
#include <partons/Partons.h>
#include <map>

#include "../../include/mellin_moment/MellinMomentModule.h"
#include "../../include/parton_value/PartonValues.h"

namespace PARTONS {

const std::string MellinMomentService::MELLIN_MOMENT_SERVICE_N = "N";
const std::string MellinMomentService::MELLIN_MOMENT_SERVICE_COMPUTE_ONE_KINEMATIC =
		"computeOneKinematic";
const std::string MellinMomentService::MELLIN_MOMENT_SERVICE_COMPUTE_MANY_KINEMATIC =
		"computeManyKinematic";
const std::string MellinMomentService::FUNCTION_NAME_GENERATE_PLOT_FILE =
		"generatePlotFile";

// Initialise [class]::classId with a unique name and selfregister this module into the global registry.
const unsigned int MellinMomentService::classId =
		Partons::getInstance()->getBaseObjectRegistry()->registerBaseObject(
				new MellinMomentService("MellinMomentService"));

MellinMomentService::MellinMomentService(const std::string &className) :
		ServiceObjectTyped<MellinMomentKinematic, MellinMomentResult>(className) {
}

MellinMomentService::~MellinMomentService() {
}

void MellinMomentService::resolveObjectDependencies() {
	ServiceObject::resolveObjectDependencies();

	try {
		m_batchSize = ElemUtils::GenericType(
				ElemUtils::PropertiesManager::getInstance()->getString(
						"mellin.moment.service.batch.size")).toUInt();
	} catch (const std::exception &e) {
		throw ElemUtils::CustomException(getClassName(), __func__, e.what());
	}
}

MellinMomentResult MellinMomentService::compute(const MellinMomentKinematic &kinematic, MellinMomentModule* pMellinMoment, const List<GPDType> & gpdTypeList) const {

	MellinMomentResult result;

	List<GPDType> restrictedByGPDTypeListFinal = getFinalGPDTypeList(pMellinMoment,gpdTypeList);

	for (unsigned int i = 0; i != restrictedByGPDTypeListFinal.size(); i++) {
		PartonValues values = pMellinMoment->computeAll(kinematic,	restrictedByGPDTypeListFinal[i]);
		result.addPartonValues(restrictedByGPDTypeListFinal[i], values);
	}

	result.setKinematic(kinematic);
	result.setComputationModuleName(pMellinMoment->getClassName());

	return result;
}

List<MellinMomentResult> MellinMomentService::computeManyKinematic(
		const List<MellinMomentKinematic> &kinematicList,
		MellinMomentModule* pMellinModule, const List<GPDType> &gpdTypeList,
		const bool storeInDB) {
	debug(__func__, "Processing ...");

	info(__func__,
			ElemUtils::Formatter() << pMellinModule->getN()
					<< "-th Mellin moment will be computed at "
					<< kinematicList.size() << " kinematic(s)");

	List<MellinMomentResult> results;
	List<ElemUtils::Packet> listOfPacket;

	List<GPDType> finalGPDTypeList = getFinalGPDTypeList(pMellinModule,gpdTypeList);

	if (finalGPDTypeList.size() == 0) {
		info(__func__,
				"Nothing to compute with your computation configuration ; there is no GPDType available");
		return results;

	}

	initComputationalThread(pMellinModule);

	info(__func__, "Thread(s) running ...");

	// ##### Batch feature start section #####
	unsigned int i = 0;
	unsigned int j = 0;

	while (i != kinematicList.size()) {
		listOfPacket.clear();
		j = 0;

		while ((j != m_batchSize) && (i != kinematicList.size())) {
			ElemUtils::Packet packet;
			MellinMomentKinematic kinematic;
			kinematic = kinematicList[i];
			packet << kinematic << finalGPDTypeList;
			listOfPacket.add(packet);
			i++;
			j++;
		}

		addTasks(listOfPacket);

		launchAllThreadAndWaitingFor();
		sortResultList();

		info(__func__,
				ElemUtils::Formatter() << "Kinematic(s) already computed : "
						<< i);

		updateResultInfo(getResultList(), m_resultInfo);

		if (storeInDB) {
			//TODO implement class MellinMomentResultDaoService gpdResultDaoService;
			//
			// info(__func__,
			//		ElemUtils::Formatter()
			//				<< "Results have been stored with computation_id = "
			//				<< gpdResultDaoService.insert(getResultList()));
		} else {
			results.add(getResultList());
		}

		clearResultListBuffer();
	}
	// ##### Batch feature end section #####

	clearAllThread();

	return results;
}

void MellinMomentService::computeTask(Task &task) {
	debug(__func__, "Processing ...");

	ServiceObjectTyped<MellinMomentKinematic, MellinMomentResult>::computeTask(task);

	List<MellinMomentResult> resultList;

	if (ElemUtils::StringUtils::equals(task.getFunctionName(),
			MellinMomentService::MELLIN_MOMENT_SERVICE_COMPUTE_MANY_KINEMATIC)) {
		resultList.add(computeManyKinematicTask(task));
	} else {
		if (ElemUtils::StringUtils::equals(task.getFunctionName(),
				MellinMomentService::MELLIN_MOMENT_SERVICE_COMPUTE_ONE_KINEMATIC)) {
			resultList.add(computeOneKinematicTask(task));
		} else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
				MellinMomentService::FUNCTION_NAME_GENERATE_PLOT_FILE)) {
			generatePlotFileTask(task);
		} else if (!ServiceObjectTyped<MellinMomentKinematic, MellinMomentResult>::computeGeneralTask(
				task)) {
			errorUnknownMethod(task);
		}

		updateResultInfo(resultList, m_resultInfo);

		if (task.isStoreInDB()) {
			//TODO implemented connection with Data Base
			//GPDResultDaoService gpdResultDaoService;
			//gpdResultDaoService.insert(resultList);
		}

	}

	m_resultListBuffer = resultList;
}

MellinMomentResult MellinMomentService::computeOneKinematicTask(Task& task) {
	MellinMomentKinematic kinematic = newKinematicFromTask(task);

	MellinMomentModule* pMellinModule = newMellinMomentModuleFromTask(task);
	int n = getNFromTask(task);

	MellinMomentResult result = compute(kinematic,pMellinModule);

	// Remove reference to pGPDModule pointer.
	m_pModuleObjectFactory->updateModulePointerReference(pMellinModule, 0);
	pMellinModule = 0;

	return result;
}

List<MellinMomentResult> MellinMomentService::computeManyKinematicTask(
		Task& task) {
	List<MellinMomentKinematic> listOfKinematic = newListOfKinematicFromTask(task);

	List<GPDType> gpdTypeList = getGPDTypeListFromTask(task);

	int n = getNFromTask(task);

	MellinMomentModule* pMellinModule = newMellinMomentModuleFromTask(task);

	List<MellinMomentResult> result = computeManyKinematic(listOfKinematic,pMellinModule,
			gpdTypeList, task.isStoreInDB());

// Remove reference to pGPDModule pointer.
	m_pModuleObjectFactory->updateModulePointerReference(pMellinModule, 0);
	pMellinModule = 0;
	return result;
}

void MellinMomentService::generatePlotFileTask(Task& task) {
	generatePlotFile(getOutputFilePathForPlotFileTask(task),
			generateSQLQueryForPlotFileTask(task, "mellin_moment_plot_2d_view"), ' ');
}

List<GPDType> MellinMomentService::getFinalGPDTypeList(MellinMomentModule* pMellinModule,
		const List<GPDType> &gpdTypeList) const {

	List<GPDType> restrictedByGPDTypeListFinal =
			pMellinModule->getListOfAvailableGPDType();

	if (!gpdTypeList.isEmpty()) {
		restrictedByGPDTypeListFinal = VectorUtils::intersection(
				restrictedByGPDTypeListFinal, gpdTypeList);
	}

	info(__func__,
			ElemUtils::Formatter() << restrictedByGPDTypeListFinal.size()
					<< " GPDType will be computed");

	return restrictedByGPDTypeListFinal;
}

MellinMomentModule* MellinMomentService::newMellinMomentModuleFromTask(const Task& task) const {
	MellinMomentModule* pMellinModule = 0;

	if (ElemUtils::StringUtils::equals(
			task.getModuleComputationConfiguration().getModuleType(),
			MellinMomentModule::MELLIN_MOMENT_MODULE_CLASS_NAME)) {
		pMellinModule =	static_cast<PARTONS::MellinMomentModule*>(
				PARTONS::Partons::getInstance()->getModuleObjectFactory()->newModuleObject(
						task.getModuleComputationConfiguration().getModuleClassName()));

		pMellinModule->configure(
				task.getModuleComputationConfiguration().getParameters());

		pMellinModule->prepareSubModules(
				task.getModuleComputationConfiguration().getSubModules());
	} else {
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter()
						<< "You have not provided any GPDModule");
	}

	return pMellinModule;
}

int MellinMomentService::getNFromTask(const Task& task) const
{
	//TODO: Implement
	return 1;
}

MellinMomentKinematic MellinMomentService::newKinematicFromTask(const Task& task) const {
	MellinMomentKinematic kinematic;

	if (ElemUtils::StringUtils::equals(
			task.getKinematicsData().getModuleClassName(), "MellinMomentKinematic")) {
		kinematic = MellinMomentKinematic(task.getKinematicsData().getParameters());
	} else {
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter()
						<< "Missing object : <MellinMomentKinematic> for method "
						<< task.getFunctionName());
	}

	return kinematic;
}

List<MellinMomentKinematic> MellinMomentService::newListOfKinematicFromTask(
		const Task& task) const {
	List<MellinMomentKinematic> listOfKinematic;

	if (ElemUtils::StringUtils::equals(
			task.getKinematicsData().getModuleClassName(), "MellinMomentKinematic")) {

		if (task.getKinematicsData().getParameters().isAvailable("file")) {

			//TODO Missing getMellinMomentKinematicFromFile
			//listOfKinematic =
			//		KinematicUtils().getGPDKinematicFromFile(
			//				task.getKinematicsData().getParameters().getLastAvailable().getString());
		} else {
			throw ElemUtils::CustomException(getClassName(), __func__,
					ElemUtils::Formatter()
							<< "Missing parameter file in object <MellinMomentKinematic> for method "
							<< task.getFunctionName());
		}
	} else {
		throw ElemUtils::CustomException(getClassName(), __func__,
				ElemUtils::Formatter()
						<< "Missing object : <MellinMomentKinematic> for method "
						<< task.getFunctionName());
	}

	return listOfKinematic;
}



} /* namespace PARTONS */
