#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jul 29 09:32:51 2021

@author: maximelucas
"""

import pypartons
import numpy as np

p = pypartons.PARTONS
e = pypartons.ElemUtils
n = pypartons.NumA

p.Partons.init()
inst = p.Partons.getInstance()

#gpdKinematicList = p.KinematicUtils().getGPDKinematicFromFile("data/examples/gpd/kinematics_gpd.csv")
#gpdResultList = gpdService.computeManyKinematic(gpdKinematicList, gpdModel)
#for i in range(gpdResultList.size()):
#    print(i, gpdResultList.get(i).getPartonDistribution(p.GPDType.H).getQuarkDistribution(p.QuarkFlavor.UP).getQuarkDistribution())

# Define test functions
def computeSingleKinematicsForGPD():
    gpdService = inst.getServiceObjectRegistry().getGPDService()
    gpdModel = inst.getModuleObjectFactory().newGPDModule("GPDGK16")
    gpdKinematic = p.GPDKinematic(0.1, 0.2, -0.1, 2, 2)
    gpdResult = gpdService.computeSingleKinematic(gpdKinematic, gpdModel)
    print(gpdResult.toString())
    inst.getLoggerManager().info("example", "info", gpdResult.toString())

def computeManyKinematicsForGPD():
    gpdService = inst.getServiceObjectRegistry().getGPDService()
    gpdModel = inst.getModuleObjectFactory().newGPDModule("GPDGK16")
    gpdKinematicList = p.KinematicUtils().getGPDKinematicFromFile("data/examples/gpd/kinematics_gpd.csv")
    gpdResultList = gpdService.computeManyKinematic(gpdKinematicList, gpdModel)
    print(gpdResultList.toString())
    inst.getLoggerManager().info("example", "info", gpdResultList.toString())
    
def computeSingleKinematicsForDVCSComptonFromFactor():
    dvcsConvolCoeffFunctionService = inst.getServiceObjectRegistry().getDVCSConvolCoeffFunctionService()
    gpdModule = inst.getModuleObjectFactory().newGPDModule(p.GPDGK16.classId)
    dvcsCFFModule = inst.getModuleObjectFactory().newDVCSConvolCoeffFunctionModule(p.DVCSCFFStandard.classId)
    parameters = e.Parameters(p.PerturbativeQCDOrderType.PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, p.PerturbativeQCDOrderType.LO)
    dvcsCFFModule.configure(parameters)
    dvcsCFFModule.setGPDModule(gpdModule)
    cffKinematic = p.DVCSConvolCoeffFunctionKinematic(0.01, -0.1, 4, 4, 4)
    cffResult = dvcsConvolCoeffFunctionService.computeSingleKinematic(cffKinematic, dvcsCFFModule)
    print(cffResult.toString())
    inst.getLoggerManager().info("example", "info", cffResult.toString())
    
def computeManyKinematicsForDVCSComptonFromFactor():
    dvcsConvolCoeffFunctionService = inst.getServiceObjectRegistry().getDVCSConvolCoeffFunctionService()
    gpdModule = inst.getModuleObjectFactory().newGPDModule(p.GPDGK16.classId)
    dvcsCFFModule = inst.getModuleObjectFactory().newDVCSConvolCoeffFunctionModule(p.DVCSCFFStandard.classId)
    parameters = e.Parameters(p.PerturbativeQCDOrderType.PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, p.PerturbativeQCDOrderType.LO)
    dvcsCFFModule.configure(parameters)
    dvcsCFFModule.setGPDModule(gpdModule)
    cffKinematicList = p.KinematicUtils().getDVCSCCFKinematicFromFile("data/examples/cff/kinematics_dvcs_cff.csv")
    cffResultList = dvcsConvolCoeffFunctionService.computeManyKinematic(cffKinematicList, dvcsCFFModule)
    print(cffResultList.toString())
    inst.getLoggerManager().info("example", "info", cffResultList.toString())
    
def computeSingleKinematicsForDVCSObservable():
    observableService = inst.getServiceObjectRegistry().getDVCSObservableService()
    gpdModule = inst.getModuleObjectFactory().newGPDModule(p.GPDGK16.classId)
    dvcsCFFModule = inst.getModuleObjectFactory().newDVCSConvolCoeffFunctionModule(p.DVCSCFFStandard.classId)
    dvcsCFFModule.configure(e.Parameters(p.PerturbativeQCDOrderType.PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, p.PerturbativeQCDOrderType.LO))
    xiConverterModule = inst.getModuleObjectFactory().newDVCSXiConverterModule(p.DVCSXiConverterXBToXi.classId)
    scalesModule = inst.getModuleObjectFactory().newDVCSScalesModule(p.DVCSScalesQ2Multiplier.classId)
    scalesModule.configure(e.Parameters(p.DVCSScalesQ2Multiplier.PARAMETER_NAME_LAMBDA, e.GenericType(1)))
    processModule = inst.getModuleObjectFactory().newDVCSProcessModule(p.DVCSProcessGV08.classId)
    observable = inst.getModuleObjectFactory().newDVCSObservable(p.DVCSAllMinus.classId)
    observable.setProcessModule(processModule)
    processModule.setScaleModule(scalesModule)
    processModule.setXiConverterModule(xiConverterModule)
    processModule.setConvolCoeffFunctionModule(dvcsCFFModule)
    dvcsCFFModule.setGPDModule(gpdModule)
    observableKinematic = p.DVCSObservableKinematic(0.2, -0.1, 2, 6, 0)
    observableResult = observableService.computeSingleKinematic(observableKinematic, observable)
    print(observableResult.toString())
    inst.getLoggerManager().info("example", "info", observableResult.toString())
        
def computeManyKinematicsForDVCSObservable():
    observableService = inst.getServiceObjectRegistry().getDVCSObservableService()
    gpdModule = inst.getModuleObjectFactory().newGPDModule(p.GPDGK16.classId)
    dvcsCFFModule = inst.getModuleObjectFactory().newDVCSConvolCoeffFunctionModule(p.DVCSCFFStandard.classId)
    dvcsCFFModule.configure(e.Parameters(p.PerturbativeQCDOrderType.PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, p.PerturbativeQCDOrderType.LO))
    xiConverterModule = inst.getModuleObjectFactory().newDVCSXiConverterModule(p.DVCSXiConverterXBToXi.classId)
    scalesModule = inst.getModuleObjectFactory().newDVCSScalesModule(p.DVCSScalesQ2Multiplier.classId)
    scalesModule.configure(e.Parameters(p.DVCSScalesQ2Multiplier.PARAMETER_NAME_LAMBDA, e.GenericType(1)))
    processModule = inst.getModuleObjectFactory().newDVCSProcessModule(p.DVCSProcessGV08.classId)
    observable = inst.getModuleObjectFactory().newDVCSObservable(p.DVCSAllMinus.classId)
    observable.setProcessModule(processModule)
    processModule.setScaleModule(scalesModule)
    processModule.setXiConverterModule(xiConverterModule)
    processModule.setConvolCoeffFunctionModule(dvcsCFFModule)
    dvcsCFFModule.setGPDModule(gpdModule)
    observableKinematicList = p.KinematicUtils().getDVCSObservableKinematicFromFile("data/examples/observable/kinematics_dvcs_observable.csv")
    observableResultList = observableService.computeManyKinematic(observableKinematicList, observable)
    print(observableResultList.toString())
    inst.getLoggerManager().info("example", "info", observableResultList.toString())
    
def changeIntegrationRoutine():
    gpdService = inst.getServiceObjectRegistry().getGPDService()
    gpdModel = inst.getModuleObjectFactory().newGPDModule("GPDGK16Numerical")
    gpdModel.configure(e.Parameters(p.MathIntegratorModule.PARAM_NAME_INTEGRATOR_TYPE, n.IntegratorType1D.GK21_ADAPTIVE))
    gpdKinematic = p.GPDKinematic(0.1, 0.2, -0.1, 2, 2)
    gpdResult = gpdService.computeSingleKinematic(gpdKinematic, gpdModel)
    print(gpdResult.toString())
    inst.getLoggerManager().info("example", "info", gpdResult.toString())
    
def makeUseOfGPDEvolution():
    gpdService = inst.getServiceObjectRegistry().getGPDService()
    gpdModel = inst.getModuleObjectFactory().newGPDModule("GPDGK16")
    gpdEvolutionModel = inst.getModuleObjectFactory().newGPDEvolutionModule("GPDEvolutionVinnikov")
    runningAlphaStrongModule = inst.getModuleObjectFactory().newRunningAlphaStrongModule("RunningAlphaStrongVinnikov")
    activeFlavorsThresholdsModule = inst.getModuleObjectFactory().newActiveFlavorsThresholdsModule("ActiveFlavorsThresholdsConstant")
    activeFlavorsThresholdsModule.setNFlavors(3)
    parameters = e.Parameters()
    parameters.add(n.QuadratureIntegrator1D.PARAM_NAME_N, 2)
    parameters.add(p.PerturbativeQCDOrderType.PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, p.PerturbativeQCDOrderType.LO)
    gpdEvolutionModel.configure(parameters)
    gpdEvolutionModel.setRunningAlphaStrongModule(runningAlphaStrongModule)
    gpdEvolutionModel.setActiveFlavorsModule(activeFlavorsThresholdsModule)
    gpdModel.setEvolQcdModule(gpdEvolutionModel)
    gpdKinematic = p.GPDKinematic(0.1, 0.2, -0.1, 40, 40)
    gpdResult = gpdService.computeSingleKinematic(gpdKinematic, gpdModel)
    print(gpdResult.toString())
    inst.getLoggerManager().info("example", "info", gpdResult.toString())
    
def selectSpecificGPDTypes():
    gpdService = inst.getServiceObjectRegistry().getGPDService()
    gpdModel = inst.getModuleObjectFactory().newGPDModule("GPDGK16")
    gpdTypeList = p.ListGPDType()
    gpdTypeList.add(p.GPDType.E)
    gpdTypeList.add(p.GPDType.Et)
    gpdKinematic = p.GPDKinematic(0.1, 0.2, -0.1, 2, 2)
    gpdResult = gpdService.computeSingleKinematic(gpdKinematic, gpdModel, gpdTypeList)
    print(gpdResult.toString())
    inst.getLoggerManager().info("example", "info", gpdResult.toString())
    
def demonstrateUnits():
    gpdService = inst.getServiceObjectRegistry().getGPDService()
    gpdModel = inst.getModuleObjectFactory().newGPDModule("GPDGK16")
    x = p.PhysicalTypedouble(0.1, p.PhysicalUnit.NONE)
    xi = p.PhysicalTypedouble(0.2, p.PhysicalUnit.NONE)
    t = p.PhysicalTypedouble(-0.1, p.PhysicalUnit.GEV2)
    muF2 = p.PhysicalTypedouble(2, p.PhysicalUnit.GEV2)
    muR2 = p.PhysicalTypedouble(2, p.PhysicalUnit.GEV2)
    tInMeV2_a = t.makeSameUnitAs(p.PhysicalUnit.MEV2)
    tInMeV2_b = t
    tInMeV2_b.makeSameUnitAs(tInMeV2_a)
    gpdKinematik = p.GPDKinematic(x, xi, tInMeV2_a, muF2, muR2)
    gpdResult = gpdService.computeSingleKinematic(gpdKinematik, gpdModel)
    print(gpdResult.toString())
    inst.getLoggerManager().info("example", "info", gpdResult.toString())

# Call test function
computeSingleKinematicsForGPD()
#computeManyKinematicsForGPD()
#computeSingleKinematicsForDVCSComptonFromFactor()
#computeManyKinematicsForDVCSComptonFromFactor()
#computeSingleKinematicsForDVCSObservable()
#computeManyKinematicsForDVCSObservable()
#changeIntegrationRoutine()
#makeUseOfGPDEvolution()
#selectSpecificGPDTypes()
#demonstrateUnits()
